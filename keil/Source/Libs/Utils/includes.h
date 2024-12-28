#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "adc_pm.h"
#include "button.h"
#include "dac.h"
#include "delay.h"
#include "glcd.h"
#include "joystick.h"
#include "led.h"
#include "power.h"
#include "rit.h"
#include "timer.h"
#include "touch.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // Needs to be visible in order to become visible by the simulator
#endif

extern void SystemInit();

#endif