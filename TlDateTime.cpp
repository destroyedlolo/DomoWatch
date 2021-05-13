/************************************************
*	Main tile : Hour and date
*************************************************/

#include <ctime>
#include <Arduino.h>

#include "Gui.h"
#include "TlDateTime.h"

	/****
	 * Fonts 
	 ****
	 * Need to be defined to be used
	 ****/

LV_FONT_DECLARE(Ubuntu_48px);
LV_FONT_DECLARE(Ubuntu_16px);

TlDateTime::TlDateTime( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned ),
	daynum( -1 )
{

		/* Container to keep it centered */
	this->cont = new Container( mainStyle, this->getMyself() );
	this->cont->setSize( this->getWidth(), this->getHeight()/2 );
	this->cont->Align( LV_ALIGN_CENTER );
	this->cont->setClickable( false );	// Pass click to the parent

		/* Display time */
	this->timelabel = new Label( this->cont );
	this->timelabel->seTexttFont( &Ubuntu_48px );
	this->timelabel->setText( "??:??:??" );
	this->timelabel->Align( LV_ALIGN_IN_TOP_MID );
	this->timelabel->AutoRealign();
	this->timelabel->setClickable( false );	// Pass click to the parent

		/* and date */
	this->datelabel = new Label( this->cont );
	this->datelabel->seTexttFont( &Ubuntu_16px );
	this->datelabel->setText( "??.???? ????" );
	this->datelabel->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->datelabel->AutoRealign();
	this->datelabel->setClickable( false );	// Pass click to the parent

#if 0
LV_IMG_DECLARE(foot_16px);
lv_obj_t 	*_obj = lv_img_create( lv_scr_act(), NULL );
lv_obj_align( _obj, NULL, LV_ALIGN_IN_BOTTOM_MID, 0,0 );
lv_img_set_src( _obj, LV_SYMBOL_DUMMY "Test" );
Serial.printf(" Obj : %d,%d %dx%d\n", 
		lv_obj_get_x(_obj), lv_obj_get_y(_obj),
		lv_obj_get_width(_obj), lv_obj_get_height(_obj)
);

#if 0
	Image *tst = new Image( lv_scr_act() );
	tst->Align( LV_ALIGN_IN_BOTTOM_MID);
	tst->Set( &foot_16px );
tst->dumpObj("tst DT");
#endif
#endif
}

void TlDateTime::updateTime( void ){
	time_t now;
	struct tm  info;
	char buf[64];

	time( &now );
	localtime_r( &now, &info );

	strftime( buf, sizeof(buf), "%H:%M:%S", &info );
	this->timelabel->setText( buf );

	if(info.tm_yday != this->daynum){
		this->daynum = info.tm_yday;
		strftime( buf, sizeof(buf), "%a %d %b %Y", &info );
		this->datelabel->setText( buf );
	}
}

static void cbUpdTime( lv_task_t *tsk ){
	((TlDateTime *)(tsk->user_data))->updateTime();
}

void TlDateTime::initAutomation( void ){
	this->upd_h_task = lv_task_create( cbUpdTime, 500, LV_TASK_PRIO_MID, this );
}
