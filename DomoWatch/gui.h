/* GUI definition
 *
 * part of this code comes from SimpleWatch example
 */

#ifndef __GUI_H
#define __GUI_H

typedef enum {
    LV_ICON_BAT_EMPTY,
    LV_ICON_BAT_1,
    LV_ICON_BAT_2,
    LV_ICON_BAT_3,
    LV_ICON_BAT_FULL,
    LV_ICON_CHARGE,
    LV_ICON_CALCULATION
} lv_icon_battery_t;


typedef enum {
    LV_STATUS_BAR_BATTERY_LEVEL = 0,
    LV_STATUS_BAR_BATTERY_ICON = 1,
    LV_STATUS_BAR_WIFI = 2,
    LV_STATUS_BAR_BLUETOOTH = 3,
} lv_icon_status_bar_t;

extern EventGroupHandle_t g_event_group;
extern QueueHandle_t g_event_queue_handle;

void setupGui();
void updateStepCounter(uint32_t counter);
void updateBatteryIcon(lv_icon_battery_t index);

void wifi_connect_status(bool result);
void updateBatteryLevel();

#endif /*__GUI_H */

