/************************************************
*	Network's tile
*************************************************/

#ifndef TLNET_H
#define TLNET_H

#include "TileView.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"

class TlNetwork : public Container {
	/* time synchronisation */
	Button		*syncButton;
	Image		*syncIcon;

	/* MQTT */
	Button		*MQTTButton;
	Image		*MQTTIcon;


public:
	/* Screen's tile constructor
	 * -> parent, cloned : see Container
	 */
	TlNetwork( TileView *parent, TileView *cloned=NULL );
};

#endif
