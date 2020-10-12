/************************************************
*	GUI's root definition
*************************************************/
#include "Gui.h"
#include "TlDateHour.h"

#include "myfont.h"

	/* "standard" font */
LV_FONT_DECLARE(Ubuntu);
LV_FONT_DECLARE(Geometr);

	/* Wallpaper */
#if 0
LV_IMG_DECLARE(bg);
LV_IMG_DECLARE(bg1);
LV_IMG_DECLARE(bg2);
LV_IMG_DECLARE(bg3);
LV_IMG_DECLARE(WALLPAPER_1_IMG);
LV_IMG_DECLARE(WALLPAPER_2_IMG);
LV_IMG_DECLARE(WALLPAPER_3_IMG);
#endif


	/*****
	 * objects
	 *****/
	
class Gui *gui;


	/**** 
	 * Build the GUI
	 *****/

/* Gui's constructor
 *	Create the gui's skeleton then add all objects
 */
Gui::Gui(){

		/* Main style */
	lv_style_init( &this->mainbar_style );
    lv_style_set_radius( &this->mainbar_style, LV_OBJ_PART_MAIN, 0 );
    lv_style_set_bg_color( &this->mainbar_style, LV_OBJ_PART_MAIN, LV_COLOR_GRAY );
    lv_style_set_bg_opa( &this->mainbar_style, LV_OBJ_PART_MAIN, LV_OPA_0 );
    lv_style_set_border_width( &this->mainbar_style, LV_OBJ_PART_MAIN, 0 );
    lv_style_set_text_color( &this->mainbar_style, LV_OBJ_PART_MAIN, LV_COLOR_WHITE );
    lv_style_set_image_recolor( &this->mainbar_style, LV_OBJ_PART_MAIN, LV_COLOR_WHITE );

		/* Background images */
	LV_IMG_DECLARE( bg2 );
	this->background = lv_img_create( lv_scr_act() , NULL );
	lv_obj_set_width( this->background, lv_disp_get_hor_res( NULL ) );
	lv_obj_set_height( this->background, lv_disp_get_ver_res( NULL ) );

	lv_img_set_src( this->background, &bg2 );
 	lv_obj_align( this->background, NULL, LV_ALIGN_CENTER, 0, 0 );
	lv_obj_set_hidden( this->background, false );	// Image is visible

		/* Tileview : how tiles will be managed */
	this->mainbar = lv_tileview_create( lv_scr_act(), NULL);
	lv_obj_add_style( this->mainbar, LV_OBJ_PART_MAIN, &this->mainbar_style );
    lv_page_set_scrlbar_mode( this->mainbar, LV_SCRLBAR_MODE_OFF);

		// For the moment, not movement 
	lv_tileview_set_valid_positions( this->mainbar, NULL, 0 );
    lv_tileview_set_edge_flash( this->mainbar, true);

		/* Create tiles */
}


	/*****
	 * Interfaces 
	 *****/
void Gui::updateStepCounter(uint32_t counter){
}

void Gui::updateBatteryIcon(lv_icon_battery_t index){
}

void Gui::updateBatteryLevel(void){
}
