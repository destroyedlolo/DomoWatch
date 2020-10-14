/************************************************
*	GUI's tile
*************************************************/

#ifndef Tile_H
#define Tile_H

#include "config.h"
#include <Arduino.h>

#include <lvgl/lvgl.h>

class Tile {
	lv_obj_t *tile;

public:
	Tile(lv_obj_t *);

	lv_obj_t *getTile( void ){ return this->tile; }
};

#endif
