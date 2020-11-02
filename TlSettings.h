/************************************************
*	Settings' tile
*************************************************/

#ifndef TLSET_H
#define TLSET_H

#include "Gui.h"
#include "Tile.h"
#include "Label.h"

class TlSettings : public Tile {

	Label	*version;

public:
	/* Settings tile's constructor
	 * -> Gui * : the gui main class
	 */
	TlSettings( Gui * );
};

#endif
