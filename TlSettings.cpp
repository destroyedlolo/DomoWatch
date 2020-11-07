/************************************************
*	Settings tile
*************************************************/

#include "TlSettings.h"
#include "Version.h"
#include "font.h"

LV_FONT_DECLARE(Ubuntu_16px);

TlSettings::TlSettings( lv_obj_t *parent, const lv_obj_t *cloned ) : 
	Container( parent, cloned )
{
		/* Display version */
	this->_version = new Label( this );
	this->_version->setFont( &Ubuntu_16px );
	this->_version->setText( "DomoWatch " VERSION_H );
	this->_version->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->_version->AutoRealign();

}
