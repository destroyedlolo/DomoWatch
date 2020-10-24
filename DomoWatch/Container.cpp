/************************************************
*	Container : Display zone restricting other objects
*************************************************/

#include "Container.h"

Container::Container( lv_obj_t *parent, const lv_obj_t *cloned ){
	this->container = lv_obj_create( lv_scr_act(), NULL );
}
