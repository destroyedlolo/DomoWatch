/************************************************
*	GUI's root interface
*************************************************/
#ifndef GUI_H
#define GUI_H

#include "config.h"
#include <Arduino.h>
#include <lvgl/lvgl.h>

#include "Style.h"

class Gui : 
	virtual public Style 	// default object style
{
	lv_obj_t *background;

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
