/************************************************
*	Main tile : Hour and date
*************************************************/

#include <ctime>
#include <Arduino.h>

#include "TlDateTime.h"
#include "font.h"

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
	this->setSize( parent );

		/* Container to keep it centered */
	this->cont = new Container( this );
	lv_style_set_bg_opa(this->cont->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_80);
	this->applyStyle();
	this->cont->setSize( this->getWidth(), this->getHeight()/2 );
	this->cont->Align( LV_ALIGN_CENTER );

		/* Display time */
	this->timelabel = new Label( this->cont );
	this->timelabel->setFont( &Ubuntu_48px );
	this->timelabel->setText( "??:??" );
	this->timelabel->Align( LV_ALIGN_IN_TOP_MID );
	this->timelabel->AutoRealign();

		/* and date */
	this->datelabel = new Label( this->cont );
	this->datelabel->setFont( &Ubuntu_16px );
	this->datelabel->setText( "??.???? ????" );
	this->datelabel->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->datelabel->AutoRealign();

	/* Debug */
	parent->dumpObj( "Parent (tv)" );
	this->dumpObj( "Tile (cont)" );
	this->cont->dumpObj( "SubCont" );
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
		strftime( buf, sizeof(buf), "%a %d.%b %Y", &info );
		this->datelabel->setText( buf );
	}
}

static void cbUpdTime( lv_task_t *tsk ){
	((TlDateTime *)(tsk->user_data))->updateTime();
}

void TlDateTime::initAutomation( void ){
	this->upd_h_task = lv_task_create( cbUpdTime, 500, LV_TASK_PRIO_MID, this );
}
