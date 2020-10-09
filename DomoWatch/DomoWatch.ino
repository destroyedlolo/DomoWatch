/*
Copyright (c) 2019 lewis he
This is just a demonstration. Most of the functions are not implemented.
The main implementation is low-power standby.
The off-screen standby (not deep sleep) current is about 4mA.
Select standard motherboard and standard backplane for testing.
Created by Lewis he on October 10, 2019.
*/

// Please select the model you want to use in config.h
#include "config.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include <soc/rtc.h>
#include "esp_wifi.h"
#include "esp_sleep.h"
#include "gui.h"

#define G_EVENT_VBUS_PLUGIN         _BV(0)
#define G_EVENT_VBUS_REMOVE         _BV(1)
#define G_EVENT_CHARGE_DONE         _BV(2)

enum {
    Q_EVENT_BMA_INT,
    Q_EVENT_AXP_INT,
} ;

#define DEFAULT_SCREEN_TIMEOUT  30*1000

#define WATCH_FLAG_SLEEP_MODE   _BV(1)
#define WATCH_FLAG_SLEEP_EXIT   _BV(2)
#define WATCH_FLAG_BMA_IRQ      _BV(3)
#define WATCH_FLAG_AXP_IRQ      _BV(4)

QueueHandle_t g_event_queue_handle = NULL;
EventGroupHandle_t g_event_group = NULL;
EventGroupHandle_t isr_group = NULL;
bool lenergy = false;
TTGOClass *ttgo;


/*****
 * Handle sleep
 *****/

