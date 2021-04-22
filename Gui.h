/************************************************
*	GUI's root interface
*************************************************/
#ifndef GUI_H
#define GUI_H

#include "config.h"
#include <Arduino.h>
#include <lvgl/lvgl.h>

#include "Style.h"
#include "Container.h"
#include "TileView.h"
#include "TlDateTime.h"
#include "TlStatus.h"
#include "TlScreen.h"
#include "TlNetwork.h"

class StatusBar;

class Gui : 
	public Style 	// default object style
{
	lv_obj_t	*_background;	// Background image
	StatusBar	*_statusbar;
	Container	*_workarea;		// container of the work area (all but the status bar)
	TileView	*_tileview;

	/* Tiles */
	TlNetwork	*_tile_network;
	TlDateTime	*_tile_datetime;
	TlStatus	*_tile_status;
	TlScreen	*_tile_screen;

public:


		/* Battery icons */
	enum lv_icon_battery_t {
			/* Icons ID */
		LV_ICON_BAT_EMPTY,
		LV_ICON_BAT_1,
		LV_ICON_BAT_2,
		LV_ICON_BAT_3,
		LV_ICON_BAT_FULL,
		LV_ICON_CHARGE,			// Plugged

			/* Fake id : some calculation needed */
		LV_ICON_CALCULATION,	// calculated against charging percentage
		LV_ICON_UNKNOWN			// check first of plugged
	};

	/* Build the GUI
	 */
	Gui( void );

	/* Launch automation tasks.
	 * Has to be called ONLY when everything is initialised
	 */
	void initAutomation( void );

		/* Update icons.
		 *
		 * Documentation are in StatusBar.h
		 */
	void updateStepCounter( void );
	void updateBatteryIcon( lv_icon_battery_t index );
	void updateBatteryLevel( void );

	void updateNetwork( void );


		/* GUI movement
		 *
		 * Modules can add other tiles and enable additional movements.
		 * This function updates allowed movements as per activated extensions
		 */
	void updateMovements( void );
	void backToHome( void );
};


	/***
	 * Objects used elsewhere
	 ***/

extern uint32_t inactive_counter;
extern uint32_t inactive_wifi_counter;

#define BARHEIGHT 30	// Status bar height

extern TTGOClass *ttgo;
extern class Gui *gui;

extern bool mvtWakeup;
extern uint8_t bl_lev;

#endif
