/************************************************
*	GUI's tile
*************************************************/

#ifndef Tile_H
#define Tile_H

#include "config.h"
#include <Arduino.h>

#include "GfxObject.h"
#include "Gui.h"

class Tile : virtual public GfxObject {
	lv_obj_t *tile;

public:
	Tile(Gui *);

	lv_obj_t *getMyself( void ){ return this->tile; }
};

#endif
