/************************************************
*	Label : display a text
*************************************************/

#ifndef LABEL_H
#define LABEL_H

#include "GfxObject.h"

class Label : public GfxObject {
	lv_obj_t *label;

public:
	lv_obj_t *getMyself( void ) { return this->label; }

	/* Container constructor
	 * -> GfxObject *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 * -> const lv_obj_t *cloned : copy from this object (default : NULL)
	 */
	Label( GfxObject *parent=NULL, const lv_obj_t *cloned=NULL ){
		this->label = lv_label_create( parent ? **parent : NULL, cloned );
		if(parent)
			this->CopyStyle( parent->getStyle() );
	}

		/* Set text
		 * -> const char *text
		 */
	void SetText( const char *text ){
		lv_label_set_text( this->getMyself(), text );
	}
};

#endif
