/************************************************
*	GUI's tile
*
*	The interface is built around LVGL's tileview
*	Tile is a particular "page" to be displayed
*************************************************/

#include "Tile.h"
#include "Gui.h"

/* Create a tile
 * -> tileview : list of tiles to add too
 */
Tile::Tile(Gui *tileview){
	this->tile = lv_obj_create(**tileview, NULL);
	this->SetSize( LV_HOR_RES, LV_VER_RES );
	this->CopyStyle( tileview->getStyle() );
	lv_tileview_add_element(**tileview, this->tile);

	Serial.printf("Tile %dx%d created\n", LV_HOR_RES, LV_VER_RES);
}
