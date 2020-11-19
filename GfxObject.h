/************************************************
*	Gfx object abstraction
*
*	contains methods shared by all objects
*************************************************/
#ifndef GFXOBJ_H
#define GFXOBJ_H

#include <lvgl/lvgl.h>

#include "Style.h"

class Container;
class Label;

class GfxObject : public Style {
protected:
	lv_obj_t 	*_obj;

public:
	virtual lv_obj_t *getMyself( void ){ return this->_obj; }

	/***
	 * Position related
	 *
	 * Notez-bien : positions are _ALWAYS_ related to object's parent
	 ***/

	/* set position
	 * 	lv_coord_t x,y
	 */
	void setX( lv_coord_t x ){
		lv_obj_set_x( this->getMyself(), x );
	}

	void setY( lv_coord_t y ){
		lv_obj_set_y( this->getMyself(), y );
	}

	void setPosXY( lv_coord_t x, lv_coord_t y ){
		lv_obj_set_pos( this->getMyself(), x, y );
	}
	
	/* get position
	 */
	lv_coord_t getX( void ){
		return lv_obj_get_x( this->getMyself() );
	}

	lv_coord_t getY( void ){
		return lv_obj_get_y( this->getMyself() );
	}

	/***
	 * Size related
	 ***/

	/* Get object's width
	 * <- lv_coord_t : width
	 */
	lv_coord_t getWidth( void ){
		return lv_obj_get_width( this->getMyself() );
	}

	/* Get object's height
	 * <- lv_coord_t : height
	 */
	lv_coord_t getHeight( void ){
		return lv_obj_get_height( this->getMyself() );
	}

	/* Set object's size
	 * -> lv_coord_t width
	 * -> lv_coord_t height
	 */
	void setSize( lv_coord_t width, lv_coord_t height ){
		lv_obj_set_size( this->getMyself(), width, height );
	}

	void setSize( GfxObject *source ){
		lv_obj_set_size(
			this->getMyself(),
			source->getWidth(), source->getHeight()
		);
	}

	void setWidth( lv_coord_t width ){
		lv_obj_set_width( this->getMyself(), width );
	}

	void setHeight( lv_coord_t height ){
		lv_obj_set_height( this->getMyself(), height );
	}

	/***
	 * Align related
	 ***/

	/* Set Align attribut
	 * -> lv_align_t align : type of alignment
	 * -> const lv_obj_t *base : Pointer to object to align too (default: NULL)
	 * -> lv_coord_t x_mod, y_mod : offsets (default: 0)
	 */
	void Align( lv_align_t align, const lv_obj_t *base=NULL, lv_coord_t x_mod=0, lv_coord_t y_mod=0){
		lv_obj_align( this->getMyself(), base, align, x_mod, y_mod );
	}
	void Align( lv_align_t align, Container *base, lv_coord_t x_mod=0, lv_coord_t y_mod=0);
	void Align( lv_align_t align, Label *base, lv_coord_t x_mod=0, lv_coord_t y_mod=0);

	/* Set Auto-Realign attribut
	 * -> bool activate : if true activate, desactivate otherwise (default: true)
	 */
	void AutoRealign( bool activate=true ){
		lv_obj_set_auto_realign( this->getMyself(), activate );
	}


	/***
	 * Style related
	 ***/

	/* Apply localy stored style
	 * -> uint8_t part : which part to update (LV_OBJ_PART_MAIN)
	 *
	 * NOTEZ-BIEN :
	 * 		Ignored if the derived class' getMyself() returns
	 *		something different than NULL
	 */
	void applyStyle( uint8_t part=LV_OBJ_PART_MAIN ){
		if( this->getMyself() )
			lv_obj_add_style( this->getMyself(), part, &this->_style );
	}


	/***
	 * Misc
	 ***/

	/* Enable or disable the clicking of an object
	 * if disabled, clicking events are transfered to parent
	 */
	void setClickable( bool en ){
		lv_obj_set_click( this->getMyself(), en );
	}

	/* Dump object value
	 * Output on the serial console
	 */
	void dumpObj( const char *name=NULL );
};
#endif
