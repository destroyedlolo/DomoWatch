/************************************************
*	Settings tile
*************************************************/

#ifndef TLSET_H
#define TLSET_H

#include "TileView.h"
#include "Container.h"
#include "Label.h"

class TlSettings : public Container {
	Label		*_version;

public:
	/* Settings' tile constructor
	 * -> parent, cloned : see Container
	 */
	TlSettings( TileView *parent=NULL, TileView *cloned=NULL );
};

#endif
