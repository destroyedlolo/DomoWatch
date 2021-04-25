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

	/* Consumption */
	Container	*NRJCont;
	Image		*consoIcon;
	Label 		*consoText;
	Image		*prodIcon;
	Label 		*prodText;

public:
	/* Screen's tile constructor
	 * -> parent, cloned : see Container
	 */
	TlNetwork( TileView *parent, TileView *cloned=NULL );

	/* Subscribe to topics used but this tile */
	void subscribe( void );

	/* MQTT message received
	 * -> const char *topic
	 * -> const char *payload
	 * <- is the message accepted. If not, the message may
	 * 	be passed to further handlers
	 */
	bool msgreceived( const char *topic, const char *payload );
};

#endif
