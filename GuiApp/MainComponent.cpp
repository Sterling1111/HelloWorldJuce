#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    int dog = 3;
    backgroundColor = juce::Colour((uint8_t)31, 31, 255, (uint8_t)255);
    pixelOffColor = juce::Colour((uint8_t)0, 0, 255, (uint8_t)255);
    pixels.resize(system.lcd.numPixelsX(),
                  std::vector<std::pair<juce::Rectangle<float>, juce::Colour>>(system.lcd.numPixelsY()));
    lcdScreen.setSize(380.f, 68.f);
    lcdScreen.setPosition(10.f, 10.f);
    system.loadProgram("a.out");

    for (unsigned y{}; y < system.lcd.numPixelsY(); ++y) {
        for (unsigned x{}; x < system.lcd.numPixelsX(); ++x) {
            auto& [rect, color] = pixels[x][y];
            rect.setSize(3.f, 3.f);
            rect.setPosition(lcdScreen.getPosition().x + x * 4.f, lcdScreen.getPosition().y + y * 4);
            if(y == 8 or x % 6 == 5) color = backgroundColor;
            else if(y > 8) color = pixelOffColor;
            else color = juce::Colours::white;
        }
    }
    setFramesPerSecond(75);
    setSize (400, 88);
}

void MainComponent::updatePixels() {
    system.lcd.updatePixels();

    for (unsigned y{}; y < system.lcd.numPixelsY(); ++y) {
        for (unsigned x{}; x < system.lcd.numPixelsX(); ++x) {
            auto& [rect, color] = pixels[x][y];
            if(system.lcd.pixelState(x, y) == -1) color = backgroundColor;
            else if(system.lcd.pixelState(x, y) == 1) {
                //uint16_t rg = std::min((uint8_t)0xFF, (uint8_t)(color.getRed() * 1.2));
                //color = juce::Colour((uint8_t)rg, rg, 255, (uint8_t)255);
                color = juce::Colours::white;
            }
            else color = pixelOffColor;
            //else color = juce::Colour((uint8_t)(color.getRed() * .8), color.getGreen() * 0.8, 255, (uint8_t)255);
        }
    }
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (backgroundColor);

    for (unsigned y = 0; y < system.lcd.numPixelsY(); ++y) {
        for (unsigned x = 0; x < system.lcd.numPixelsX(); ++x) {
            auto& [rect, color] = pixels[x][y];
            g.setColour(color);
            g.fillRect(rect);
        }
    }
}

void MainComponent::update() {
    using namespace juce::Colours;
    if(!system.firstReset) return;
    system.cpu.execute(5);
    updatePixels();
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

