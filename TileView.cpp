/************************************************
*	Tileview : grid of tiles
*************************************************/

#include "TileView.h"

TileView::TileView( Container *parent, Container *cloned ){
	this->_obj = lv_tileview_create( 
		parent->getMyself(),
		cloned ? cloned->getMyself() : NULL
	);

	this->setSize( parent );
	this->copyStyle( parent->getStyle() );	// Copy style from parent
	this->applyStyle();

	lv_page_set_scrlbar_mode( this->_obj, LV_SCRLBAR_MODE_OFF );	// hide scrollbar
}
