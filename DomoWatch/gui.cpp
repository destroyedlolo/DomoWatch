/************************************************
*	GUI's root definition
*************************************************/
#include "config.h"
#include <Arduino.h>

#include "gui.h"
#include "myfont.h"

	/* "standard" font */
LV_FONT_DECLARE(Ubuntu);
LV_FONT_DECLARE(Geometr);

	/* Wallpaper */
LV_IMG_DECLARE(bg);
LV_IMG_DECLARE(bg1);
LV_IMG_DECLARE(bg2);
LV_IMG_DECLARE(bg3);
LV_IMG_DECLARE(WALLPAPER_1_IMG);
LV_IMG_DECLARE(WALLPAPER_2_IMG);
LV_IMG_DECLARE(WALLPAPER_3_IMG);

	/* objects */
static lv_obj_t *mainBar = nullptr;
class Gui *gui;

	/* Build the GUI */
Gui::Gui(){
}

	/* Interfaces */
void Gui::updateStepCounter(uint32_t counter){
}

void Gui::updateBatteryIcon(lv_icon_battery_t index){
}

void Gui::updateBatteryLevel(void){
}
