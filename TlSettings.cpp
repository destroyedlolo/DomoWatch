/************************************************
*	Settings' tile
*************************************************/

#include "TlSettings.h"
#include "Version.h"
#include "myfont.h"

LV_FONT_DECLARE(Ubuntu_16px);

TlSettings::TlSettings( Gui *g ) : Tile(g) {

	/* display application version */
	this->version = new Label( this );
	this->version->SetFont( &Ubuntu_16px );
	this->version->SetText( "DomoWatch " VERSION_H );
	this->version->Align( LV_ALIGN_IN_BOTTOM_MID );
}
