/************************************************
*		DomoWatch
*
*	Copyright 2020 Laurent Faillie (destroyedlolo)
*
*	Follow me on twitter : @DestroyedLolo
 ************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

// Please select the model you want to use in config.h
#include "config.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <freertos/queue.h>

#include <esp_task_wdt.h>	// Watchdog

#include <soc/rtc.h>	// RTC interface
#include <rom/rtc.h>	// RTC wakeup code

#include "Version.h"
#include "Gui.h"
#include "CommandLine.h"

#define G_EVENT_VBUS_PLUGIN         _BV(0)
#define G_EVENT_VBUS_REMOVE         _BV(1)
#define G_EVENT_CHARGE_DONE         _BV(2)

enum {
    Q_EVENT_BMA_INT,
    Q_EVENT_AXP_INT,
} ;

	/* Go to sleep after this time w/o any activities */
uint32_t inactive_counter = 30*1000;

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

void low_energy( void ){
/* Enter in low energy */

	xEventGroupSetBits(isr_group, WATCH_FLAG_SLEEP_MODE);
	ttgo->closeBL();
	ttgo->stopLvglTick();
	ttgo->bma->enableStepCountInterrupt(false);
	ttgo->displaySleep();

//	if(!WiFi.isConnected()){
		lenergy = true;
// 		WiFi.mode(WIFI_OFF);
		setCpuFrequencyMhz(20);

		Serial.println("ENTER IN LIGHT SLEEEP MODE");
		delay(500);
		gpio_wakeup_enable ((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
		gpio_wakeup_enable ((gpio_num_t)BMA423_INT1, GPIO_INTR_HIGH_LEVEL);
		esp_sleep_enable_gpio_wakeup ();
		esp_light_sleep_start();
//	}
}

/* wake up from low energy mode */
void wakeup( void ){
	ttgo->startLvglTick();
	ttgo->displayWakeup();
	ttgo->rtc->syncToSystem();

	gui->updateStepCounter();
	gui->updateBatteryLevel();
	gui->updateBatteryIcon( Gui::LV_ICON_UNKNOWN );

	lv_disp_trig_activity(NULL);
	ttgo->openBL();
	ttgo->bma->enableStepCountInterrupt();
}

void setup(){
	Serial.begin(115200);

	Serial.println("starting Domo watch v" VERSION_H);
	Serial.printf("Configure watchdog to 30s: %d\n", esp_task_wdt_init( 30, true ) );
	Serial.print("Wakeup raison : ");
	switch( rtc_get_reset_reason(0) ){
	case POWERON_RESET :
		Serial.println("Power on");
		break;
	case SW_RESET :
		Serial.println("Software");
		break;
	case OWDT_RESET :
		Serial.println("Legacy Watch dog");
		break;
	case DEEPSLEEP_RESET :
		Serial.println("Deep Sleep");
		break;
	case SDIO_RESET :
		Serial.println("SLC");
		break;
	case TG0WDT_SYS_RESET :
		Serial.println("Timer Group0 Watch dog");
		break;
	case TG1WDT_SYS_RESET :
		Serial.println("Timer Group1 Watch dog");
		break;
	case RTCWDT_SYS_RESET :
		Serial.println("RTC Watch dog digital");
		break;
	case INTRUSION_RESET :
		Serial.println("Instrusion tested to reset");
		break;
	case TGWDT_CPU_RESET :
		Serial.println("Time Group");
		break;
	case SW_CPU_RESET :
		Serial.println("Software");
		break;
	case RTCWDT_CPU_RESET :
		Serial.println("RTC Watch dog");
		break;
	case EXT_CPU_RESET :
		Serial.println("reseted by PRO CPU");
		break;
	case RTCWDT_BROWN_OUT_RESET :
		Serial.println("vdd voltage is not stable");
		break;
	case RTCWDT_RTC_RESET :
		Serial.println("RTC Watch dog reset digital core and rtc module");
		break;
	default :
		Serial.println("Who know ...");
	}

	/* Initialize time zone (France) */
	setenv("TZ", "GMT0BST,M3.5.0/01,M10.5.0/02",1);	// France
	tzset();

	/* Create a program that allows the required message objects 
	 * and group flags
	 */
	g_event_queue_handle = xQueueCreate(20, sizeof(uint8_t));
	g_event_group = xEventGroupCreate();
	isr_group = xEventGroupCreate();

	/* handling the watch */
	ttgo = TTGOClass::getWatch();	
	ttgo->begin();	// start peripherals

	/* starting with LVGL */
	ttgo->lvgl_begin();

	/* Power */
	Serial.println("Setting up interrupts ...");

	// Turn on the IRQ used
	ttgo->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
	ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
	ttgo->power->clearIRQ();

	// Turn off unused power
	ttgo->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
	ttgo->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
	ttgo->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
	ttgo->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);

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

	
	/* Initialise clock */
	Serial.println("Reading RTC ...");
	ttgo->rtc->check();	// Check if the clock is valid or fallback to compilation time
	ttgo->rtc->syncToSystem(); //Synchronize time to system time

	{	// Debugging
		time_t now;
		struct tm  info;
		char buf[64];

		time(&now);
		localtime_r(&now, &info);
		strftime(buf, sizeof(buf), "%H:%M:%S", &info);
		Serial.print("Setting clock to ");
		Serial.println(buf);
	}

	/* Execute our own GUI interface */

	Serial.println("Setting up the GUI ...");
	gui = new Gui();

	gui->updateBatteryLevel();	// Initial values
	gui->updateBatteryIcon( Gui::LV_ICON_UNKNOWN );

	lv_disp_trig_activity(NULL); // Clear lvgl activity counter

	ttgo->openBL(); // Everything done, turn on the backlight

	Serial.printf("Total heap: %d\n", ESP.getHeapSize());
    Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());

	Serial.println("Initialisation completed");
}

