/************************************************
*	Status bar : status icons
*************************************************/

#include "StatusBar.h"
#include "Network.h"

	/****
	 * callbacks
	 ***/

static void stepClicked( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.println("RAZ podo");
		ttgo->bma->resetStepCounter();
		gui->updateStepCounter();
	}
}

static void wifiClicked( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.println("WIFI clicked");
		if( network.isActive() ){
			/* Nothing on way : we can disconnect */
			network.disconnect();
		} else if( network.getStatus() == Network::net_status_t::WIFI_NOT_CONNECTED ||
						network.getStatus() == Network::net_status_t::WIFI_FAILED ){
			/* Not connected : we can connect */
			network.connect();
		} else
			Serial.println("Network is processing");
	}
}


	/*****
	 * GUI
	 *****/

LV_IMG_DECLARE(foot_16px);

StatusBar::StatusBar( lv_obj_t *parent, const lv_obj_t *cloned ) : 
	Container( parent, cloned )
{
	this->addStyle( mainStyle );	// Copy gui style

		/***
		 * Customize style of the bar
		 ***/

	this->setBgOpacity( LV_OPA_40 );
	this->setSize( LV_HOR_RES, BARHEIGHT );

		/***
		 * Step counter related 
		 ***/

		/* Create of button above step counter to reset it */
	this->stepButton = new Button( mainStyle, this );
	this->stepButton->setLayout( LV_LAYOUT_ROW_MID );	// child are horizontally aligned
	this->stepButton->setSize( this->getWidth() / 2, this->getHeight() );
	this->stepButton->Align( LV_ALIGN_IN_LEFT_MID );	// it is itself aligned on the left
	this->stepButton->setBgOpacity( LV_OPA_0 );

		/* Only an image of a foot */
	this->stepIcon = new Image( this->stepButton );
	this->stepIcon->Set( &foot_16px );
	this->stepIcon->setClickable( false );	// Pass click to the parent

	this->stepCounter = new Label( this->stepButton );
	this->stepCounter->setText( "??????" );
	this->stepCounter->setClickable( false );	// Pass click to the parent

	this->stepButton->attacheEventeHandler( stepClicked );

	this->updateStepCounter();	// Ensure the counter is not "????" :)

		/***
		 * Battery related
		 *
		 * No need for a button around as no action is possible
		 * so using "classical" alignment
		 ***/
	this->batPercent = new Label( this );	// Battery value
	this->batPercent->setText( "100%" );
	this->batPercent->Align( LV_ALIGN_IN_RIGHT_MID );
	this->batPercent->AutoRealign();

	this->batIcon = new Image( this );		// corresponding icon
	this->batIcon->seTexttFont( &lv_font_montserrat_16 );
	this->batIcon->Set( LV_SYMBOL_BATTERY_FULL );
	this->batIcon->Align( LV_ALIGN_OUT_LEFT_MID,  this->batPercent->getMyself(), -5);

		/***
		 * Wifi
		 *
		 * Set it clickable to enable/disable WiFi networking
		 ****/
	this->wifiIcon = new Image( this );
	this->wifiIcon->seTexttFont( &lv_font_montserrat_16 );
	this->wifiIcon->Set( LV_SYMBOL_WIFI );
	this->wifiIcon->Recolor( LV_COLOR_WHITE );	// Apply the color
	this->wifiIcon->Align( LV_ALIGN_OUT_LEFT_MID, this->batIcon, -15 );
	this->wifiIcon->setIntensity();
	this->wifiIcon->setClickable( true );
	this->wifiIcon->attacheEventeHandler( wifiClicked );
}

void StatusBar::updateStepCounter( void ){
	this->stepCounter->setText( String(ttgo->bma->getCounter()).c_str() );
}

void StatusBar::updateBatteryLevel( void ){
	this->batPercent->setText( (String(ttgo->power->getBattPercentage())+'%').c_str() );
	this->updateBatteryIcon( Gui::LV_ICON_UNKNOWN );	// Rethink the icon
}

void StatusBar::updateBatteryIcon( Gui::lv_icon_battery_t index ){
	lv_color_t color = LV_COLOR_WHITE;	// Default color

	if( index == Gui::LV_ICON_UNKNOWN && ttgo->power->isChargeing() )	// Are we charging ?
		index = Gui::LV_ICON_CHARGE;
	
	if( index == Gui::LV_ICON_CHARGE )
		color = LV_COLOR_BLUE;
	else if( index >= Gui::LV_ICON_CALCULATION ){	// Determine as per battery level
		int level = ttgo->power->getBattPercentage();
		if(level > 95){
			color = LV_COLOR_GREEN;
			index = Gui::LV_ICON_BAT_FULL;
		} else if(level > 80){
			color = LV_COLOR_GREEN;
			index = Gui::LV_ICON_BAT_3;
		} else if(level > 45)
			index = Gui::LV_ICON_BAT_2;
		 else if(level > 20){
			color = LV_COLOR_ORANGE;
			index = Gui::LV_ICON_BAT_1;
		} else {
			color = LV_COLOR_RED;
			index = Gui::LV_ICON_BAT_EMPTY;
		}
	}

	this->batIcon->Recolor( color );	// Apply the color

	static const char *icons[] = {LV_SYMBOL_BATTERY_EMPTY, LV_SYMBOL_BATTERY_1, LV_SYMBOL_BATTERY_2, LV_SYMBOL_BATTERY_3, LV_SYMBOL_BATTERY_FULL, LV_SYMBOL_CHARGE};
	this->batIcon->Set( icons[index] );	// And the icon
}

void StatusBar::updateNetwork( void ){
Serial.printf(" *** upd net : %d\n", network.getStatus() );
	switch( network.getStatus() ){
	case Network::net_status_t::WIFI_CONNECTING :
		this->wifiIcon->Recolor(LV_COLOR_ORANGE);
		break;
	case Network::Network::net_status_t::WIFI_FAILED :
		this->wifiIcon->Recolor(LV_COLOR_RED);
		break;
	case Network::Network::net_status_t::WIFI_CONNECTED :
		this->wifiIcon->Recolor(LV_COLOR_LIME);
		break;
	case Network::net_status_t::WIFI_MQTT :
		this->wifiIcon->Recolor(LV_COLOR_BLUE);
		break;
	default :
		this->wifiIcon->Recolor(LV_COLOR_WHITE);
	}
}

	/*
	 * Utilities
	 */

static void cbUpdBat( lv_task_t *tsk ){
	((StatusBar *)(tsk->user_data))->updateBatteryLevel();
}

void StatusBar::initAutomation( void ){
	this->upd_bat_task = lv_task_create( cbUpdBat, 30000, LV_TASK_PRIO_LOWEST, this );
}
