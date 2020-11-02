/************************************************
*	GUI's root definition
*************************************************/
#include "Gui.h"
#include "StatusBar.h"
#include "TlDateHour.h"
#include "TlSettings.h"

#include "myfont.h"

	/* Wallpaper */
#if 0
LV_IMG_DECLARE(bg);
LV_IMG_DECLARE(bg1);	// Aurore violet en bas
LV_IMG_DECLARE(bg2);	// Dégradé avec hallo jaune a droite
LV_IMG_DECLARE(bg3);	// mauve avec ecriture en font
LV_IMG_DECLARE(WALLPAPER_1_IMG);
LV_IMG_DECLARE(WALLPAPER_2_IMG);
LV_IMG_DECLARE(WALLPAPER_3_IMG);
#endif


	/*****
	 * objects
	 *****/
	
		Gui *gui;
static 	StatusBar	*statusbar;

	/*****
	 * Tiles
	 *****/

static TlDateHour	*hourdate;
static TlSettings	*tlsettings;

	/* set allowed navigation */
static lv_point_t valid_pos[] = { {0,0}, {0,1} };

	/*****
	 * Automation
	 *****/

void cbUpdDH( lv_task_t * ){
	hourdate->updateTime();
}

void cbUpdBat( lv_task_t * ){
	statusbar->updateBatteryLevel();
}

/* Launch automation tasks.
 * Has to be called ONLY when everything is initialised
 */
void initGuiAutomation( void ){
	hourdate->initAutomation( cbUpdDH );
	statusbar->initAutomation( cbUpdBat );
}

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

		/* Tileview : how tiles will be managed */
	this->mainbar = lv_tileview_create( lv_scr_act(), NULL);
	this->ApplyStyle();
	lv_page_set_scrlbar_mode( this->mainbar, LV_SCRLBAR_MODE_OFF );

		/* Create status bar */
	statusbar = new StatusBar( this, lv_scr_act() );

		/* Where tiles will be positionned */
	lv_tileview_set_valid_positions( this->mainbar, valid_pos, sizeof(valid_pos)/sizeof(valid_pos[1]) );
	lv_tileview_set_edge_flash( this->mainbar, true);

		/* Create tiles */
	hourdate = new TlDateHour( this );
	tlsettings = new TlSettings( this );

		/* The GUI is initialised,
		 * ready to launch automation
		 */
	initGuiAutomation();	// Launch automation
}


	/*****
	 * Interfaces 
	 *****/
void Gui::updateStepCounter( uint32_t counter ){
	statusbar->updateStepCounter( counter );
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

	statusbar->updateBatteryIcon( index, color );
}

void Gui::updateBatteryLevel( void ){
	statusbar->updateBatteryLevel();
}
