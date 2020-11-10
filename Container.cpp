/************************************************
*	Container : Display zone restricting other objects
*************************************************/

#include "Container.h"
#include "TileView.h"

Container::Container( Container *parent, Container *cloned ){
	this->_obj = lv_cont_create( 
		parent->getMyself(),
		cloned ? cloned->getMyself() : NULL
	);

	if(parent)
		this->copyStyle( parent->getStyle() );
}

Container::Container( TileView *parent, TileView *cloned ){
	this->_obj = lv_cont_create( 
		parent->getMyself(),
		cloned ? cloned->getMyself() : NULL
	);

	if(parent)
		this->copyStyle( parent->getStyle() );
}
