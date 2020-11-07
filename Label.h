/************************************************
*	Label : display a text
*************************************************/

#ifndef LABEL_H
#define LABEL_H

#include "Style.h"
#include "Container.h"

class Label : virtual public Style, virtual public GfxObject {
	lv_obj_t *_label;

public:
	lv_obj_t *getMyself( void ) { return this->_label; }
	lv_obj_t *getLabel( void ) { return this->_label; }

	/* Container constructor
	 * -> Container *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 * -> const lv_obj_t *cloned : copy from this object (default : NULL)
	 */
	Label( Container *parent=NULL, Container *cloned=NULL ){
		this->_label = lv_label_create( 
			parent ? parent->getMyself() : NULL, 
			cloned ? cloned->getMyself() : NULL
		);

		if(parent)
			this->copyStyle( parent->getStyle() );
	}

		/* Set text
		 * -> const char *text
		 */
	void setText( const char *text ){
		lv_label_set_text( this->getMyself(), text );
	}

};

#endif
