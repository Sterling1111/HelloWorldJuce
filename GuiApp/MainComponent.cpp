#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    backgroundColor = juce::Colour((uint8_t)31, 31, 255, (uint8_t)255);
    pixels.resize(system.lcd.numPixelsX(), std::vector<juce::Rectangle<float>>(system.lcd.numPixelsY()));
    lcdScreen.setSize(380.f, 68.f);
    lcdScreen.setPosition(10.f, 10.f);
    system.loadProgram("a.out");

    for (unsigned y{}; y < system.lcd.numPixelsY(); ++y) {
        for (unsigned x{}; x < system.lcd.numPixelsX(); ++x) {
            pixels[x][y].setSize(3.f, 3.f);
            pixels[x][y].setPosition(lcdScreen.getPosition().x + x * 4.f, lcdScreen.getPosition().y + y * 4);
        }
    }
    setFramesPerSecond(75);
    setSize (400, 88);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (backgroundColor);

    for (unsigned y = 0; y < system.lcd.numPixelsY(); ++y) {
        for (unsigned x = 0; x < system.lcd.numPixelsX(); ++x) {
            auto &pixel = pixels[x][y];

            if(y == 8 || x % 6 == 5) {
                g.setColour(backgroundColor);
                g.fillRect(pixel);
                continue;
            }

            char pixelState{system.lcd.pixelState(x, y)};
            if (pixelState == -1 || pixelState == 0) {
                g.setColour(juce::Colour((uint8_t) 0, 0, 224, (uint8_t) 225));
            } else g.setColour(juce::Colours::white);
            g.fillRect(pixel);
        }
    }
}

void MainComponent::update() {
    using namespace juce::Colours;
    system.cpu.execute(5);
    system.lcd.updatePixels();
}

bool MainComponent::keyStateChanged(bool isKeyDown) {
    static bool NMIB_released{true};
    if(isKeyDown) {
        if(juce::KeyPress::isKeyCurrentlyDown('r')) {
            system.firstReset = true;
            system.cpu.reset(system.eeprom[0xFFFC - 0x8000] | system.eeprom[0xFFFD - 0x8000] << 8);
        }
        if(juce::KeyPress::isKeyCurrentlyDown('n') and NMIB_released) {
            NMIB_released = false;
            system.cpu.NMIB = true;
        }
        if(juce::KeyPress::isKeyCurrentlyDown('i')) system.cpu.IRQB = true;
    } else {
        if(!juce::KeyPress::isKeyCurrentlyDown('n')) {
            system.cpu.NMIB = false;
            NMIB_released = true;
        }
        if(!juce::KeyPress::isKeyCurrentlyDown('i')) system.cpu.IRQB = false;
    }
    return true;
}

