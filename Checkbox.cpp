/************************************************
*	Checkbox
*************************************************/

#include "Checkbox.h"

Checkbox::Checkbox( Container *parent, Container *cloned ){
	this->_obj = lv_checkbox_create( 
		parent->getMyself(),
		cloned ? cloned->getMyself() : NULL
	);

	if(parent){
		this->copyStyle( parent->getStyle() );
		this->applyStyle();
	}
}

void Checkbox::setText( const char *text, bool istatic ){
	if( istatic )
		lv_checkbox_set_text_static( this->getMyself(), text );
	else
		lv_checkbox_set_text( this->getMyself(), text );
}
