/************************************************
*	GUI's root interface
*************************************************/

#ifndef GUI_H
#define GUI_H

#include <lvgl/lvgl.h>

class Gui {
	lv_style_t mainbar_style;
	lv_obj_t *mainbar;
	lv_obj_t *background;

public :
	enum lv_icon_battery_t {
		LV_ICON_BAT_EMPTY,
		LV_ICON_BAT_1,
		LV_ICON_BAT_2,
		LV_ICON_BAT_3,
		LV_ICON_BAT_FULL,
		LV_ICON_CHARGE,
		LV_ICON_CALCULATION
	};

	Gui();

	void updateStepCounter(uint32_t counter);
	void updateBatteryIcon(lv_icon_battery_t index);
	void updateBatteryLevel(void);
};

extern class Gui *gui;


/*
typedef enum {
	LV_STATUS_BAR_BATTERY_LEVEL = 0,
	LV_STATUS_BAR_BATTERY_ICON = 1,
	LV_STATUS_BAR_WIFI = 2,
	LV_STATUS_BAR_BLUETOOTH = 3,
} lv_icon_status_bar_t;
*/


	/***
	 * Objects used elsewhere
	 ***/

extern TTGOClass *ttgo;
extern EventGroupHandle_t g_event_group;
extern QueueHandle_t g_event_queue_handle;


/*
void wifi_list_add(const char *ssid);
void wifi_connect_status(bool result);
*/

#endif
