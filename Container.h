/************************************************
*	Container : Display zone restricting other objects
*************************************************/

#ifndef CONT_H
#define CONT_H

#include <Arduino.h>

#include "GfxObject.h"

class TileView;

class Container : public GfxObject {
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

	/*	set position as a tile
	 * 	lv_point_t pos
	 */
	void setTilePos( lv_point_t pos ){
		this->setPosXY( pos.x * LV_HOR_RES, pos.y * LV_VER_RES );
	}
};

#endif
