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

class Gui : 
	virtual public Style 	// default object style
{
	lv_obj_t *_background;
	Container *_maincont;	// main controler
	TileView *_maintv;		// main tileview

	/* Tiles */
	TlDateTime *_tile_datetime;

public:
	virtual lv_obj_t *getMyself( void ) { return NULL; }

	Gui( void );

};


	/***
	 * Objects used elsewhere
	 ***/

#define BARHEIGHT 30	// Status bar height

extern TTGOClass *ttgo;
extern class Gui *gui;

#endif
