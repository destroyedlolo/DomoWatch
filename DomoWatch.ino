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
#include <freertos/event_groups.h>

#include <esp_task_wdt.h>	// Watchdog
#include <driver/uart.h>		// uart_set_wakeup_threshold ...

#include <soc/rtc.h>	// RTC interface
#include <rom/rtc.h>	// RTC wakeup code

#include "Version.h"
#include "Gui.h"
#include "CommandLine.h"


	/****
	* Shared object
	*****/

TTGOClass *ttgo;
uint32_t inactive_counter = 30*1000;	// The watch is going to sleep if no GUI activities
bool mvtWakeup = true; // can wakeup from movement

	/*******
	* Signaling
	 *******/

	/* IRQ own codes are running in dedicated environment so
	 * are having reduced API and must run as fast as possible.
	 * Consequently, they are only positioning some flags and 
	 * actions are done in handler that are running  as normal
	 * tasks.
	 */

#define WATCH_IRQ_AXP	_BV(0)	// IRQ from the power management
#define WATCH_IRQ_BMA	_BV(1)	// IRQ from movements

EventGroupHandle_t irqs = NULL;


	/*********************
	* Initialization
	* Called when the watch is starting after power-up
	* but also when waked-up after deep-sleep
	**********************/

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

		/****
		* Handle the watch
		*****/

	ttgo = TTGOClass::getWatch();
	ttgo->begin();			// start peripherals

	// Turn off unused power
	ttgo->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
	ttgo->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
	ttgo->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
	ttgo->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);


		/****
		* start the GUI
		*****/

	Serial.println("Setting up the GUI ...");

	ttgo->lvgl_begin();		// start LVGL
	gui = new Gui();
	gui->updateBatteryLevel();	// Initial values
	gui->updateBatteryIcon( Gui::LV_ICON_UNKNOWN );
	lv_disp_trig_activity(NULL); // Clear lvgl activity counter

	ttgo->openBL(); // Everything done, turn on the backlight


		/****
		* restore the time
		*****/

	Serial.println("Reading RTC ...");
	ttgo->rtc->check();			// Ensure the RTC is valid (if not use compilation time)
	ttgo->rtc->syncToSystem();	// sync with ESP32


		/****
		* IRQ and events
		*****/

	Serial.println("Setting up interrupts ...");
	irqs = xEventGroupCreate();

	// power on interrupt
	pinMode(AXP202_INT, INPUT);
	attachInterrupt(AXP202_INT, [] {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(irqs, WATCH_IRQ_AXP, &xHigherPriorityTaskWoken);

		if(xHigherPriorityTaskWoken)
			portYIELD_FROM_ISR ();
	}, FALLING);

	ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
	ttgo->power->clearIRQ();


		/****
		* Completed
		*****/

	Serial.printf("Total heap: %d\n", ESP.getHeapSize());
    Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());

	Serial.println("Initialisation completed");
}


	/*******************
	*	loop() is mandatory when using Arduino IDE
	*	In RTOS, loop() is a task like another one with a priority 1
	*	
	*	In DomoWatch, loop() is handling :
	*		- CLI
	*		- lvgl related things (ALL related things as it seems
	*	lvgl is not currently multi-tasking aware).
	*
	*	CAUTION : as not event driven, it's important to
	*	avoid blocking or long standing activities in loop()
	********************/
void loop(){
	EventBits_t bits = xEventGroupWaitBits(
		irqs, 
		WATCH_IRQ_AXP,	// which even to wait for
		pdTRUE,							// clear them when got
		pdFALSE,						// no need to have all
		5 / portTICK_RATE_MS			// 5 ms then let loop to let recurrent tasks
	);

	if( bits & WATCH_IRQ_AXP ){
		ttgo->power->readIRQ();	// update library buffer

		if(ttgo->power->isPEKShortPressIRQ())
				Serial.println("PEK");

	    ttgo->power->clearIRQ();	// Free for other interrupt
		ttgo->power->readIRQ();	// DEBUG

		if( ttgo->bl->isOn() ){
			ttgo->closeBL();		// turn off back light
			ttgo->stopLvglTick();	// stop Lvgl
			ttgo->displaySleep();	// turn off touchscreen

			gpio_wakeup_enable( (gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL );	// IRQ to wakeup
			esp_sleep_enable_gpio_wakeup();

			Serial.println("ENTER IN LIGHT SLEEP MODE");
			setCpuFrequencyMhz(20);
			delay(500);	// let time to flush uart

			esp_light_sleep_start();	// dodo

			setCpuFrequencyMhz(160);
			Serial.println("Hello, I'm back");

			return;	// lets loop() doing another turn
		} else {
			Serial.println("wake up");

			ttgo->startLvglTick();
			ttgo->displayWakeup();
			ttgo->rtc->syncToSystem();

			gui->updateStepCounter();
			gui->updateBatteryLevel();
			gui->updateBatteryIcon( Gui::LV_ICON_UNKNOWN );

			lv_disp_trig_activity(NULL);
			ttgo->openBL();

			return;	// lets loop() doing another turn
		}
	}

	CommandLine::loop();	// Any command to handle ?

#if 0
	if(lv_disp_get_inactive_time(NULL) < inactive_counter)
		lv_task_handler();	// let Lvgl to handle it's own internals
	else {	// No activities : going to sleep
		Serial.println("No activity : Go to sleep");
	}
#endif
	lv_task_handler();	// let Lvgl to handle it's own internals
}

