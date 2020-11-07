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
/*
	this->_maincont = new Container( lv_scr_act(), NULL );
	this->_maincont->setSize( LV_HOR_RES, LV_VER_RES - BARHEIGHT );	// Keep some space for the statusbar
	this->_maincont->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->_maincont->copyStyle( this->getStyle() );	// Copy and apply the main style

	this->_maintv = new TileView( this->_maincont->getContainer() );
*/
	this->_maintv = new TileView( lv_scr_act() );
	this->_maintv->setSize( LV_HOR_RES, LV_VER_RES - BARHEIGHT );	// Keep some space for the statusbar
	this->_maintv->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->_maintv->setEdgeFlash( true );
//	this->_maintv->Align( LV_ALIGN_CENTER );
	this->_maintv->copyStyle( this->getStyle() );	// Copy and apply the main style


		/***
		 * Tiles
		 ***/
	
		/* Set valide tiles position */
	static lv_point_t valid_pos[] = { {0,0}, {0,1} };	// define tiles' position
	this->_maintv->setValidPositions( valid_pos, 2 );	// apply it

	this->_tile_datetime = new TlDateTime( this->_maintv->getTileView(), this->_maintv->getTileView() );
	this->_tile_datetime->setSize( this->_maintv );	// Resize as per tileview
	this->_tile_datetime->copyStyle( this->getStyle() );	// Copy and apply style
	this->_maintv->AddTile( this->_tile_datetime );	// Add this tile

	this->_tile_settings = new TlSettings( this->_maintv->getTileView(), this->_maintv->getTileView() );
	this->_tile_settings->setSize( this->_maintv );
	this->_tile_settings->copyStyle( this->getStyle() );	// Copy and apply style
	this->_maintv->AddTile( this->_tile_settings );	// Add this tile
}

void Gui::updateStepCounter( uint32_t counter ){
	_statusbar->updateStepCounter( counter );
}

void Gui::updateBatteryIcon( lv_icon_battery_t index ){
	lv_color_t color = LV_COLOR_WHITE;
	if( index == LV_ICON_UNKNOWN && ttgo->power->isChargeing() )
		index = LV_ICON_CHARGE;
	
	if( index == LV_ICON_CHARGE )
		color = LV_COLOR_BLUE;
	else if( index >= LV_ICON_CALCULATION ){
		int level = ttgo->power->getBattPercentage();
		if(level > 95){
			color = LV_COLOR_GREEN;
			index = LV_ICON_BAT_FULL;
		} else if(level > 80)
			index = LV_ICON_BAT_3;
		else if(level > 45)
			index = LV_ICON_BAT_2;
		 else if(level > 20){
			color = LV_COLOR_ORANGE;
			index = LV_ICON_BAT_1;
		} else {
			color = LV_COLOR_RED;
			index = LV_ICON_BAT_EMPTY;
		}
	}

	_statusbar->updateBatteryIcon( index, color );
}

void Gui::updateBatteryLevel( void ){
	_statusbar->updateBatteryLevel();
}
