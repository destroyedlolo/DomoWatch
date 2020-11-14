/************************************************
*	Label : display a text
*************************************************/

#ifndef LABEL_H
#define LABEL_H

#include "Container.h"

class Label : public GfxObject {
public:
	/* Container constructor
	 * -> Container *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 * -> const lv_obj_t *cloned : copy from this object (default : NULL)
	 */
	Label( Container *parent=NULL, Container *cloned=NULL ){
		this->_obj = lv_label_create( 
			parent ? parent->getMyself() : NULL, 
			cloned ? cloned->getMyself() : NULL
		);

		if(parent){
			this->copyStyle( parent->getStyle() );
			this->applyStyle();
		}
	}

		/* Set text
		 * -> const char *text
		 */
	void setText( const char *text ){
		lv_label_set_text( this->getMyself(), text );
	}

};
#endif
