/************************************************
*	Main tile : Hour and date
*************************************************/

#include "TlDateTime.h"
#include "font.h"

	/****
	 * Fonts 
	 ****
	 * Need to be defined to be used
	 ****/

LV_FONT_DECLARE(Ubuntu_48px);
LV_FONT_DECLARE(Ubuntu_16px);

TlDateTime::TlDateTime( lv_obj_t *parent, const lv_obj_t *cloned ) : 
	Container( parent, cloned ),
	daynum( -1 )
{
		/* Display time */
	this->timelabel = new Label( this );
	this->timelabel->setFont( &Ubuntu_48px );
	this->timelabel->setText( "??:??" );
	this->timelabel->Align( LV_ALIGN_IN_TOP_MID );
	this->timelabel->AutoRealign();

			/* and date */
/*
	this->datelabel = new Label( this );
	this->datelabel->SetFont( &Ubuntu_16px );
	this->datelabel->SetText( "??.???? ????" );
	this->datelabel->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->datelabel->AutoRealign();
*/
}
