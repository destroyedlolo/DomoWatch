/************************************************
*	Container : Display zone restricting other objects
*************************************************/

#ifndef CONT_H
#define CONT_H

#include "GfxObject.h"

class Container : virtual public Style {
	lv_obj_t 	*_container;

public:
	virtual lv_obj_t *getMyself( void ) { return this->_container; }
	lv_obj_t *getContainer( void ) { return this->_container; }

	/* Container constructor
	 * -> lv_obj_t *parent : parent object (default : NULL)
	 * -> const lv_obj_t *cloned : copy from this object (default : NULL)
	 */
	Container( lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL ){
		this->_container = lv_obj_create( parent, cloned );
	}

	/* Add an external style to an object
	 * -> lv_style_t *style : new style to be added
	 * -> uint8_t part : which part to update (LV_OBJ_PART_MAIN)
	 */
	void AddStyle( lv_style_t *style, uint8_t part=LV_OBJ_PART_MAIN ){
		lv_obj_add_style( this->getMyself(), part, style );
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

};

#endif
