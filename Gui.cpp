/************************************************
*	GUI's root definition
*************************************************/

#include "Gui.h"
#include "StatusBar.h"
#include "font.h"

#define BACKGROUND bg2

	/*****
	 * objects
	 *****/
	
		Gui *gui;

	/**** 
	 * Build the GUI
	 *****/

/* Gui's constructor
 *	Create the gui's skeleton then add all objects
 */
Gui::Gui( void ){

		/* Main style */
	lv_style_set_radius( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_bg_color( this->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_GRAY );
	lv_style_set_bg_opa( this->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_0 );
	lv_style_set_border_width( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_pad_top( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_pad_bottom( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_pad_left( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_pad_right( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_pad_inner( this->getStyle(), LV_OBJ_PART_MAIN, 0 );

	lv_style_set_margin_top( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_margin_bottom( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_margin_left( this->getStyle(), LV_OBJ_PART_MAIN, 0 );
	lv_style_set_margin_right( this->getStyle(), LV_BTN_PART_MAIN, 0 );

	lv_style_set_text_color( this->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_WHITE );
	lv_style_set_image_recolor( this->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_WHITE );

		/* Background images */
	this->_background = lv_img_create( lv_scr_act() , NULL );
	lv_obj_set_width( this->_background, lv_disp_get_hor_res( NULL ) );
	lv_obj_set_height( this->_background, lv_disp_get_ver_res( NULL ) );
	LV_IMG_DECLARE( BACKGROUND );
	lv_img_set_src( this->_background, &BACKGROUND );
 	lv_obj_align( this->_background, NULL, LV_ALIGN_CENTER, 0, 0 );
	lv_obj_set_hidden( this->_background, false );	// Image is visible

		/* Status bar */
	this->_statusbar = new StatusBar( this->getStyle(), lv_scr_act() );

		/* Main container : everything but status bar */
	this->_maintv = new TileView( lv_scr_act() );
	this->_maintv->setSize( LV_HOR_RES, LV_VER_RES - BARHEIGHT );	// Keep some space for the statusbar
	
	this->_maintv->Align( LV_ALIGN_OUT_BOTTOM_MID, this->_statusbar);
	this->_maintv->setEdgeFlash( true );
	this->_maintv->copyStyle( this->getStyle() );	// Copy and apply the main style

		/***
		 * Tiles
		 ***/
	
		/* Set valide tiles position */
	static lv_point_t valid_pos[] = { {0,0}, {0,1} };	// define tiles' position
	this->_maintv->setValidPositions( valid_pos, 2 );	// apply it

	this->_tile_datetime = new TlDateTime( this->getStyle(), this->_maintv->getTileView() , NULL );
	this->_tile_datetime->setSize( this->_maintv );	// Resize as per tileview
	this->_tile_datetime->copyStyle( this->getStyle() );	// Copy and apply style
	this->_maintv->AddTile( this->_tile_datetime );	// Add this tile

	this->_tile_settings = new TlSettings( this->_maintv->getTileView() );
	this->_tile_settings->setSize( this->_maintv );
	this->_tile_settings->copyStyle( this->getStyle() );	// Copy and apply style
	this->_maintv->AddTile( this->_tile_settings );	// Add this tile

		/* The GUI is initialised,
		 * ready to launch automation
		 */
	this->initAutomation();	// Launch automation
}

void Gui::updateStepCounter( uint32_t counter ){
	this->_statusbar->updateStepCounter( counter );
}

void Gui::updateBatteryIcon( lv_icon_battery_t index ){
	this->_statusbar->updateBatteryIcon( index);
}

void Gui::updateBatteryLevel( void ){
	this->_statusbar->updateBatteryLevel();
}

void Gui::initAutomation( void ){
		this->_statusbar->initAutomation();
		this->_tile_datetime->initAutomation();
	}

