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


	/*****
	* flags shared among tasks/IRQ
	* so arbitration is needed
	 *****/

EventGroupHandle_t flags = NULL;

#define WATCH_IRQ_AXP	_BV(0)	// IRQ from the power management
#define WATCH_IRQ_BMA	_BV(1)	// IRQ from movements
#define	WATCH_WACKUP	_BV(2)	// wakeup

EventGroupHandle_t watch_mode = NULL;

#define WATCH_MODE_SLEEP _BV(0)	// We're in sleep mode


	/****
	* Shared object
	*****/

TTGOClass *ttgo;
uint32_t inactive_counter = 30*1000;	// The watch is going to sleep if no GUI activities
bool mvtWakeup = true; // can wakeup from movement


	/****
	* Sleep and wakeup
	 ****/

void low_energy( void ){
	ttgo->closeBL();		// turn off back light
	ttgo->stopLvglTick();	// stop Lvgt
	ttgo->displaySleep();	// turn off screen
}

void start_everything( void ){
	ttgo->startLvglTick();	// start lvgl
	ttgo->displayWakeup();	// start screen
	ttgo->rtc->syncToSystem();

	gui->updateStepCounter();
	gui->updateBatteryLevel();
	gui->updateBatteryIcon( Gui::LV_ICON_UNKNOWN );

	lv_disp_trig_activity(NULL);
	ttgo->openBL();

}

void light_sleep(){
	low_energy();

	Serial.println("ENTER IN LIGHT SLEEEP MODE");

	xEventGroupSetBits( watch_mode, WATCH_MODE_SLEEP );	// Notifying we want to sleep

	gpio_wakeup_enable( (gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL );
	esp_sleep_enable_gpio_wakeup();		// work only in light sleep mode

#if 0
	uart_set_wakeup_threshold(UART_NUM_0,3);
	esp_sleep_enable_uart_wakeup(UART_NUM_0);	// work only in light sleep mode
	Serial.println("Type 'a' to wake-up");
#endif

	setCpuFrequencyMhz(20);	// Slow down processor

	delay(500);	// let some time to flush serial

	esp_light_sleep_start();	// Go to sleep

	Serial.println("Waked ...");

	setCpuFrequencyMhz(160);
	xEventGroupSetBits( flags, WATCH_WACKUP );
}


	/****
	* IRQ from PEK / AXP
	 ****/
TaskHandle_t powerTask = NULL;
TaskHandle_t wakeupTask = NULL;

void handlePowerIRQ( void *pvParameters ){
	for(;;){
		EventBits_t bits = xEventGroupWaitBits(
			flags, 
			WATCH_IRQ_AXP | WATCH_IRQ_BMA,	// which even to wait for
			pdTRUE,							// clear them when got
			pdFALSE,						// no need to have all
			portMAX_DELAY					// wait forever
		);

		Serial.printf("bip %x\n", bits);

		EventBits_t bmode = xEventGroupGetBits( watch_mode );

		if( bits & WATCH_IRQ_AXP ){
			ttgo->power->readIRQ();
			ttgo->power->clearIRQ();	// Reset IRQ
			if( !(bmode & WATCH_MODE_SLEEP) )	// we are not sleeping
				light_sleep();
			else
				Serial.println("Sleeping");
		}
	}
}

void handleWakeup( void *pvParameters ){
	for(;;){
		xEventGroupWaitBits(
			flags, 
			WATCH_WACKUP,	// which even to wait for
			pdTRUE,			// clear them when got
			pdFALSE,		// no need to have all
			portMAX_DELAY	// wait forever
		);
		
		start_everything();

Serial.println("wakeup");
		xEventGroupClearBits( watch_mode, WATCH_MODE_SLEEP );
	}
}

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
	flags = xEventGroupCreate();
	watch_mode = xEventGroupCreate();

	// power on interrupt
	pinMode(AXP202_INT, INPUT);
	attachInterrupt(AXP202_INT, [] {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(flags, WATCH_IRQ_AXP, &xHigherPriorityTaskWoken);

		if(xHigherPriorityTaskWoken)
			portYIELD_FROM_ISR ();
	}, FALLING);

	ttgo->power->clearIRQ();

	if( pdPASS != xTaskCreate( 
		handlePowerIRQ,				// function to call
		"Power", 					// name
		/* configMINIMAL_STACK_SIZE */ 65535,	// stack size (default)
		NULL,						// Argument
		10,							// priority
		&powerTask					// Task's handle
	) )
		Serial.println("Power task creation failed");

	if( pdPASS != xTaskCreate( 
		handleWakeup,				// function to call
		"wakeup", 					// name
		/* configMINIMAL_STACK_SIZE */ 65535,	// stack size (default)
		NULL,						// Argument
		10,							// priority
		&wakeupTask					// Task's handle
	) )
		Serial.println("Wakeup task creation failed");


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
	*		- lvgl timeout
	*
	*	CAUTION : as not event driven, it's important to
	*	avoid blocking or long standing activities in loop()
	********************/
void loop(){
	CommandLine::loop();	// Any command to handle ?

	if(lv_disp_get_inactive_time(NULL) < inactive_counter)
		lv_task_handler();	// let Lvgl to handle it's own internals
	else {	// No activities : going to sleep
		Serial.println("No activity : Go to sleep");
		light_sleep();
	}

	delay( 5 );
}

