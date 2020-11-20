/************************************************
*	MsgBox : Pop-up with message and buttons
*************************************************/

#ifndef MSGBOX_H
#define MSGBOX_H

#include "Container.h"

class MsgBox : public GfxObject {
public:
	/* MessageBox constructor
	 * -> Container *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 * -> const Container *cloned : copy from this object (default : NULL)
	 */
	MsgBox( Container *parent, Container *cloned=NULL ){
		this->_obj = lv_img_create( 
			parent ? parent->getMyself() : NULL, 
			cloned ? cloned->getMyself() : NULL
		);

		if(parent)
			this->copyStyle( parent->getStyle() );
	}

	/* Set MessageBox's text
	 * -> const char *text - what to display.
	 *
	 *  Notez-bien : the text will be copied locally, so the parameter can be 
	 *  a local variable.
	 */
	void setText( const char *text ){
		lv_msgbox_set_text( this->getMyself(), text );
	}

	/* Init the message box
	 * -> const char *text - text to be displayed
	 * -> const char *btns[] - static array of buttons labels. 
	 *  Last one is an empty string
	 * -> uint16_t anim - animation duration (optional)
	 */
	void init( const char *text, const char *btns[], uint16_t anim = (uint16_t)-1 ){
		lv_msgbox_set_text( this->getMyself(), text );
		lv_msgbox_add_btns( this->getMyself(), btns );
		if( anim != (uint16_t)-1 )
			lv_msgbox_set_anim_time( this->getMyself(), anim );
	}

	/* get lastly activated button ID
	 *	<- uint16_t : button's ID (starting with 0)
	 */
	uint16_t getActiveButton( void ){
		return lv_msgbox_get_active_btn( this->getMyself() );
	}

	/* get lastly activated button ID
	 *	<- const char * : button's text
	 */
	const char *getActiveButtonText( void ){
		return lv_msgbox_get_active_btn_text( this->getMyself() );
	}
};

#endif
