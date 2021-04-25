/************************************************
*	Network's tile
*************************************************/

#include "Gui.h"
#include "Network.h"
#include "TlNetwork.h"

LV_IMG_DECLARE(timezone_64px);
LV_IMG_DECLARE(MQTT_64px);
LV_IMG_DECLARE(brightness_32px);
LV_IMG_DECLARE(soleil_32px);

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

static void startstopMQTT( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){

		if( network.MQTTconnected() ){
			Serial.println("MQTT disconnecting");
			network.MQTTdisconnect();
		} else {
			Serial.println("MQTT requested");
			network.MQTTconnect();
		}
	}
}

void TlNetwork::subscribe( void ){
	network.MQTTsubscribe( "TeleInfo/Consommation/values/PAPP" );
	network.MQTTsubscribe( "TeleInfo/Production/values/PAPP" );
}

bool TlNetwork::msgreceived( const char *topic, const char *payload ){
	if(!strcmp( topic, "TeleInfo/Consommation/values/PAPP" )){
		this->consoText->setText( payload );
		return true;
	} else if(!strcmp( topic, "TeleInfo/Production/values/PAPP" )){
		this->prodText->setText( payload );
		return true;
	}

	return false;
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

	this->MQTTButton->attacheEventeHandler( startstopMQTT );

		/*
		 * Consumption
		 */
	this->NRJCont = new Container( this );
	this->NRJCont->Align( LV_ALIGN_IN_BOTTOM_LEFT );
	this->NRJCont->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->NRJCont->AutoRealign();	// otherwise the icon is shifted
	this->NRJCont->setPadding(0);
	this->NRJCont->setClickable( false );	// Pass click to the parent

	this->consoIcon = new Image( this->NRJCont );
	this->consoIcon->Set( &brightness_32px );
	this->consoIcon->setClickable( false );

	this->consoText = new Label( this->NRJCont );	// Battery value
	this->consoText->setText( "-----" );
	this->consoText->Align( LV_ALIGN_OUT_RIGHT_MID, this->consoIcon, 20 );
	this->consoText->AutoRealign();

	this->prodIcon = new Image( this->NRJCont );
	this->prodIcon->Set( &soleil_32px );
	this->prodIcon->Align( LV_ALIGN_OUT_TOP_MID, this->consoIcon, 2 );
	this->prodIcon->setClickable( false );

	this->prodText = new Label( this->NRJCont );	// Battery value
	this->prodText->setText( "-----" );
	this->prodText->Align( LV_ALIGN_OUT_RIGHT_MID, this->prodIcon, 20 );
	this->prodText->AutoRealign();

}
