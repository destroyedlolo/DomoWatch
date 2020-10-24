/************************************************
*	Gfx object abstraction
*************************************************/
#ifndef GFXOBJ_H
#define GFXOBJ_H

#include <lvgl/lvgl.h>

class GfxObject {
protected:
	virtual lv_obj_t *getMyself( void ) = 0;

public:
		/* Set object's size
		 * -> lv_coord_t width
		 * -> lv_coord_t height
		 */
	void SetSize( lv_coord_t width, lv_coord_t height ){
		lv_obj_set_size( this->getMyself(), width, height );
	}

		/* Set Align attribut
		 * -> lv_align_t align : type of alignment
		 * -> const lv_obj_t *base : Pointer to object to align too (default: NULL)
		 * -> lv_coord_t x_mod, y_mod : offsets (default: 0)
		 */
	void Align( lv_align_t align, const lv_obj_t *base=NULL, lv_coord_t x_mod=0, lv_coord_t y_mod=0){
		lv_obj_align( this->getMyself(), base, align, x_mod, y_mod );
	}

		/* Set Auto-Realign attribut
		 * -> bool activate : if true activate, desactivate otherwise
		 */
	void AutoRealign( bool activate ){
		lv_obj_set_auto_realign( this->getMyself(), activate );
	}

		/* Apply style
		 * -> uint8_t part : which part to update (LV_OBJ_PART_MAIN)
		 * -> lv_style_t *style : new style to be added
		 */
	void AddStyle( uint8_t part, lv_style_t *style ){
		lv_obj_add_style( this->getMyself(), part, style);
	}
};

#endif
