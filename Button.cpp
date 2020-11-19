/************************************************
*	Clickable Button
*************************************************/

#include "Button.h"
#include "StatusBar.h"

Button::Button( Container *parent, Container *cloned ){
	this->_obj = lv_btn_create( 
		parent->getMyself(),
		cloned ? cloned->getMyself() : NULL
	);

	if(parent){
		this->copyStyle( parent->getStyle() );
		this->applyStyle();
	}
}

Button::Button( StatusBar *parent, StatusBar *cloned ){
	this->_obj = lv_btn_create( 
		parent->getMyself(),
		cloned ? cloned->getMyself() : NULL
	);

	if(parent){
		this->copyStyle( parent->getStyle() );
		this->applyStyle();
	}
}
