/************************************************
*	GUI's root definition
*************************************************/

#include "Gui.h"
#include "font.h"

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
	this->background = lv_img_create( lv_scr_act() , NULL );
	lv_obj_set_width( this->background, lv_disp_get_hor_res( NULL ) );
	lv_obj_set_height( this->background, lv_disp_get_ver_res( NULL ) );

	LV_IMG_DECLARE( bg2 );
	lv_img_set_src( this->background, &bg2 );
 	lv_obj_align( this->background, NULL, LV_ALIGN_CENTER, 0, 0 );
	lv_obj_set_hidden( this->background, false );	// Image is visible

	static const lv_point_t valid_pos[] = {{0,0}, {0, 1}, {1,1}, {LV_COORD_MIN, LV_COORD_MIN}};
    lv_obj_t *tileview;
    tileview = lv_tileview_create(lv_scr_act(), NULL);
    lv_tileview_set_valid_positions(tileview, valid_pos, 3);
    lv_tileview_set_edge_flash(tileview, true);

    lv_obj_t * tile1 = lv_obj_create(tileview, NULL);
    lv_obj_set_size(tile1, LV_HOR_RES, LV_VER_RES);
    lv_tileview_add_element(tileview, tile1);

    /*Tile1: just a label*/
    lv_obj_t * label = lv_label_create(tile1, NULL);
    lv_label_set_text(label, "Scroll down");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

    /*Tile2: a list*/
    lv_obj_t * list = lv_list_create(tileview, NULL);
    lv_obj_set_size(list, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(list, 0, LV_VER_RES);
    lv_list_set_scroll_propagation(list, true);
    lv_list_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);

    lv_list_add_btn(list, NULL, "One");
    lv_list_add_btn(list, NULL, "Two");
    lv_list_add_btn(list, NULL, "Three");
    lv_list_add_btn(list, NULL, "Four");
    lv_list_add_btn(list, NULL, "Five");
    lv_list_add_btn(list, NULL, "Six");
    lv_list_add_btn(list, NULL, "Seven");
    lv_list_add_btn(list, NULL, "Eight");

    /*Tile3: a button*/
    lv_obj_t * tile3 = lv_obj_create(tileview, tile1);
    lv_obj_set_pos(tile3, LV_HOR_RES, LV_VER_RES);
    lv_tileview_add_element(tileview, tile3);

    lv_obj_t * btn = lv_btn_create(tile3, NULL);
    lv_obj_align(btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_tileview_add_element(tileview, btn);
    label = lv_label_create(btn, NULL);
    lv_label_set_text(label, "No scroll up");
}
