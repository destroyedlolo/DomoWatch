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
	network.MQTTsubscribe( "maison/Temperature/Dehors" );
	network.MQTTsubscribe( "maison/Temperature/Salon" );
	network.MQTTsubscribe( "maison/Temperature/Congelateur" );
}

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

void TlNetwork::clearObsoletedValues( void ){
	if(last < time(NULL)-50){	// Clear value if older than 5 minutes
		this->salonText->setText( "--.---" );
		this->jardinText->setText( "--.---" );
		this->congeloText->setText( "--.---" );
		this->consoText->setText( "-----" );
		this->prodText->setText( "-----" );
	} else
		Serial.println("Fresh data");
}

TlNetwork::TlNetwork( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned ), last(0)
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
		 * Temperatures
		 */

	this->tempCont = new Container( this );
	this->tempCont->Align( LV_ALIGN_IN_BOTTOM_LEFT );
	this->tempCont->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->tempCont->AutoRealign();	// otherwise the icon is shifted
	this->tempCont->setPadding(0);
	this->tempCont->setClickable( false );	// Pass click to the parent

	this->salonIcon = new Image( this->tempCont );
	this->salonIcon->Set( &salon_32px );
	this->salonIcon->setClickable( false );

	this->salonText = new Label( this->tempCont );	// Battery value
	this->salonText->setText( "--.---" );
	this->salonText->Align( LV_ALIGN_OUT_RIGHT_MID, this->salonIcon, 10 );
	this->salonText->AutoRealign();

	this->jardinIcon = new Image( this->tempCont );
	this->jardinIcon->Set( &jardin_32px );
	this->jardinIcon->Align( LV_ALIGN_OUT_TOP_MID, this->salonIcon, 0, -15 );
	this->jardinIcon->setClickable( false );

	this->jardinText = new Label( this->tempCont );	// Battery value
	this->jardinText->setText( "--.---" );
	this->jardinText->Align( LV_ALIGN_OUT_RIGHT_MID, this->jardinIcon, 10 );
	this->jardinText->AutoRealign();

	this->congeloIcon = new Image( this->tempCont );
	this->congeloIcon->Set( &congelo_32px );
	this->congeloIcon->Align( LV_ALIGN_OUT_TOP_MID, this->jardinIcon, 0, -15 );
	this->congeloIcon->setClickable( false );

	this->congeloText = new Label( this->tempCont );	// Battery value
	this->congeloText->setText( "--.---" );
	this->congeloText->Align( LV_ALIGN_OUT_RIGHT_MID, this->congeloIcon, 10 );
	this->congeloText->AutoRealign();

		/*
		 * Consumption
		 */
	this->NRJCont = new Container( this );
	this->NRJCont->Align( LV_ALIGN_OUT_RIGHT_MID, this->tempCont, 30 );
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
