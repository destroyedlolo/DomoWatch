/************************************************
*	Checkbox
*************************************************/

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "Container.h"

class Checkbox : public Container {
	/* Checkbox constructor 
	 *	-> Container *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 */
	Checkbox( Container *parent, Container *cloned=NULL );

		/* Set text
		 * -> const char *text
		 * -> bool static : text hasn't to be locally copied
		 *  	(so the text MUST exist as long as this checkbox)
		 */
	void setText( const char *text, bool istatic=false );

		/* Set the state of the check box
		 * -> bool checked
		 */
	void setChecked( bool checked ){
		lv_checkbox_set_checked( this->getMyself(), checked );
	}

	bool getChecked( void ){
		return lv_checkbox_is_checked( this->getMyself() );
	}

		/* Set the state of a check box
		 * -> lv_btn_state_t state
		 */
	void setState( lv_btn_state_t state ){
		lv_checkbox_set_state( this->getMyself(), state );
	}

	lv_btn_state_t getState( void ){
		return lv_checkbox_get_state( this->getMyself() );
	}

};

#endif
