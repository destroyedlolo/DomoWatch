/************************************************
*	Container : Display zone restricting other objects
*************************************************/

#ifndef CONT_H
#define CONT_H

#include "GfxObject.h"

class Container : public GfxObject {
	lv_obj_t 	*container;

public:
	lv_obj_t *getMyself( void ) { return this->container; }

	/* Container constructor
	 * -> lv_obj_t *parent : parent object (default : NULL)
	 * -> const lv_obj_t *cloned : copy from this object (default : NULL)
	 */
	Container( lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL );
};

#endif
