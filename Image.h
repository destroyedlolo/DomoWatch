/************************************************
*	Image
*************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#include "Container.h"

class Image : public GfxObject {

protected:
	/* internal image constructor
	 * it should be ONLY used when the object is initialised afterward
	 * like within ClKImage otherwise ... Guru
	 */
	Image(){
		this->_obj = NULL;
	}

public:
	/* Image constructor
	 * -> Container *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 * -> const Container *cloned : copy from this object (default : NULL)
	 */
	Image( lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL ) {
		this->_obj = lv_img_create( parent, cloned );
	}

	Image( Container *parent, Container *cloned=NULL ){
		this->_obj = lv_img_create( 
			parent ? parent->getMyself() : NULL, 
			cloned ? cloned->getMyself() : NULL
		);

		if(parent)
			this->copyStyle( parent->getStyle() );
	}

	/* set the image content
	 * -> const void *src : image's source
	 */
	void Set( const void *src ){
		lv_img_set_src( this->_obj, src );
	}

	/* recolor image's color
	 *	-> lv_color_t color : new image color
	 */
	void Recolor( lv_color_t color ){
		lv_style_set_image_recolor( this->getStyle(), LV_OBJ_PART_MAIN, color );
		this->applyStyle();
	}
};
#endif
