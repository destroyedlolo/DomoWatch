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

#include "config.h"
#include <TTGO.h>

#include "hardware/display.h"
#include "gui/splashscreen.h"

	/*	My own setting for my automation.
	 *	Has to define
	 *		Wifi :
	 *		------
	 *			WIFI_SSID
	 *			WIFI_PASSWORD
	 *
	 *		Alternative WiFi network
	 *		------------------------
	 *		(this one provides connectivities in my garden)
	 *			DOMO_SSID
	 *			DOMO_PASSWORD
	 *
	 *		Broker point of contact :
	 *		-------------------------
	 *			BROKER_HOST	- IP address of your broker
	 *			BROKER_PORT
	 *
	 */
// #include <Maison.h>

TTGOClass *ttgo = TTGOClass::getWatch();

void setup(){
	Serial.begin(2000000);

    ttgo->begin();
    ttgo->lvgl_begin();
		
		// force to store all new heap allocations in psram to get more internal ram
    heap_caps_malloc_extmem_enable( 1 );
    display_setup();
}

void loop(){
}

