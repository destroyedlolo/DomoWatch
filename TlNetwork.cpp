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
LV_IMG_DECLARE(jardin_32px);
LV_IMG_DECLARE(salon_32px);
LV_IMG_DECLARE(congelo_32px);

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
	if( event == LV_EVENT_CLICKED ){
		Serial.println("Time synchronisation requested");

		configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", "pool.ntp.org");

		gui->backToHome();	// Return to time tile
	}
}

/* Connect or disconnect to the broker
 *
 * It's also asynchronous
 */
static void startstopMQTT( lv_obj_t *, lv_event_t event ){
	if( event == LV_EVENT_CLICKED ){
		if( network.MQTTconnected() ){
			Serial.println("MQTT disconnecting");
			network.MQTTdisconnect();
		} else {
			Serial.println("MQTT requested");
			network.MQTTconnect();
		}
	}
}

static void salonPopup( lv_obj_t *, lv_event_t event ){
	if( event == LV_EVENT_CLICKED && network.MQTTconnected() ){
		Serial.println("Salon history popup");
		gui->openPopup( DWPopup::Kind::SALON );
	}
}

/* Subscribe to MQTT's topics
 */
void TlNetwork::subscribe( void ){
	network.MQTTsubscribe( "TeleInfo/Consommation/values/PAPP" );
	network.MQTTsubscribe( "TeleInfo/Production/values/PAPP" );
	network.MQTTsubscribe( "maison/Temperature/Dehors" );
	network.MQTTsubscribe( "maison/Temperature/Salon" );
	network.MQTTsubscribe( "maison/Temperature/Congelateur" );
}

/* We got a message !
 * -> const char *topic : which topic
 * -> const char *payload : message's content
 *	(only text payload is supported)
 */
bool TlNetwork::msgreceived( const char *topic, const char *payload ){
	this->last = time(NULL);
	if(!strcmp( topic, "TeleInfo/Consommation/values/PAPP" )){
		this->consoText->setText( payload );
		return true;
	} else if(!strcmp( topic, "TeleInfo/Production/values/PAPP" )){
		this->prodText->setText( payload );
		return true;
	} else if(!strcmp( topic, "maison/Temperature/Dehors" )){
		this->jardinText->setText( payload );
		return true;
	} else if(!strcmp( topic, "maison/Temperature/Salon" )){
		this->salonText->setText( payload );
		return true;
	} else if(!strcmp( topic, "maison/Temperature/Congelateur" )){
		this->congeloText->setText( payload );
		return true;
	}

	return false;
}



/* Check if display is too old and need to be cleared
 */
void TlNetwork::clearObsoletedValues( void ){
	if(last < time(NULL)-500){	// Clear value if older than 5 minutes
		this->salonText->setText( "--.---" );
		this->jardinText->setText( "--.---" );
		this->congeloText->setText( "--.---" );
		this->consoText->setText( "-----" );
		this->prodText->setText( "-----" );
	} else
		Serial.println("Fresh data");
}


	/****
	 * GUI
	 ***/

