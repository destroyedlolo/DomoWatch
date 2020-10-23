/************************************************
*	Main tile : Hour and date
*************************************************/

#include "TlDateHour.h"
#include "myfont.h"

	/****
	 * Fonts 
	 ****
	 * Need to be defined to be used
	 ****/

LV_FONT_DECLARE(Ubuntu_48px);
LV_FONT_DECLARE(Ubuntu_16px);

TlDateHour::TlDateHour( Gui *g ) : /*Tile( g->getTileView() ), */ gui( g ), prev_day(-1){

		/* Create a container to align labels in
		 * the middle of the screen
		 */
/*D Change lv_scr_act() avec le tile */
	this->contener = lv_obj_create( lv_scr_act(), NULL );
	lv_obj_set_size( this->contener, lv_disp_get_hor_res( NULL ) , lv_disp_get_ver_res( NULL ) / 2 );
	lv_obj_align( this->contener, NULL, LV_ALIGN_CENTER, 0, 0 );
	lv_obj_add_style( this->contener, LV_OBJ_PART_MAIN, gui->getStyle() );

		/* Display time */
	this->timelabel = lv_label_create( this->contener, NULL );
	lv_style_copy( &this->timestyle, gui->getStyle() );
	lv_style_set_text_font( &this->timestyle, LV_STATE_DEFAULT, &Ubuntu_48px );
	lv_label_set_text( this->timelabel, "??:??" );
	lv_obj_add_style( this->timelabel, LV_OBJ_PART_MAIN, &this->timestyle );
	lv_obj_align( this->timelabel, this->contener, LV_ALIGN_CENTER, 0, 0 );

	this->datelabel = lv_label_create( this->contener, NULL );
	lv_style_copy( &this->datestyle, gui->getStyle() );
	lv_style_set_text_font( &this->datestyle, LV_STATE_DEFAULT, &Ubuntu_16px );
	lv_label_set_text( this->datelabel, "??.???? ????" );
	lv_obj_add_style( this->datelabel, LV_OBJ_PART_MAIN, &this->datestyle );
	lv_obj_align( this->datelabel, this->contener, LV_ALIGN_IN_BOTTOM_MID, 0, 0 );

	Serial.println("TimeDate created");
}

void TlDateHour::updateTime( void ){
	time_t now;
	struct tm  info;
	char buf[64];

	time(&now);
	localtime_r(&now, &info);
	strftime(buf, sizeof(buf), "%H:%M:%S", &info);
	lv_label_set_text(this->timelabel, buf);
	lv_obj_align(this->timelabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);

	if(info.tm_yday != this->prev_day){
		this->prev_day = info.tm_yday;
		strftime( buf, sizeof(buf), "%a %d.%b %Y", &info );
		lv_label_set_text(this->datelabel, buf);
		lv_obj_align(this->timelabel, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 20);
	}
}

void TlDateHour::initAutomation( void (*func)(lv_task_t *) ){
	this->upd_task = lv_task_create( func, 500, LV_TASK_PRIO_MID, NULL );
}
