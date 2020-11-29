/************************************************
*	Clickable Button
*
*	from https://github.com/lvgl/lvgl/issues/119#issuecomment-367611954
*************************************************/

#ifndef BUTTON_H
#define BUTTON_H

#include "Container.h"

class StatusBar;

class Button : public Container {
public:
	/* Button constructor 
	 *	-> Container *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 */
	Button( Container *parent, Container *cloned=NULL );
	Button( StatusBar *parent, StatusBar *cloned=NULL );

	/* Set layout policy
	 * lv_layout_t layout
	 */
	void setLayout( lv_layout_t layout ){
		lv_btn_set_layout( this->getMyself(), layout );
	}

	/* Set the fit policy
	 *	-> lv_fit_t all : policy in all directions
	 *	-> lv_fit_t hor,ver
	 *	-> lv_fit_t left, right, top, bottom
	 */
	void setFit( lv_fit_t all ){
		lv_btn_set_fit( this->getMyself(), all );
	}
	void setFit( lv_fit_t hor, lv_fit_t ver ){
		lv_btn_set_fit2( this->getMyself(), hor, ver );
	}
	void setFit( lv_fit_t left, lv_fit_t right, lv_fit_t top, lv_fit_t bottom ){
		lv_btn_set_fit4( this->getMyself(), left, right, top, bottom );
	}

	/* get button state
	 */
	lv_btn_state_t getState( void ){
		return lv_btn_get_state( this->getMyself() );
	}

	/* Enable the toggled states
	 * -> bool toggleable
	 */
	void setCHeckable( bool tgl ){
		lv_btn_set_checkable( this->getMyself(), tgl );
	}
};
#endif
