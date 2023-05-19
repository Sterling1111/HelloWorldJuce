#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <juce_gui_extra/juce_gui_extra.h>
#include "System.h"
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

    //==============================================================================
    void paint (juce::Graphics&) override;
    void update() override;
    bool keyStateChanged(bool isKeyDown) override;


private:
    juce::Colour backgroundColor;
    juce::Rectangle<float> lcdScreen;
    std::vector<std::vector<juce::Rectangle<float>>> pixels;
    System system{0x00, 0x3fff, 0x6000, 0x7fff,
                  0x8000, 0xffff, 1};


    //==============================================================================
    // Your private member variables go here...

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
