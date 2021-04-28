/************************************************
*	Shutter tile
*************************************************/

#ifndef TLSHUT_H
#define TLSHUT_H

#include "TileView.h"
#include "Container.h"

#include <DropDown.h>

class TlShutter : public Container {
	DropDown	*_stair;	// Stairs selector
	DropDown	*_shutter;	// shutter selector

public:
	/* Shutter's tile constructor
	 * -> parent, cloned : see Container
	 */
	TlShutter( TileView *parent=NULL, TileView *cloned=NULL );

};

#endif
