/************************************************
*	GUI's root interface
*************************************************/
#ifndef GUI_H
#define GUI_H

#include "config.h"
#include <Arduino.h>
#include <lvgl/lvgl.h>

class Gui {
	lv_style_t style;
	lv_obj_t *background;

public:
	Gui( void );

	lv_style_t *getStyle( void ){
		return( &this->style );
	}
};

extern class Gui *gui;

	/***
	 * Objects used elsewhere
	 ***/
extern TTGOClass *ttgo;

#endif
