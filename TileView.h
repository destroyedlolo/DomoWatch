/************************************************
*	Tileview : grid of tiles
*
*	Note : Tiles themselves are Container
*************************************************/

#ifndef TILEV_H
#define TILEV_H

#include "Style.h"
#include "GfxObject.h"
#include "Container.h"

class TileView : virtual public Style, virtual public GfxObject {
public:

	/* TileView constructor
	 * -> lv_obj_t *parent : parent object (default : NULL)
	 * -> const lv_obj_t *cloned : copy from this object (default : NULL)
	 */
	TileView( lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL ){
		this->_obj = lv_tileview_create( parent, cloned );
	}

	/* Add a tile
	 * -> Container *tile
	 */
	void AddTile( Container *tile ){
		lv_tileview_add_element( this->getMyself(), tile->getContainer() );
	}

	/* Set allowed movements
	 * -> lv_point_t *valid_pos : array of valid tiles position
	 * -> uint16_t valid_pos_cnt : number of entries in the array
	 *
	 * Notez-bien : Only a pointer to the array is kept. So it can't be a
	 *  local variable, must be dynamically allocated, a global variable or
	 *  static.
	 */
	void setValidPositions( lv_point_t *valid_pos, uint16_t valid_pos_cnt ){
		lv_tileview_set_valid_positions( this->getMyself(), valid_pos, valid_pos_cnt );
	}

	/* Set active tile
	 * -> lv_coord_t x, y : tile position
	 * -> lv_anim_enable_t anim : with LV_ANIM_ON or without LV_ANIM_OFF
	 */
	void setActiveTile( lv_coord_t x, lv_coord_t y, lv_anim_enable_t anim ){
		lv_tileview_set_tile_act( this->getMyself(), x,y, anim );
	}

	/* get active tile position
	 * -> lv_coord_t &x,&y : reference to variable that will hold the position
	 */
	void getActiveTile( lv_coord_t &x, lv_coord_t &y ){
		lv_tileview_get_tile_act( this->getMyself(), &x, &y );
	}

	/* Flash if an edge is reached
	 * -> bool : true flash, false don't flash
	 */
	void setEdgeFlash( bool en ){
		lv_tileview_set_edge_flash( this->getMyself(), en );
	}
};

#endif
