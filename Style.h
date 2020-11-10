/************************************************
*	Style : Style container
*************************************************/

#ifndef STYLE_H
#define STYLE_H

#include <lvgl/lvgl.h>

class Style {
	lv_style_t	_style;

protected:

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
	 */
	void copyStyle( lv_style_t *style, bool apply=true ){
		lv_style_copy( &(this->_style), style );
	}

};

#endif
