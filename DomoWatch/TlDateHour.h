/************************************************
*	Main tile : Hour and date
*************************************************/

#ifndef DTHR_H
#define DTHR_H

#include "Gui.h"
#include "Tile.h"

class TlDateHour : public Tile {
	Gui &gui;

public:
	TlDateHour(lv_obj_t *, Gui &);
};


#endif
