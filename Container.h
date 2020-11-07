/************************************************
*	Container : Display zone restricting other objects
*************************************************/

#ifndef CONT_H
#define CONT_H

#include "Style.h"
#include "GfxObject.h"

class Container : virtual public Style, virtual public GfxObject {
	lv_obj_t 	*_container;

public:
	virtual lv_obj_t *getMyself( void ) { return this->_container; }
	lv_obj_t *getContainer( void ) { return this->_container; }

	/* Container constructor
	 * -> lv_obj_t *parent : parent object (default : NULL)
	 * -> const lv_obj_t *cloned : copy from this object (default : NULL)
	 */
	Container( lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL ){
		this->_container = lv_cont_create( parent, cloned );
	}

};

#endif
