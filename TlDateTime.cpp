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
	this->datelabel->setText( "??? ??.????????? ????" );
	this->datelabel->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->datelabel->AutoRealign();
	this->datelabel->setClickable( false );	// Pass click to the parent

}

void TlDateTime::updateTime( void ){
	RTC_Date now = ttgo->rtc->getDateTime();
	uint32_t wday = ttgo->rtc->getDayOfWeek( now.day, now.month, now.year );
	char buf[64];

	sprintf( buf, "%02u:%02u:%02u", now.hour, now.minute, now.second );
	this->timelabel->setText( buf );

	if(now.day != this->daynum){
			// French translation tables
		const char *wds[] = { "Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam" };
		const char *mths[] = {
			"Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
			"Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"
		};

		this->daynum = now.day;
		sprintf( buf, "%s %d %s %d", wds[wday], now.day, mths[now.month-1], now.year );
		this->datelabel->setText( buf );
	}
}

static void cbUpdTime( lv_task_t *tsk ){
	((TlDateTime *)(tsk->user_data))->updateTime();
}

void TlDateTime::initAutomation( void ){
	this->upd_h_task = lv_task_create( cbUpdTime, 500, LV_TASK_PRIO_MID, this );
}
