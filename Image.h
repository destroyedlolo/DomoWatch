/************************************************
*	Image
*************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#include "GfxObject.h"

class Image : public GfxObject {
	lv_obj_t *img;

public:
	lv_obj_t *getMyself( void ) { return this->img; }

	/* Container constructor
	 * -> GfxObject *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 * -> const lv_obj_t *cloned : copy from this object (default : NULL)
	 */
	Image( GfxObject *parent=NULL, const lv_obj_t *cloned=NULL ) : GfxObject(){
		this->img = lv_img_create( parent ? **parent : NULL, cloned );
	}

	/* set the image content
	 * -> const void *src : image's source
	 */
	void Set( const void *src ){
		lv_img_set_src( this->img, src );
	}

	/* recolor image's color
	 *	-> lv_color_t color : new image color
	 */
	void Recolor( lv_color_t color ){
		lv_style_set_image_recolor( this->getStyle(), LV_OBJ_PART_MAIN, color );
		this->ApplyStyle();
	}
};

#endif
