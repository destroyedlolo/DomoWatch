/************************************************
*	Style : Style container
*************************************************/

#ifndef STYLE_H
#define STYLE_H

#include <lvgl/lvgl.h>

class Style {
	lv_style_t	_style;

protected:
	/* Accessor to the object that is needing this style.
	 * Have to be overloaded on derived classes
	 */
	virtual lv_obj_t *getMyself( void ) = 0;

public:

	/* Style constructor
	 * initialize an empty style
	 */
	Style(){
		lv_style_init( &this->_style );
	}

	/* Get a pointer to the local style
	 */
	lv_style_t *getStyle( void ){
		return( &this->_style );
	}

	/* Copy a style to local one
	 * -> lv_style_t *style : original style to copy from
	 * -> bool apply : if true (default) apply immediately
	 *
	 * NOTEZ-BIEN :
	 *  	apply is ignored if the derived class' getMyself() returns
	 *  	something different than NULL
	 */
	void CopyStyle( lv_style_t *style, bool apply=true ){
		lv_style_copy( &(this->_style), style );
		if( apply && this->getMyself() )
			this->ApplyStyle();
	}

	/* Apply localy stored style
	 * -> uint8_t part : which part to update (LV_OBJ_PART_MAIN)
	 *
	 * NOTEZ-BIEN :
	 * 		Ignored if the derived class' getMyself() returns
	 *		something different than NULL
	 */
	void ApplyStyle( uint8_t part=LV_OBJ_PART_MAIN ){
		if( this->getMyself() )
			lv_obj_add_style( this->getMyself(), part, &this->_style );
	}

		/* Set text font
		 * -> lv_font_t *font : font to use
		 * -> int state (default : LV_STATE_DEFAULT)
		 */
	void SetFont( lv_font_t *font, int state=LV_STATE_DEFAULT ){
		lv_style_set_text_font( &this->_style, state, font );
	}
};

#endif