void loop(){
	bool  rlst;
	uint8_t data;

	/* Test about wake up irq */
	EventBits_t  bits = xEventGroupGetBits(isr_group);
	if(bits & WATCH_FLAG_SLEEP_EXIT){
		Serial.println("Waking up");
		if(lenergy){
			Serial.println("from low energy mode.");
			lenergy = false;
			setCpuFrequencyMhz(160);
		}

		wakeup();

		if(bits & WATCH_FLAG_BMA_IRQ){
			Serial.println("IRQ BMA");
			do {
				rlst =  ttgo->bma->readInterrupt();
			} while(!rlst);
			xEventGroupClearBits(isr_group, WATCH_FLAG_BMA_IRQ);
		}
		if(bits & WATCH_FLAG_AXP_IRQ){
			Serial.println("IRQ AXP");
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

	/* other activities */
	if(xQueueReceive(g_event_queue_handle, &data, 5 / portTICK_RATE_MS) == pdPASS){
		switch(data){
		case Q_EVENT_BMA_INT: // step counter
			do
				rlst =  ttgo->bma->readInterrupt();
			while(!rlst);

			if(ttgo->bma->isStepCounter())
				gui->updateStepCounter();
			break;

		case Q_EVENT_AXP_INT:
				// Power management
			ttgo->power->readIRQ();

			if(ttgo->power->isVbusPlugInIRQ())
				gui->updateBatteryIcon( Gui::LV_ICON_CHARGE );
	
			if(ttgo->power->isVbusRemoveIRQ())
				gui->updateBatteryIcon( Gui::LV_ICON_CALCULATION );

			if(ttgo->power->isChargingDoneIRQ())
				gui->updateBatteryIcon( Gui::LV_ICON_CALCULATION );

				// Button push
			if(ttgo->power->isPEKShortPressIRQ()){
				Serial.println("Button pressed");
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

	CommandLine::loop();

	if(lv_disp_get_inactive_time(NULL) < inactive_counter)
		lv_task_handler();
	else {	// No activities : going to sleep
		Serial.println("No activity : Go to sleep");
		low_energy();
	}
}
