#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <juce_gui_extra/juce_gui_extra.h>
#include "DataRace.h"
//#include "System.h"

using namespace juce;

struct MyThread : Thread
{
    MyThread() : Thread("MyThread")
    {
        startThread();
    }

    ~MyThread() override
    {
        stopThread(100);
    }

    void run() override
    {
        while( true )
        {
            if(threadShouldExit()) break;


            if(threadShouldExit()) break;


            if(threadShouldExit()) break;

            wait(-1);
        }
    }
};

struct ImageProcessingThread : Thread
{
    ImageProcessingThread(int w_, int h_);
    ~ImageProcessingThread() override;

    void run() override;
    void setUpdateRendererFunc(std::function<void(Image&&)> f);

private:
    int w{}, h{};
    std::function<void(Image&&)> updateRenderer{};
    Random r;
};

struct LambdaTimer : Timer
{
    LambdaTimer(int ms, std::function<void()> f);
    ~LambdaTimer() override;
    void timerCallback() override;
private:
    std::function<void()> lambda;
};

struct Renderer : Component, AsyncUpdater
{
    Renderer();
    ~Renderer() override;
    void paint(Graphics& g) override;
    void handleAsyncUpdate() override;

private:
    std::unique_ptr<ImageProcessingThread> processingThread;
    std::unique_ptr<LambdaTimer> lambdaTimer;
    bool firstImage = true;
    std::array<Image, 2> imageToRender;
};

struct DualButton : Component
{
    DualButton();
    void resized() override;
    void setButton1Handler(std::function<void()> handler);
    void setButton2Handler(std::function<void()> handler);
private:
    TextButton button1 {"button1"}, button2 {"button2"};
};

struct MyAsyncHighResGui : Component, AsyncUpdater, HighResolutionTimer
{
    void handleAsyncUpdate() override
    {
        paintColor = (paintColor + 1) % numColors;
        repaint();
    }

    void hiResTimerCallback() override { triggerAsyncUpdate(); }

    void paint(Graphics& g) override
    {
        switch (paintColor) {
            case 0:
                g.setColour(Colours::green);
                break;
            case 1:
                g.setColour(Colours::red);
                break;
            case 2:
                g.setColour(Colours::blue);
                break;
        }
        g.fillAll();
    }

    MyAsyncHighResGui() { startTimer(1000 / 3); }
    ~MyAsyncHighResGui() override { stopTimer(); }
private:
    int paintColor{};
    const int numColors{3};
};

struct BlinkingThing : Component, Timer
{
    BlinkingThing()
    {
    }
    ~BlinkingThing() override
    {
        stopTimer();
    }

    void timerCallback() override
    {
        isRed = !isRed;
        repaint();
    }

    void paint(Graphics& g) override
    {
        g.fillAll(isRed ? Colours::red : Colours::green);
    }

private:
    bool isRed{};
};

struct Widget : Component
{
    explicit Widget(int i) : num{i} {}

    void paint(Graphics& g) override
    {
        g.fillAll(Colours::red);
        g.setColour(Colours::black);
        g.drawRect(getLocalBounds().reduced(2));
        g.drawFittedText(String(num), getLocalBounds(), Justification::centred, 1);
    };
private:
    int num{};
};

struct OwnedArrayComponent : Component, Button::Listener
{
    OwnedArrayComponent();
    ~OwnedArrayComponent();
    void resized() override;
    void buttonClicked(Button* buttonThatWasClicked) override;
private:
    OwnedArray<TextButton> buttons;
};


struct MyComp : Component
{
    //void resized() override {}
    void paint(Graphics& g) override { g.fillAll(Colours::green); };

    void mouseEnter(const MouseEvent& e) override {
        //DBG("MyComp mouseEnter");
    }
    void mouseExit(const MouseEvent& e) override {
        //DBG("MyComp mouseExit");
    }

    void mouseMove(const MouseEvent& e) override {
        //DBG("MyComp mouseMoved: " << ++counter);
    }
private:
    int counter{};
};
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public juce::AnimatedAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();
    //==============================================================================
    void paint (juce::Graphics&) override;
    void update() override;
    void resized() override;

    void mouseEnter(const MouseEvent& e) override {
        //DBG("MainComponent mouseEnter");
    }
    void mouseExit(const MouseEvent& e) override {
        //DBG("MainComponent mouseExit");
    }
    void mouseMove(const MouseEvent& e) override {
        //DBG("MainComponent mouseMoved: " << ++counter);
    }

    void mouseDown(const MouseEvent& e) override {
        DBG("you clicked the mouse");
    }
private:
    int counter{};
    MyComp comp;
    OwnedArrayComponent ownedArrayComponent;
    BlinkingThing blinkingThing;
    DualButton dualButton;
    MyAsyncHighResGui highResGui;
    Renderer renderer;
    Test test;
    //==============================================================================
    // Your private member variables go here...

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
