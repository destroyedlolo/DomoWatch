/************************************************
*	Settings tile
*************************************************/

#include "TlSettings.h"
#include "font.h"

LV_FONT_DECLARE(Ubuntu_48px);

TlSettings::TlSettings( lv_obj_t *parent, const lv_obj_t *cloned ) : 
	Container( parent, cloned )
{
		/* Display time */
	this->label = new Label( this );
	this->label->setFont( &Ubuntu_48px );
	this->label->setText( "Prout" );
	this->label->Align( LV_ALIGN_IN_TOP_MID );
	this->label->AutoRealign();

			/* and date */
/*
	this->datelabel = new Label( this );
	this->datelabel->SetFont( &Ubuntu_16px );
	this->datelabel->SetText( "??.???? ????" );
	this->datelabel->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->datelabel->AutoRealign();
*/
}
