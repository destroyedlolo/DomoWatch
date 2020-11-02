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

TlDateHour::TlDateHour( Gui *g ) :
	Tile( g ),
	gui( g ), 
	prev_day(-1)
{

		/* Update the tile to align labels in
		 * the middle of the screen
		 */
	this->SetSize( lv_disp_get_hor_res( NULL ) , lv_disp_get_ver_res( NULL ) / 3 );
	this->Align( LV_ALIGN_CENTER );
	this->CopyStyle( gui->getStyle() );	// Copy gui style to tile's local one & apply it


		/* Display time */
	this->timelabel = new Label( this );
	this->timelabel->SetFont( &Ubuntu_48px );
	this->timelabel->SetText( "??:??" );
	this->timelabel->Align( LV_ALIGN_IN_TOP_MID );
	this->timelabel->AutoRealign();

		/* and date */
	this->datelabel = new Label( this );
	this->datelabel->SetFont( &Ubuntu_16px );
	this->datelabel->SetText( "??.???? ????" );
	this->datelabel->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->datelabel->AutoRealign();

	Serial.println("TimeDate created");
}

void TlDateHour::updateTime( void ){
	time_t now;
	struct tm  info;
	char buf[64];

	time( &now );
	localtime_r( &now, &info );

	strftime( buf, sizeof(buf), "%H:%M:%S", &info );
	this->timelabel->SetText( buf );

	if(info.tm_yday != this->prev_day){
		this->prev_day = info.tm_yday;
		strftime( buf, sizeof(buf), "%a %d.%b %Y", &info );
		this->datelabel->SetText( buf );

		Serial.println("Date updated");
	}
}

void TlDateHour::initAutomation( void (*func)(lv_task_t *) ){
	this->upd_h_task = lv_task_create( func, 500, LV_TASK_PRIO_MID, NULL );
}
