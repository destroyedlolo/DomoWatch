/************************************************
*	Container : Display zone restricting other objects
*************************************************/

#include "Container.h"
#include "TileView.h"

Container::Container( Container *parent, Container *cloned ){
	this->_container = lv_cont_create( 
		parent->getContainer(),
		cloned ? cloned->getContainer() : NULL
	);

	if(parent)
		this->copyStyle( parent->getStyle() );
}

Container::Container( TileView *parent, TileView *cloned ){
	this->_container = lv_cont_create( 
		parent->getTileView(),
		cloned ? cloned->getTileView() : NULL
	);

	if(parent)
		this->copyStyle( parent->getStyle() );
}
