#include "MainComponent.h"
#include "bullshit.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (backgroundColor);

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::black);
    g.drawText (currentSizeAsString, getLocalBounds(), juce::Justification::centred, true);
    g.fillRect(500, 5, 100, 200);
    g.setColour(juce::Colours::aqua);
    g.setFont (juce::Font (48.0f));
    g.setColour (juce::Colours::red);
    g.drawText (printBullshit(), getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    auto rnd = juce::Random();
    currentSizeAsString = juce::String(getWidth()) + " x " + juce::String(getHeight());
    backgroundColor = juce::Colour((uint8_t) 100, rnd.nextInt(250), 200, (uint8_t) 200);
}
