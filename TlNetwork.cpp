/************************************************
*	Network's tile
*************************************************/

#include "Gui.h"
#include "Network.h"
#include "TlNetwork.h"

LV_IMG_DECLARE(timezone_64px);

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
		ttgo->rtc->syncToRtc();

		gui->backToHome();	// Return to time tile
	}
}

TlNetwork::TlNetwork( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned )
{
		/* button above the icon to handle action */
	this->syncButton = new Button( this );
	this->syncButton->setLayout( LV_LAYOUT_ROW_MID );	// child are horizontally aligned
//	this->syncButton->setSize( this->getWidth() / 2, this->getHeight() );
	this->syncButton->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->syncButton->Align( LV_ALIGN_IN_LEFT_MID );	// it is itself aligned on the left
	lv_style_set_bg_opa( this->syncButton->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_0 );
	this->syncButton->applyStyle();

	this->syncIcon = new Image( this->syncButton );
	this->syncIcon->Set( &timezone_64px );
	this->syncIcon->setClickable( false );	// Pass click to the parent

	this->syncButton->attacheEventeHandler( syncTime );
}
