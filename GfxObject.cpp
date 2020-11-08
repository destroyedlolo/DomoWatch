/************************************************
*	Gfx object abstraction
*
*	contains methods shared by all objects
*************************************************/

#include "GfxObject.h"
#include "Container.h"

void GfxObject::Align( lv_align_t align, Container *base, lv_coord_t x_mod, lv_coord_t y_mod){
	lv_obj_align( this->getMyself(), base->getMyself(), align, x_mod, y_mod );
}
