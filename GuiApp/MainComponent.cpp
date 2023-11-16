#include "MainComponent.h"

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
    setSize (600, 400);
    comp.addMouseListener(this, false);
    ownedArrayComponent.setBounds(comp.getX(), comp.getBottom() + 5, getWidth() - comp.getX(), getHeight() - comp.getBottom());
    ownedArrayComponent.addMouseListener(this, true);
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
}