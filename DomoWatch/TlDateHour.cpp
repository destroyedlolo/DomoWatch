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

TlDateHour::TlDateHour( Gui *g ):Tile( g->getTileView() ), gui( g ){
	this->timelabel = lv_label_create( this->getTile(), NULL );
	lv_style_copy( &this->timestyle, gui->getStyle() );
	lv_style_set_text_font( &this->timestyle, LV_STATE_DEFAULT, &Ubuntu_48px );
	lv_label_set_text( this->timelabel, "??:??" );
	lv_obj_add_style( this->timelabel, LV_OBJ_PART_MAIN, &this->timestyle );
	lv_obj_align( this->timelabel, NULL, LV_ALIGN_CENTER, 0, 0 );

	this->datelabel = lv_label_create( this->getTile(), NULL );
	lv_style_copy( &this->datestyle, gui->getStyle() );
	lv_style_set_text_font( &this->datestyle, LV_STATE_DEFAULT, &Ubuntu_16px );
	lv_label_set_text( this->datelabel, "??.???? ????" );
	lv_obj_add_style( this->datelabel, LV_OBJ_PART_MAIN, &this->datestyle );

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
}

void TlDateHour::initAutomation( void (*func)(lv_task_t *) ){
	this->upd_task = lv_task_create( func, 500, LV_TASK_PRIO_MID, NULL );
}
