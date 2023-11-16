#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <juce_gui_extra/juce_gui_extra.h>
//#include "System.h"

using namespace juce;

struct MyComp : Component
{
    //void resized() override {}
    void paint(Graphics& g) override { g.fillAll(Colours::green); };

    void mouseEnter(const MouseEvent& e) override {
        DBG("MyComp mouseEnter");
    }
    void mouseExit(const MouseEvent& e) override {
        DBG("MyComp mouseExit");
    }

    void mouseMove(const MouseEvent& e) override {
        DBG("MyComp mouseMoved: " << ++counter);
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
        DBG("MainComponent mouseEnter");
    }
    void mouseExit(const MouseEvent& e) override {
        DBG("MainComponent mouseExit");
    }
    void mouseMove(const MouseEvent& e) override {
        DBG("MainComponent mouseMoved: " << ++counter);
    }
private:
    int counter{};
    MyComp comp;

    //==============================================================================
    // Your private member variables go here...

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
