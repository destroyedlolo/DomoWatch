#include "Gui.h"
#include "TlShutter.h"

TlShutter::TlShutter( TileView *parent, TileView *cloned ) :
	Container( parent, cloned )
{
		/* Stair selector */
	this->_stair = new DropDown( this );
	this->_stair->setChoices( "Salon\nRdc\n1er", true );
	this->_stair->Align( LV_ALIGN_IN_LEFT_MID, (const lv_obj_t *)NULL, 0, -10 );

		/* Add custom style to add background */
	lv_style_set_radius( this->_stair->getStyle(), LV_OBJ_PART_MAIN, 5 );
	lv_style_set_bg_color( this->_stair->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_GRAY );
	lv_style_set_bg_opa( this->_stair->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_70 );
	lv_style_set_border_width( this->_stair->getStyle(), LV_OBJ_PART_MAIN, 1 );
	this->_stair->applyStyle();

		/* Shutter selector */
	this->_shutter = new DropDown( this );
	this->_shutter->Align( LV_ALIGN_OUT_BOTTOM_MID, this->_stair, 0, 20);
	this->_shutter->copyStyle( this->_stair->getStyle() );
	this->_shutter->applyStyle();

}
