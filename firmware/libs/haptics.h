#pragma once
#include <Arduino.h>

#define HAPTIC_TAP 1
#define HAPTIC_DOUBLE 2
#define HAPTIC_HEAVY 3

void HapticsInit(int pin);
void HapticsStart(int type);
void HapticsUpdate();
void HapticsStop();