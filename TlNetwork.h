/************************************************
*	Network's tile
*************************************************/

#ifndef TLNET_H
#define TLNET_H

#include <TileView.h>
#include <Container.h>
#include <Image.h>
#include <Button.h>

class TlNetwork : public Container {
	time_t last;

	/* time synchronisation */
	Image		*syncIcon;

	/* MQTT */
	Image		*MQTTIcon;

	/* Temperatures */
	Container	*salonCont;
	Image		*salonIcon;
	Label 		*salonText;

	Container	*jardinCont;
	Image		*jardinIcon;
	Label 		*jardinText;

	Container	*congeloCont;
	Image		*congeloIcon;
	Label 		*congeloText;

	/* Energy */
	Container	*NRJCont;
	Image		*consoIcon;
	Label 		*consoText;
	Image		*prodIcon;
	Label 		*prodText;


public:
	/* Network's tile constructor
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

	/* Clear obsoleted value if not accurate anymore
	 * (too old)
	 */
	void clearObsoletedValues( void );
};

#endif
