/************************************************
*	GUI's root definition
*************************************************/

#include "Gui.h"
#include "StatusBar.h"
#include "Network.h"

#define BACKGROUND Annecy	// Which background to use
LV_IMG_DECLARE( BACKGROUND );

LV_FONT_DECLARE( Ubuntu_16px );

	/*****
	 * objects
	 *****/
	
Gui *gui;

Style *mainStyle;
Style *gaugeStyle;
Style *sliderStyle;
Style *stairsStyle;
Style *popupStyle;

	/**** 
	 * Build the GUI
	 *****/

Gui::Gui( void ) : _popup(NULL){

		/***
		 * Build main style
		 ***/
	mainStyle = new Style();
	mainStyle->setRadius( 0 );
	mainStyle->setBgColor( LV_COLOR_GRAY );
	mainStyle->setBgOpacity( LV_OPA_0 );
	mainStyle->setBorderWidth( 0 );
	mainStyle->setTextColor( LV_COLOR_WHITE );
	mainStyle->seTexttFont( &Ubuntu_16px );

		/***
		 * Build Gauge style
		 ***/
	gaugeStyle = new Style();
	gaugeStyle->copyStyle( mainStyle );
	gaugeStyle->setBgColor( LV_COLOR_AQUA );
	gaugeStyle->setBgOpacity( LV_OPA_100 );
	gaugeStyle->setBorderWidth( 1 );
	gaugeStyle->setRadius( 5 );

		/***
		 * Build Slider style
		 ***/
	sliderStyle = new Style();
	sliderStyle->copyStyle( mainStyle );
	sliderStyle->setBgColor( LV_COLOR_AQUA );
	sliderStyle->setBgOpacity( LV_OPA_100 );
	sliderStyle->setBorderWidth( 1 );
	sliderStyle->setRadius( 5 );

		/***
		 * Build drop down style
		 ***/
	stairsStyle = new Style();
	stairsStyle->copyStyle( mainStyle );
	stairsStyle->setRadius( 5 );
	stairsStyle->setBgOpacity( LV_OPA_70 );
	stairsStyle->setBorderWidth( 1 );

		/***
		 * Build Popup style
		 ***/
	popupStyle = new Style();
	popupStyle->copyStyle( mainStyle );
	popupStyle->setBgColor( LV_COLOR_WHITE );
	popupStyle->setBgOpacity( LV_OPA_80 );
	popupStyle->setBorderWidth( 2 );
	popupStyle->setCaptionFont( &lv_font_montserrat_22 );
	popupStyle->setCaptionAlign( LV_ALIGN_IN_TOP_MID );
	popupStyle->setCaptionColor( LV_COLOR_BLACK );

		/***
		 * Background images 
		 ***/
	this->_background = new Image( lv_scr_act() );
	this->_background->setSize( lv_disp_get_hor_res( NULL ), lv_disp_get_ver_res( NULL ) );
	this->_background->Set( &BACKGROUND );
	this->_background->Align( LV_ALIGN_CENTER );
	this->_background->setHidden( false );

		/***
		 * Status bar
		 ***/
	this->_statusbar = new StatusBar( lv_scr_act() );

		/***
		 * Work area
		 * this container is only need to allow heritage b/w tiles and their
		 * tileview.
		 * Without it, child will be shifted down as the tv must leave
		 * room for the status bar.
		 ***/
	this->_workarea = new Container( mainStyle, lv_scr_act() );
	this->_workarea->setSize( LV_HOR_RES, LV_VER_RES - BARHEIGHT);	// Keep some space for the statusbar
	this->_workarea->Align( LV_ALIGN_OUT_BOTTOM_MID, this->_statusbar);

		/***
		 * Tileview
		 ***/

	this->_tileview = new TileView( mainStyle, this->_workarea );
	this->_tileview->setEdgeFlash( true );

		/***
		 * Define tiles
		 ***/

		 // main tile placed centered
	this->_tile_datetime = new TlDateTime( this->_tileview, this->_tileview );
	this->_tile_datetime->setTilePos( {1, 1} );	// place it on the right
	this->_tileview->AddTile( this->_tile_datetime );	// Add this tile

		// status one, placed on the left
	this->_tile_status = new TlStatus( this->_tileview, this->_tileview );
	this->_tile_status->setTilePos( {0, 1} );
	this->_tileview->AddTile( this->_tile_status );

		// settings, bottom
	this->_tile_screen = new TlScreen( this->_tileview, this->_tileview );
	this->_tile_screen->setTilePos( {1, 2} );
	this->_tileview->AddTile( this->_tile_screen );

		// Network, on the top
	this->_tile_network = new TlNetwork( this->_tileview, this->_tileview );
	this->_tile_network->setTilePos( {1, 0} );
	this->_tileview->AddTile( this->_tile_network );

			// Shutters, top-left
	this->_tile_shutter = new TlShutter( this->_tileview, this->_tileview );
	this->_tile_shutter->setTilePos( {0, 0} );
	this->_tileview->AddTile( this->_tile_shutter );

	this->updateMovements();	// Allow movement

		// date and time is the default tile
	this->_tileview->setActiveTile( 1,1, LV_ANIM_OFF );

		/* The GUI is initialised,
		 * ready to launch automation
		 */
	this->initAutomation();	// Launch automation
}

