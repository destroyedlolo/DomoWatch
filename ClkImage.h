/************************************************
*	Clickable Image
*
*	from https://github.com/lvgl/lvgl/issues/119#issuecomment-367611954
*************************************************/

#ifndef CLKIMAGE_H
#define CLKIMAGE_H

#include "Image.h"

class ClKImage : public Image {
	lv_obj_t *_btn;
	lv_style_t	_transp_style;

public:
	/* ClkImage constructor 
	 * 	 * -> GfxObject *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 */
	ClKImage( Container *parent ){
		this->_btn = lv_btn_create( parent, NULL );
		lv_btn_set_fit( this->_btn, LV_FIT_TIGHT );

		lv_style_init( &this->_transp_style );
		lv_style_set_bg_opa( &this->_transp_style, LV_OBJ_PART_MAIN, LV_OPA_0 );
		lv_btn_set_style( this->_btn, LV_BTN_STYLE_REL, &this->_transp_style);
		lv_btn_set_style( this->_btn, LV_BTN_STYLE_PR, &this->_transp_style);

		this->_obj = lv_img_create( this->_btn );
		lv_obj_set_click( this->_obj, false );		// Click are passed to the button

		if(parent)	// copy the style of the parent if provided
			this->copyStyle( parent->getStyle() );
	}

};
#endif
