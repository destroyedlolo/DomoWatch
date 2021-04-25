/************************************************
*	Network's tile
*************************************************/

#include "Gui.h"
#include "Network.h"
#include "TlNetwork.h"

LV_IMG_DECLARE(timezone_64px);
LV_IMG_DECLARE(MQTT_64px);

	/****
	 * callbacks
	 ***/


/* Request time synchronisation.
 *
 * It seems, NTP query is done in a background tasks.
 * The added value, it doesn't block the GUI while querying.
 * The bad side is I can't set the RTC immediately : it will be done
 * at shutdonw.
 */
static void syncTime( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.println("Time synchronisation requested");

		configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", "pool.ntp.org");

		gui->backToHome();	// Return to time tile
	}
}

static void startMQTT( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.println("MQTT requested");

		if( network.MQTTconnected() )
			Serial.println("MQTT already connected");
		else
			network.MQTTconnect();
	}
}

TlNetwork::TlNetwork( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned )
{
		/*
		 * NTP
		 */

	this->syncButton = new Button( this );
	this->syncButton->Align( LV_ALIGN_IN_TOP_LEFT );	// it is itself aligned on the left
	this->syncButton->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->syncButton->AutoRealign();	// otherwise the icon is shifted
	this->syncButton->setPadding(0);

	this->syncIcon = new Image( this->syncButton );
	this->syncIcon->Set( &timezone_64px );
	this->syncIcon->setClickable( false );	// Pass click to the parent

	this->syncButton->attacheEventeHandler( syncTime );


		/*
		 * MQTT
		 */

	this->MQTTButton = new Button( this );
	this->MQTTButton->Align( LV_ALIGN_OUT_RIGHT_MID, this->syncButton, 20 );	
	this->MQTTButton->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->MQTTButton->AutoRealign();	// otherwise the icon is shifted
	this->MQTTButton->setPadding(0);

	this->MQTTIcon = new Image( this->MQTTButton );
	this->MQTTIcon->Set( &MQTT_64px );
	this->MQTTIcon->setClickable( false );	// Pass click to the parent
	this->MQTTIcon->setPosXY(0,0);

	this->MQTTButton->attacheEventeHandler( startMQTT );
}
