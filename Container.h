/************************************************
*	Container : Display zone restricting other objects
*************************************************/

#ifndef CONT_H
#define CONT_H

#include "Style.h"
#include "GfxObject.h"

class TileView;

class Container : virtual public Style, virtual public GfxObject {
public:

	/* Container constructor
	 * -> lv_obj_t *parent : parent object (default : NULL)
	 * -> const lv_obj_t *cloned : copy from this object (default : NULL)
	 */
	Container( lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL ){
		this->_obj = lv_cont_create( parent, cloned );
	}

	Container( Container *parent, Container *cloned=NULL );
	Container( TileView *parent, TileView *cloned=NULL );
};

#endif
