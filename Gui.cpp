/************************************************
*	GUI's root definition
*************************************************/

#include "Gui.h"
#include "StatusBar.h"
#include "Network.h"

#define BACKGROUND Annecy	// Which background to use

	/*****
	 * objects
	 *****/
	
Gui *gui;

	/**** 
	 * Build the GUI
	 *****/

Gui::Gui( void ){

		/***
		 * Build main style
		 ***/
	lv_style_set_radius( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_bg_color( this->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_GRAY );
	lv_style_set_bg_opa( this->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_0 );
	lv_style_set_border_width( this->getStyle(), LV_OBJ_PART_MAIN, 0 );

	lv_style_set_text_color( this->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_WHITE );
	lv_style_set_image_recolor( this->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_WHITE );

		/***
		 * Background images 
		 ***/
	this->_background = lv_img_create( lv_scr_act() , NULL );
	lv_obj_set_width( this->_background, lv_disp_get_hor_res( NULL ) );
	lv_obj_set_height( this->_background, lv_disp_get_ver_res( NULL ) );
	LV_IMG_DECLARE( BACKGROUND );
	lv_img_set_src( this->_background, &BACKGROUND );
 	lv_obj_align( this->_background, NULL, LV_ALIGN_CENTER, 0, 0 );
	lv_obj_set_hidden( this->_background, false );	// Image is visible

		/***
		 * Status bar
		 ***/
	this->_statusbar = new StatusBar( this->getStyle(), lv_scr_act() );

		/***
		 * Work area
		 * this container is only need to allow heritage b/w tiles and their
		 * tileview.
		 * Without it, child will be shifted down as the tv must leave
		 * room for the status bar.
		 ***/
	this->_workarea = new Container( lv_scr_act() );
	this->_workarea->setSize( LV_HOR_RES, LV_VER_RES - BARHEIGHT);	// Keep some space for the statusbar
	this->_workarea->copyStyle( this->getStyle() );
//	lv_style_set_bg_opa( this->_workarea->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_10 );
	this->_workarea->Align( LV_ALIGN_OUT_BOTTOM_MID, this->_statusbar);
	this->_workarea->applyStyle();

		
		/***
		 * Tileview
		 ***/
	this->_tileview = new TileView( this->_workarea );
	this->_tileview->setEdgeFlash( true );
// this->_tileview->dumpObj("tile");

		/***
		 * And allowed movements
		 ***/
	this->updateMovements();
	
		/***
		 * Define tiles
		 ***/

		 // main tile placed centered
	this->_tile_datetime = new TlDateTime( this->_tileview, this->_tileview );
	this->_tile_datetime->setTilePos( {1, 1} );	// place it on the right
	this->_tileview->AddTile( this->_tile_datetime );	// Add this tile
// this->_tile_datetime->dumpObj("datetime");

		// Network, on the top
	this->_tile_network = new TlNetwork( this->_tileview, this->_tileview );
	this->_tile_network->setTilePos( {1, 0} );	// place it on the right
	this->_tileview->AddTile( this->_tile_network );	// Add this tile
// this->_tile_network->dumpObj("net");

		// status one, placed on the left
	this->_tile_status = new TlStatus( this->_tileview, this->_tileview );
	this->_tile_status->setTilePos( {0, 1} );
	this->_tileview->AddTile( this->_tile_status );	// Add this tile
// this->_tile_status->dumpObj("status");

		// settings, bottom
	this->_tile_screen = new TlScreen( this->_tileview, this->_tileview );
	this->_tile_screen->setTilePos( {1, 2} );
	this->_tileview->AddTile( this->_tile_screen );	// Add this tile
// this->_tile_screen->dumpObj("screen");

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
	this->_statusbar->updateBatteryIcon( index);
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
	int sz = 0;

		// define tiles valid positions
	const lv_point_t basic_pos[] = { {0,1}, {1,1}, {1,2} };	// basic interface
	const lv_point_t net_add_pos[] = { {1,0} };	// Network addadum

	static lv_point_t valid_pos[ TABSIZE(basic_pos) + TABSIZE(net_add_pos) ];

	for(int i=0; i<TABSIZE(basic_pos); i++)
		valid_pos[sz++] = basic_pos[i];

	if( network.isActive() ){
		for(int i=0; i<TABSIZE(net_add_pos); i++)
			valid_pos[sz++] = net_add_pos[i];
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
	/* bool ret = */ this->_tile_network->msgreceived( topic, payload );
}
