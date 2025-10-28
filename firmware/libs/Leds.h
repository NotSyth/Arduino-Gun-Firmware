#pragma once
#include <Arduino.h>

void LedsInit(int pin,int count);
void LedsSetRGB(uint8_t r,uint8_t g,uint8_t b);
void LedsFlash(uint16_t ms);
void LedsPulse(uint16_t onMs,uint16_t offMs,uint8_t r,uint8_t g,uint8_t b);
void LedsStopPulse();
void LedsOff();
void LedsUpdate();