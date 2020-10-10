/************************************************
*	GUI's root definition
*************************************************/
#include "config.h"
#include <Arduino.h>

#include "gui.h"
#include "myfont.h"

	/* "standard" font */
LV_FONT_DECLARE(Ubuntu);


	/* Build the GUI */
void setupGui(void){
}

	/* Interfaces */
void updateStepCounter(uint32_t counter){
}

void updateBatteryIcon(lv_icon_battery_t index){
}

void updateBatteryLevel(void){
}
