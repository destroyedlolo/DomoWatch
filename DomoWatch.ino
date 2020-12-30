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

/* Enable DEEPSLEEP while doing long click
 * unfortunately, at wakeup, the touch screen doesn't work anymore.
 * Consequentely, it's disabled for the moment.
 */
// #define WITH_DEEPSLEEP

	/****
	* Shared object
	*****/

TTGOClass *ttgo;
uint32_t inactive_counter = 30*1000;	// The watch is going to sleep if no GUI activities
bool mvtWakeup = true; // can wakeup from movement

	/* We must call wakeup() at the end of light_sleep() take in account all case of falling asleep.
	 * But, as an AXP interrupt is wised at wake up, this flag will avoid a dead loop making the watch
	 * thinking we want to sleep again as the back light is already on.
	 */
bool wakingup = true;

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
	*	Sleeping
	**********************/

void wakeup(){
	Serial.println("wake up");

	ttgo->startLvglTick();
	ttgo->displayWakeup();
	ttgo->rtc->syncToSystem();

	gui->updateStepCounter();
//	gui->updateBatteryLevel();
//	gui->updateBatteryIcon( Gui::LV_ICON_UNKNOWN );

	lv_disp_trig_activity(NULL);
	ttgo->openBL();
	ttgo->bma->enableStepCountInterrupt();	// Restore step counter follow-up
}

void deep_sleep(){
	ttgo->stopLvglTick();
	ttgo->displayOff();
	ttgo->bma->enableStepCountInterrupt(false);	// Step counter will not generate interrupt
	ttgo->powerOff();

	esp_sleep_enable_ext0_wakeup((gpio_num_t)AXP202_INT, LOW);	// IRQ to wakeup
	
	Serial.println("ENTER IN DEEP SLEEP MODE");
	esp_deep_sleep_start();
}

void light_sleep(){
	ttgo->closeBL();		// turn off back light
	ttgo->stopLvglTick();	// stop Lvgl
	ttgo->displaySleep();	// turn off touchscreen
	ttgo->bma->enableStepCountInterrupt(false);	// Step counter will not generate interrupt

	/* We are obliged to use different "ext?" as AXP interruption need a low level
	 * whereas it's hight one for the BMA.
	 */
	esp_sleep_enable_ext0_wakeup((gpio_num_t)AXP202_INT, LOW);	// IRQ to wakeup
	if(mvtWakeup){
		esp_sleep_enable_ext1_wakeup(GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
		Serial.println("BMA allowed");
	} else {
		esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_EXT1);
		Serial.println("BMA disabled");
	}
	esp_sleep_enable_gpio_wakeup();

	Serial.println("ENTER IN LIGHT SLEEP MODE");
	setCpuFrequencyMhz(20);
	delay(500);	// let time to flush uart

	esp_light_sleep_start();	// dodo

	setCpuFrequencyMhz(160);
	Serial.print("Hello, I'm back due to ");
	switch( esp_sleep_get_wakeup_cause() ){
	case ESP_SLEEP_WAKEUP_UNDEFINED :
		Serial.println("Undefined");
		break;
	case ESP_SLEEP_WAKEUP_EXT0 :
		Serial.println("EXT0");
		break;
	case ESP_SLEEP_WAKEUP_EXT1 :
		Serial.println("EXT1");
		break;
	case ESP_SLEEP_WAKEUP_TIMER :
		Serial.println("TIMER");
		break;
	case ESP_SLEEP_WAKEUP_TOUCHPAD :
		Serial.println("TOUCHPAD");
		break;
	case ESP_SLEEP_WAKEUP_ULP :
		Serial.println("ULP");
		break;
	case ESP_SLEEP_WAKEUP_GPIO :
		Serial.println("GPIO");
		break;
	case ESP_SLEEP_WAKEUP_UART :
		Serial.println("UART");
		break;
/* Not defined in arduino IDE
	case ESP_SLEEP_WAKEUP_WIFI :
		Serial.println("WIFI");
		break;
	case ESP_SLEEP_WAKEUP_COCPU :
		Serial.println("COCPU");
		break;
	case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG :
		Serial.println("COCPU crash");
		break;
*/
	default :
		Serial.println("????");
	}
	wakeup();
	wakingup = true;
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
		* restore the time
		*****/

	Serial.println("Reading RTC ...");
	ttgo->rtc->check();			// Ensure the RTC is valid (if not use compilation time)
	ttgo->rtc->syncToSystem();	// sync with ESP32

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

	// Enable BMA423 interrupt 
	// The default interrupt configuration,
	// you need to set the acceleration parameters, please refer to the BMA423_Accel example
	ttgo->bma->attachInterrupt();

	//Connection interrupted to the specified pin
	pinMode(BMA423_INT1, INPUT);
	attachInterrupt(BMA423_INT1, [] {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(irqs, WATCH_IRQ_BMA, &xHigherPriorityTaskWoken);

		if(xHigherPriorityTaskWoken)
			portYIELD_FROM_ISR ();
	}, RISING);


		/****
		* Completed
		*****/

	Serial.printf("Total heap: %d\n", ESP.getHeapSize());
    Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());

	Serial.printf("\nLong PRESS : %d in 10thS\n", ttgo->power->getlongPressTime());
	Serial.printf("Startup PRESS : %d in 10thS\n", ttgo->power->getStartupTime());
	Serial.printf("Shutdown PRESS : %d in 10thS\n", ttgo->power->getShutdownTime());

	Serial.println("\nInitialisation completed");
}


	/*******************n
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
	EventBits_t bits = xEventGroupWaitBits(	// Waiting for event
		irqs, 
		WATCH_IRQ_AXP | WATCH_IRQ_BMA,	// which even to wait for
		pdTRUE,								// clear them when got
		pdFALSE,							// no need to have all
		5 / portTICK_RATE_MS				// 5 ms then let loop to let recurrent tasks
	);

	if( bits & WATCH_IRQ_AXP ){
		ttgo->power->readIRQ();	// update library buffer

		bool longPEK;

		if( ((longPEK = ttgo->power->isPEKLongtPressIRQ()) || ttgo->power->isPEKShortPressIRQ()) && 
		  !wakingup ){	// want to shutdown
		    ttgo->power->clearIRQ();	// Free for other interrupt
#if defined(WITH_DEEPSLEEP)
			if( longPEK )
				deep_sleep();
			else
#endif
				light_sleep();
			return;
		} else {	// Probably sharing IRQ
		    ttgo->power->clearIRQ();	// Free for other interrupt
			gui->updateBatteryLevel();
			gui->updateBatteryIcon( Gui::LV_ICON_UNKNOWN );

			if( !ttgo->bl->isOn() )
				wakeup();
		}
	}

	if( bits & WATCH_IRQ_BMA ){
		bool  rlst;
		do {
			rlst =  ttgo->bma->readInterrupt();
		} while(!rlst);

		if(ttgo->bma->isStepCounter())
			gui->updateStepCounter();
	}

	wakingup = false;
	CommandLine::loop();	// Any command to handle ?

	if(lv_disp_get_inactive_time(NULL) < inactive_counter)
		lv_task_handler();	// let Lvgl to handle it's own internals
	else {	// No activities : going to sleep
		Serial.println("No activity : Go to sleep");
		light_sleep();
	}
}