void Gui::updateStepCounter( void ){
	this->_statusbar->updateStepCounter();
}

void Gui::updateBatteryIcon( lv_icon_battery_t index ){
	this->_statusbar->updateBatteryIcon( index );
}

void Gui::updateBatteryLevel( void ){
	this->_statusbar->updateBatteryLevel();
}

void Gui::updateNetwork( void ){
	this->_statusbar->updateNetwork();
}

void Gui::initAutomation( void ){
	this->_statusbar->initAutomation();
	this->_tile_datetime->initAutomation();
	this->_tile_status->initAutomation();
}

#define TABSIZE(t) ( sizeof(t) / sizeof(t[1]) )

void Gui::updateMovements( void ){

		// As the GUI is changing, we may have to close popup
	this->closePopup();

	int sz = 0;
	Network::net_capacities_t caps = network.getCapacities();	// What is currently active

		// define tiles valid positions
	const lv_point_t basic_pos[] = { {0,1}, {1,1}, {1,2} };	// basic interface
	const lv_point_t net_add_pos[] = { {1,0} };		// Network addendum
	const lv_point_t mqtt_add_pos[] = { {0,0} };	// MQTT addadum

	static lv_point_t valid_pos[ 
		TABSIZE(basic_pos) + 
		TABSIZE(net_add_pos) +
		TABSIZE(mqtt_add_pos)
	];

	for(int i=0; i<TABSIZE(basic_pos); i++)
		valid_pos[sz++] = basic_pos[i];

	if( caps & _BV(Network::NET_CAP_WIFI) ){
		_tile_network->clearObsoletedValues();	// check if value are still accurate
		for(int i=0; i<TABSIZE(net_add_pos); i++)
			valid_pos[sz++] = net_add_pos[i];
	}

	if( caps & _BV(Network::NET_CAP_MQTT) ){
		for(int i=0; i<TABSIZE(mqtt_add_pos); i++)
			valid_pos[sz++] = mqtt_add_pos[i];
	}

		// get the actual position
	lv_coord_t x,y;
	this->_tileview->getActiveTile( x,y );

		// and check if it part of the valid ones
	bool valid = false;
	for(int i=0; i<sz; i++){
		if( valid_pos[i].x == x && valid_pos[i].y == y ){
			valid = true;
			break;
		}
	}

	if(!valid)	// if not, Back to home
		this->backToHome();

		// Notez bien : it has to be done AFTER moving to home if needed
		// otherwise, LVGL is lost and setActiveTile() is not doing the
		// expected result
	this->_tileview->setValidPositions( valid_pos, sz);	// Finally apply the new one
}

void Gui::backToHome( lv_anim_enable_t anim ){
	this->_tileview->setActiveTile( 1,1, anim );
}

void Gui::subscribe( void ){
	this->_tile_network->subscribe();
}

void Gui::msgreceived( const char *topic, const char *payload ){
	bool ret = this->_tile_network->msgreceived( topic, payload );
	if( !ret && this->_popup )
		ret = this->_popup->msgreceived( topic, payload );

	if( !ret )
		Serial.printf("%s : ignored\n", topic);
}
