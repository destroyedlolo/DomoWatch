/************************************************
*	Gfx object abstraction
*
*	contains methods shared by all objects
*************************************************/
#ifndef GFXOBJ_H
#define GFXOBJ_H

#include <lvgl/lvgl.h>

class Container;

class GfxObject {
protected:
	lv_obj_t 	*_obj;
	virtual lv_obj_t *getMyself( void ){ return this->_obj; }

public:
	/* Add an external style to an object
	 * -> lv_style_t *style : new style to be added
	 * -> uint8_t part : which part to update (LV_OBJ_PART_MAIN)
	 */
	void addStyle( lv_style_t *style, uint8_t part=LV_OBJ_PART_MAIN ){
		lv_obj_add_style( this->getMyself(), part, style );
	}

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

	/* Set Align attribut
	 * -> lv_align_t align : type of alignment
	 * -> const lv_obj_t *base : Pointer to object to align too (default: NULL)
	 * -> lv_coord_t x_mod, y_mod : offsets (default: 0)
	 */
	void Align( lv_align_t align, const lv_obj_t *base=NULL, lv_coord_t x_mod=0, lv_coord_t y_mod=0){
		lv_obj_align( this->getMyself(), base, align, x_mod, y_mod );
	}
	void Align( lv_align_t align, Container *base, lv_coord_t x_mod=0, lv_coord_t y_mod=0);

	/* Set Auto-Realign attribut
	 * -> bool activate : if true activate, desactivate otherwise (default: true)
	 */
	void AutoRealign( bool activate=true ){
		lv_obj_set_auto_realign( this->getMyself(), activate );
	}

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


	/* Dump object value
	 * Output on the serial console
	 */
	void dumpObj( const char *name=NULL );
};
#endif
