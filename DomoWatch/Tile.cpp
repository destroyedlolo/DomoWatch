/************************************************
*	GUI's tile
*
*	The interface is built around LVGL's tileview
*	Tile is a particular "page" to be displayed
*************************************************/

#include "Tile.h"

/* Create a tile
 * -> tileview : list of tiles to add too
 */
Tile::Tile(lv_obj_t *tileview){
	this->tile = lv_obj_create(tileview, NULL);
	lv_obj_set_size( this->tile, LV_HOR_RES, LV_VER_RES);
	lv_tileview_add_element(tileview, this->tile);
}