void low_energy(){
/* Enter in low energy */

	xEventGroupSetBits(isr_group, WATCH_FLAG_SLEEP_MODE);
	ttgo->closeBL();
	ttgo->stopLvglTick();
	ttgo->bma->enableStepCountInterrupt(false);
	ttgo->displaySleep();

#if 0		/* pas logic */
		if(!WiFi.isConnected()){
			lenergy = true;
			WiFi.mode(WIFI_OFF);
			// rtc_clk_cpu_freq_set(RTC_CPU_FREQ_2M);
			setCpuFrequencyMhz(20);

			Serial.println("ENTER IN LIGHT SLEEEP MODE");
			gpio_wakeup_enable ((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
			gpio_wakeup_enable ((gpio_num_t)BMA423_INT1, GPIO_INTR_HIGH_LEVEL);
			esp_sleep_enable_gpio_wakeup ();
			esp_light_sleep_start();
		}
#endif
}

void wakeup(){
/* wake up from low energy mode */

	ttgo->startLvglTick();
	ttgo->displayWakeup();
	ttgo->rtc->syncToSystem();
	updateStepCounter(ttgo->bma->getCounter());
	updateBatteryLevel();
	updateBatteryIcon(LV_ICON_CALCULATION);
	lv_disp_trig_activity(NULL);
	ttgo->openBL();
	ttgo->bma->enableStepCountInterrupt();
}

void setup(){
	Serial.begin(1152000);

	//Create a program that allows the required message objects and group flags
	g_event_queue_handle = xQueueCreate(20, sizeof(uint8_t));
	g_event_group = xEventGroupCreate();
	isr_group = xEventGroupCreate();

	//Initialize TWatch
	ttgo = TTGOClass::getWatch();	
	ttgo->begin();

	// Turn on the IRQ used
	ttgo->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
	ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
	ttgo->power->clearIRQ();

	// Turn off unused power
	ttgo->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
	ttgo->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
	ttgo->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
	ttgo->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);

	//Initialize lvgl
	ttgo->lvgl_begin();

	// Enable BMA423 interrupt 
	// The default interrupt configuration,
	// you need to set the acceleration parameters, please refer to the BMA423_Accel example
	ttgo->bma->attachInterrupt();

	//Connection interrupted to the specified pin
	pinMode(BMA423_INT1, INPUT);
	attachInterrupt(BMA423_INT1, [] {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		EventBits_t  bits = xEventGroupGetBitsFromISR(isr_group);
		if(bits & WATCH_FLAG_SLEEP_MODE){ //! For quick wake up, use the group flag
			xEventGroupSetBitsFromISR(isr_group, WATCH_FLAG_SLEEP_EXIT | WATCH_FLAG_BMA_IRQ, &xHigherPriorityTaskWoken);
		} else {
			uint8_t data = Q_EVENT_BMA_INT;
			xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
		}

		if(xHigherPriorityTaskWoken){
			portYIELD_FROM_ISR();
		}
	}, RISING);

	// Connection interrupted to the specified pin
	pinMode(AXP202_INT, INPUT);
	attachInterrupt(AXP202_INT, [] {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		EventBits_t  bits = xEventGroupGetBitsFromISR(isr_group);
		if(bits & WATCH_FLAG_SLEEP_MODE){
			//! For quick wake up, use the group flag
			xEventGroupSetBitsFromISR(isr_group, WATCH_FLAG_SLEEP_EXIT | WATCH_FLAG_AXP_IRQ, &xHigherPriorityTaskWoken);
		} else {
			uint8_t data = Q_EVENT_AXP_INT;
			xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
		}

		if(xHigherPriorityTaskWoken){
			portYIELD_FROM_ISR ();
		}
	}, FALLING);

	//Check if the RTC clock matches, if not, use compile time
	ttgo->rtc->check();

	//Synchronize time to system time
	ttgo->rtc->syncToSystem();

	//Execute your own GUI interface
	setupGui();

	//Clear lvgl counter
	lv_disp_trig_activity(NULL);

	//When the initialization is complete, turn on the backlight
	ttgo->openBL();
}

void loop(){
	bool  rlst;
	uint8_t data;

	//! Fast response wake-up interrupt
	EventBits_t  bits = xEventGroupGetBits(isr_group);
	if(bits & WATCH_FLAG_SLEEP_EXIT){
		if(lenergy){
			lenergy = false;
			setCpuFrequencyMhz(160);
		}

		wakeup();

		if(bits & WATCH_FLAG_BMA_IRQ){
			do {
				rlst =  ttgo->bma->readInterrupt();
			} while(!rlst);
			xEventGroupClearBits(isr_group, WATCH_FLAG_BMA_IRQ);
		}
		if(bits & WATCH_FLAG_AXP_IRQ){
			ttgo->power->readIRQ();
			ttgo->power->clearIRQ();
				//TODO: Only accept axp power pek key short press
			xEventGroupClearBits(isr_group, WATCH_FLAG_AXP_IRQ);
		}
		xEventGroupClearBits(isr_group, WATCH_FLAG_SLEEP_EXIT);
		xEventGroupClearBits(isr_group, WATCH_FLAG_SLEEP_MODE);
	}

	if((bits & WATCH_FLAG_SLEEP_MODE)){
		//! No event processing after entering the information screen
		return;
	}

	//! Normal polling
	if(xQueueReceive(g_event_queue_handle, &data, 5 / portTICK_RATE_MS) == pdPASS){
		switch(data){
		case Q_EVENT_BMA_INT:
			do {
				rlst =  ttgo->bma->readInterrupt();
			} while(!rlst);

			//! step counter
			if(ttgo->bma->isStepCounter()){
				updateStepCounter(ttgo->bma->getCounter());
			}
			break;

		case Q_EVENT_AXP_INT:
			ttgo->power->readIRQ();

			if(ttgo->power->isVbusPlugInIRQ())
				updateBatteryIcon(LV_ICON_CHARGE);
	
			if(ttgo->power->isVbusRemoveIRQ())
				updateBatteryIcon(LV_ICON_CALCULATION);

			if(ttgo->power->isChargingDoneIRQ())
				updateBatteryIcon(LV_ICON_CALCULATION);

			if(ttgo->power->isPEKShortPressIRQ()){
				ttgo->power->clearIRQ();
				if(ttgo->bl->isOn())
					low_energy();
				else
					wakeup();
				return;
			}

			ttgo->power->clearIRQ();
			break;

		}
	}

	if(lv_disp_get_inactive_time(NULL) < DEFAULT_SCREEN_TIMEOUT)
		lv_task_handler();
	else	// No activities : going to sleep
		low_energy();
}
