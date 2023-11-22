#include "MainComponent.h"
#include <utility>

ImageProcessingThread::ImageProcessingThread(int w_, int h_) :
                                                Thread{"ImageProcessingThread"}, w{w_}, h{h_}
{
    startThread();
}

ImageProcessingThread::~ImageProcessingThread() { stopThread(500); }

void ImageProcessingThread::setUpdateRendererFunc(std::function<void(Image &&)> f)
{
    updateRenderer = std::move(f);
}

void ImageProcessingThread::run()
{
    while( true )
    {
        if(threadShouldExit()) break;

        auto canvas = Image(Image::PixelFormat::RGB, w, h, true);
        bool shouldBail{};

        if(threadShouldExit()) break;

        for (int i = 0; i < w; ++i) {
            if(threadShouldExit()) {
                shouldBail = true;
                break;
            }
            for (int j = 0; j < h; ++j) {
                canvas.setPixelAt(i, j, Colour(r.nextFloat(), r.nextFloat(), r.nextFloat(), 1.f));
            }
        }

        if(shouldBail) break;

        if(threadShouldExit()) break;

        if(updateRenderer)
            updateRenderer(std::move(canvas));

        wait(-1);
    }
}

LambdaTimer::LambdaTimer(int ms, std::function<void()> f) : lambda{ std::move(f) }
{
    startTimer(ms);
}

LambdaTimer::~LambdaTimer()
{
    stopTimer();
}

void LambdaTimer::timerCallback()
{
    stopTimer();
    if(lambda) lambda();
}

Renderer::Renderer()
{
    lambdaTimer = std::make_unique<LambdaTimer>(10, [this](){
        processingThread = std::make_unique<ImageProcessingThread>(getWidth(), getHeight());
        processingThread->setUpdateRendererFunc([this](Image&& image)
        {
           int renderIndex = firstImage ? 0 : 1;
           firstImage = !firstImage;
           imageToRender[renderIndex] = std::move(image);

           triggerAsyncUpdate();

           lambdaTimer = std::make_unique<LambdaTimer>(200, [this]() {
                processingThread->notify();
           });
        });
    });
}

Renderer::~Renderer()
{
    processingThread.reset();
    lambdaTimer.reset();
}

void Renderer::paint(Graphics& g)
{
    g.drawImage(firstImage ? imageToRender[0] : imageToRender[1],
           getLocalBounds().toFloat());
}

void Renderer::handleAsyncUpdate()
{
    repaint();
}

DualButton::DualButton() {
    addAndMakeVisible(button1);
    addAndMakeVisible(button2);
}

void DualButton::setButton1Handler(std::function<void()> handler) {
    button1.onClick = std::move(handler);
}

void DualButton::setButton2Handler(std::function<void()> handler) {
    button2.onClick = std::move(handler);
}

void DualButton::resized() {
    auto bounds = getLocalBounds();
    button1.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
    button2.setBounds(bounds);
}

OwnedArrayComponent::OwnedArrayComponent()
{
    for (int i = 0; i < 10; ++i) {
        auto* button = buttons.add(new TextButton(String(i)));
        addAndMakeVisible(button);
        button->addListener(this);
    }
}

OwnedArrayComponent::~OwnedArrayComponent() {
    for(auto* button : buttons) {
        button->removeListener(this);
    }
}

void OwnedArrayComponent::resized()
{
    auto width = getWidth() / buttons.size();
    int x = 0;
    auto h = getHeight();

    for(auto& button : buttons) {
        button->setBounds(x, 0, width, h);
        x += width;
    }
}

void OwnedArrayComponent::buttonClicked(juce::Button *buttonThatWasClicked) {
    if(buttonThatWasClicked == buttons.getFirst()) {
        DBG("you clicked that first button");
    } else if(buttonThatWasClicked == buttons.getLast()) {
        DBG("you clicked that last button");
    } else {
        DBG("you clicked some other button");
    }
}


//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(comp);
    addAndMakeVisible(ownedArrayComponent);
    addAndMakeVisible(dualButton);
    addAndMakeVisible(blinkingThing);
    addAndMakeVisible(highResGui);
    addAndMakeVisible(renderer);
    setSize (600, 400);
    comp.addMouseListener(this, false);
    ownedArrayComponent.addMouseListener(this, true);

    dualButton.setButton1Handler([this] {
        blinkingThing.startTimerHz(2);
    });

    dualButton.setButton2Handler([this] {
        blinkingThing.startTimerHz(4);
    });
}

MainComponent::~MainComponent()
{
    comp.removeMouseListener(this);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.setFont(Font(16.0f));
    g.setColour(Colours::white);
    g.drawText("Hello World", getLocalBounds(), Justification::centred, true);
}

void MainComponent::update() {

}

void MainComponent::resized() {
    comp.setBounds(30, 30, 100, 100);
    ownedArrayComponent.setBounds(comp.getX(),
                                  comp.getBottom() + 5,
                                  getWidth() - comp.getX(),
                                  getHeight() - comp.getBottom());
    dualButton.setBounds(comp.getBounds().withX(comp.getRight() + 5).withWidth(comp.getWidth() * 2));
    blinkingThing.setBounds(comp.getBounds().withX(dualButton.getRight() + 5));
    highResGui.setBounds(blinkingThing.getBounds().withX(blinkingThing.getRight() + 5));
    renderer.setBounds(highResGui.getBounds().withX(highResGui.getRight() + 5));
}