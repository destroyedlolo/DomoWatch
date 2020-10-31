/************************************************
*	Gfx object abstraction
*************************************************/
#ifndef GFXOBJ_H
#define GFXOBJ_H

#include <lvgl/lvgl.h>

class GfxObject {
	lv_style_t	localstyle;	// if a local style is needed

protected:
	virtual lv_obj_t *getMyself( void ) = 0;

public:
	lv_obj_t * operator*( void ){ return this->getMyself(); }

	GfxObject(){
		lv_style_init(&this->localstyle);	// Avoid crash on uninitialised objects
	}
	
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
		 * -> bool activate : if true activate, desactivate otherwise (default: true)
		 */
	void AutoRealign( bool activate=true ){
		lv_obj_set_auto_realign( this->getMyself(), activate );
	}

		/* Copy a style to local one
		 * -> lv_style_t *style : original style to copy from
		 * -> bool apply : if true (default) apply immediately
		 */
	void CopyStyle( lv_style_t *style, bool apply=true ){
		lv_style_copy( &(this->localstyle), style );
		if(apply)
			this->ApplyStyle();
	}

		/* Add an external style
		 * -> lv_style_t *style : new style to be added
		 * -> uint8_t part : which part to update (LV_OBJ_PART_MAIN)
		 */
	void AddStyle( lv_style_t *style, uint8_t part=LV_OBJ_PART_MAIN ){
		lv_obj_add_style( this->getMyself(), part, style);
	}

		/* Apply localy stored style
		 * -> uint8_t part : which part to update (LV_OBJ_PART_MAIN)
		 */
	void ApplyStyle( uint8_t part=LV_OBJ_PART_MAIN ){
		lv_obj_add_style( this->getMyself(), part, &this->localstyle );
	}

		/* Get a pointer to the local style
		 */
	lv_style_t *getStyle( void ){
		return( &this->localstyle );
	}

		/* Set text font
		 * -> lv_font_t *font : font to use
		 * -> int state (default : LV_STATE_DEFAULT)
		 */
	void SetFont( lv_font_t *font, int state=LV_STATE_DEFAULT ){
		lv_style_set_text_font( &this->localstyle, state, font );
	}
};

#endif
