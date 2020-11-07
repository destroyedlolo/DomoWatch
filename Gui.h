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
#include "TlSettings.h"

class Gui : 
	virtual public Style 	// default object style
{
	lv_obj_t *_background;
	Container *_maincont;	// main controler
	TileView *_maintv;		// main tileview

	/* Tiles */
	TlDateTime *_tile_datetime;
	TlSettings *_tile_settings;

public:
	virtual lv_obj_t *getMyself( void ) { return NULL; }

	enum lv_icon_battery_t {
		LV_ICON_BAT_EMPTY,
		LV_ICON_BAT_1,
		LV_ICON_BAT_2,
		LV_ICON_BAT_3,
		LV_ICON_BAT_FULL,
		LV_ICON_CHARGE,			/* Plugged */
		LV_ICON_CALCULATION,	/* calculated against charging percentage */
		LV_ICON_UNKNOWN			/* check first of plugged */
	};

	Gui( void );

};


	/***
	 * Objects used elsewhere
	 ***/

#define BARHEIGHT 30	// Status bar height

extern TTGOClass *ttgo;
extern class Gui *gui;

#endif
