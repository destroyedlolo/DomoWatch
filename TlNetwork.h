/************************************************
*	Network's tile
*************************************************/

#ifndef TLNET_H
#define TLNET_H

#include "TileView.h"
#include "Container.h"

class TlNetwork : public Container {
public:
	/* Screen's tile constructor
	 * -> parent, cloned : see Container
	 */
	TlNetwork( TileView *parent, TileView *cloned=NULL );
};

#endif
