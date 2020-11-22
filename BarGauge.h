/************************************************
*	BarGauge : kind of linear gauge
*************************************************/

#ifndef BAR_H
#define BAR_H

#include "Container.h"

class BarGauge : public GfxObject {
public:
	/* BarGauge constructor
	 * -> Container *parent : parent object (default : NULL)
	 *  	if not null, its style is copied
	 * -> Container *cloned : copy from this object (default : NULL)
	 * -> int16_t min,max : min and maximum value (default 0, 100)
	 */
	BarGauge( Container *parent=NULL, Container *cloned=NULL, int16_t min=0, int16_t max=100 ){
		this->_obj = lv_bar_create( 
			parent ? parent->getMyself() : NULL, 
			cloned ? cloned->getMyself() : NULL
		);

		if(parent){
			this->copyStyle( parent->getStyle() );
			this->applyStyle();
		}

		lv_bar_set_range( this->_obj, min, max );
	}

	/* Set minimum and the maximum values of a bar
	 * int16_t min,max : extreme values
	 */
	void setRange( int16_t min, int16_t max ){
		lv_bar_set_range( this->_obj, min, max );
	}

	/* Set the value
	 * -> int16_t value : new value
	 * -> lv_anim_enable_t anim : LV_ANIM_ON (default) / LV_ANIM_OFF
	 */
	void setValue( int16_t val, lv_anim_enable_t anim=LV_ANIM_ON ){
		lv_bar_set_value( this->_obj, val, anim );
	}

	/* Set the anim time
	 * -> uint16_t time in ms
	 */
	void setAnimTime( uint16_t time ){
		lv_bar_set_anim_time( this->_obj, time );
	}
};

#endif
