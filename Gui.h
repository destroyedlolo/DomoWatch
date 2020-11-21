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

class StatusBar;

class Gui : 
	public Style 	// default object style
{
	lv_obj_t	*_background;	// Background image
	StatusBar	*_statusbar;
	Container	*_workarea;		// container of the work area (all but the status bar)
	TileView	*_tileview;

	/* Tiles */
	TlDateTime	*_tile_datetime;
	TlStatus	*_tile_status;

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

		/* interfaces
		 *
		 * Documentation are in StatusBar.h
		 */

	void updateStepCounter( void );
	void updateBatteryIcon( lv_icon_battery_t index );
	void updateBatteryLevel( void );
};


	/***
	 * Objects used elsewhere
	 ***/

#define BARHEIGHT 30	// Status bar height

extern TTGOClass *ttgo;
extern class Gui *gui;

#endif
