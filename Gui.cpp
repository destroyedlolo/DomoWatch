/************************************************
*	GUI's root definition
*************************************************/

#include "Gui.h"
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

		/* Main container : everything but status bar */
	this->_maincont = new Container( lv_scr_act(), NULL );
	this->_maincont->setSize( LV_HOR_RES, LV_VER_RES - BARHEIGHT );	// Keep some space for the statusbar
	this->_maincont->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->_maincont->copyStyle( this->getStyle() );	// Copy and apply the main style

	this->_maintv = new TileView( this->_maincont->getContainer() );
	this->_maintv->setEdgeFlash( true );
	this->_maintv->Align( LV_ALIGN_CENTER );
	this->_maintv->copyStyle( this->getStyle() );	// Copy and apply the main style

		/***
		 * Tiles
		 ***/
	
		/* Set valide tiles position */
	this->_maintv->setValidPositions( NULL, 0 );

	this->_tile_datetime = new TlDateTime( this->_maintv->getTileView(), this->_maintv->getTileView() );
	this->_tile_datetime->setSize( this->_maintv );
	this->_maintv->AddTile( this->_tile_datetime );	// Add this tile
}