TlNetwork::TlNetwork( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned ), last(0)
{
		/*
		 * NTP
		 */

	this->syncIcon = new Image( this );
	this->syncIcon->Align( LV_ALIGN_IN_TOP_LEFT );	// it is itself aligned on the left
	this->syncIcon->Set( &timezone_64px );
	this->syncIcon->setClickable( true );
	this->syncIcon->attacheEventeHandler( syncTime );

		/*
		 * MQTT
		 */

	this->MQTTIcon = new Image( this );
	this->MQTTIcon->Set( &MQTT_64px );
	this->MQTTIcon->Align( LV_ALIGN_OUT_RIGHT_MID, this->syncIcon, 20 );	
	this->MQTTIcon->setClickable( true );
	this->MQTTIcon->attacheEventeHandler( startstopMQTT );

		/*
		 * Temperatures
		 */

	this->congeloCont = new Container( mainStyle, this->getMyself() );
	this->congeloCont->Align( LV_ALIGN_IN_BOTTOM_LEFT );
	this->congeloCont->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->congeloCont->AutoRealign();	// otherwise the icon is shifted
	this->congeloCont->setPadding(0);
	this->congeloCont->setClickable( false);

	this->congeloIcon = new Image( this->congeloCont );
	this->congeloIcon->Set( &congelo_32px );
	this->congeloIcon->Align( LV_ALIGN_IN_LEFT_MID );
	this->congeloIcon->setClickable( false );

	this->congeloText = new Label( this->congeloCont );	// Battery value
	this->congeloText->setText( "--.---" );
	this->congeloText->Align( LV_ALIGN_OUT_RIGHT_MID, this->congeloIcon, 10 );
	this->congeloText->AutoRealign();


	this->jardinCont = new Container( mainStyle, this->getMyself() );
	this->jardinCont->Align( LV_ALIGN_OUT_TOP_LEFT, this->congeloCont, 0, -15);
	this->jardinCont->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->jardinCont->AutoRealign();	// otherwise the icon is shifted
	this->jardinCont->setPadding(0);
	this->jardinCont->setClickable( false );	// Pass click to the parent

	this->jardinIcon = new Image( this->jardinCont );
	this->jardinIcon->Set( &jardin_32px );
	this->jardinIcon->Align( LV_ALIGN_IN_LEFT_MID );
	this->jardinIcon->setClickable( false );

	this->jardinText = new Label( this->jardinCont );	// Battery value
	this->jardinText->setText( "--.---" );
	this->jardinText->Align( LV_ALIGN_OUT_RIGHT_MID, this->jardinIcon, 10 );
	this->jardinText->AutoRealign();


	this->salonCont = new Container( mainStyle, this->getMyself() );
	this->salonCont->Align( LV_ALIGN_OUT_TOP_LEFT, this->jardinCont, 0, -15);
	this->salonCont->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->salonCont->AutoRealign();	// otherwise the icon is shifted
	this->salonCont->setPadding(0);
	this->salonCont->setClickable( true);
	this->salonCont->attacheEventeHandler( salonPopup );

	this->salonIcon = new Image( this->salonCont );
	this->salonIcon->Set( &salon_32px );
	this->salonIcon->setClickable( false );

	this->salonText = new Label( this->salonCont );	// Battery value
	this->salonText->setText( "--.---" );
	this->salonText->Align( LV_ALIGN_OUT_RIGHT_MID, this->salonIcon, 10 );
	this->salonText->AutoRealign();

		/*
		 * Consumption
		 */

	this->NRJCont = new Container( mainStyle, this->getMyself() );
	this->NRJCont->Align( LV_ALIGN_OUT_RIGHT_BOTTOM, this->congeloCont, 30 );
	this->NRJCont->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->NRJCont->AutoRealign();	// otherwise the icon is shifted
	this->NRJCont->setPadding(0);
	this->NRJCont->setClickable( false );	// Pass click to the parent

	this->consoIcon = new Image( this->NRJCont );
	this->consoIcon->Set( &brightness_32px );
	this->consoIcon->setClickable( false );

	this->consoText = new Label( this->NRJCont );	// Battery value
	this->consoText->setText( "-----" );
	this->consoText->Align( LV_ALIGN_OUT_RIGHT_MID, this->consoIcon, 10 );
	this->consoText->AutoRealign();

	this->prodIcon = new Image( this->NRJCont );
	this->prodIcon->Set( &soleil_32px );
	this->prodIcon->Align( LV_ALIGN_OUT_TOP_MID, this->consoIcon, 0, -15 );
	this->prodIcon->setClickable( false );

	this->prodText = new Label( this->NRJCont );	// Battery value
	this->prodText->setText( "-----" );
	this->prodText->Align( LV_ALIGN_OUT_RIGHT_MID, this->prodIcon, 10 );
	this->prodText->AutoRealign();
}
