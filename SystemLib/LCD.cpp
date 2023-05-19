#include <iostream>
#include "LCD.h"

LCD::LCD() {
    lcd = vrEmuLcdNew(LCD_WIDTH, LCD_HEIGHT, EmuLcdRomA00);
}

LCD::~LCD() {
    vrEmuLcdDestroy(lcd);
}

void LCD::sendCommand(uint8_t data) {
    vrEmuLcdSendCommand(lcd, data);
}

void LCD::writeByte(uint8_t data) {
    vrEmuLcdWriteByte(lcd, data);
}

void LCD::updatePixels() {
    vrEmuLcdUpdatePixels(lcd);
}

char LCD::pixelState(unsigned x, unsigned y) {
    return vrEmuLcdPixelState(lcd, x, y);
}

unsigned LCD::numPixelsX() {
    return (unsigned) vrEmuLcdNumPixelsX(lcd);
}

unsigned LCD::numPixelsY() {
    return (unsigned) vrEmuLcdNumPixelsY(lcd);
}

void LCD::portAWrite(byte data) {
    E = data & 0b10000000;
    RW = data & 0b01000000;
    RS = data & 0b00100000;
    if(E.toggled_on && !busy) {
        if(!RS && !RW) {
            sendCommand(data_lines);
        } else if(!RS) {
            data_lines = 0x00;
        } else if(!RW) {
            writeByte(data_lines);
        } else {
            //unknown behavior
        }
    } else if(E.toggled_on && busy) {
        if(!RS && RW) {
            data_lines = 0b10000000;
        }
    }
}

void LCD::portBWrite(byte data) {
    data_lines = data;
}

byte LCD::portBRead() const {
    return data_lines;
}