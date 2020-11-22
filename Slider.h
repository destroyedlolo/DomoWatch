/************************************************
 * Slider
*************************************************/

#ifndef SLDR_H
#define SLDR_H

class Slider : public GfxObject {
public:
	/* Slider constructor
	 * -> Container *parent : parent object (default : NULL)
	 * -> Container *cloned : copy from this object (default : NULL)
	 * -> int16_t min,max : min and maximum value (default 0, 100)
	 *
	 *  Notez-bien : the parent style is not copied, as container's one
	 *  are more annoying than useful.
	 */
	Slider( Container *parent=NULL, Container *cloned=NULL, int16_t min=0, int16_t max=100 ){
		this->_obj = lv_slider_create( 
			parent ? parent->getMyself() : NULL, 
			cloned ? cloned->getMyself() : NULL
		);

		lv_slider_set_range( this->_obj, min, max );
	}

	/* Set minimum and the maximum values of the slider
	 * int16_t min,max : extreme values
	 */
	void setRange( int16_t min, int16_t max ){
		lv_slider_set_range( this->_obj, min, max );
	}

	/* Set the value
	 * -> int16_t value : new value
	 * -> lv_anim_enable_t anim : LV_ANIM_ON (default) / LV_ANIM_OFF
	 */
	void setValue( int16_t val, lv_anim_enable_t anim=LV_ANIM_ON ){
		lv_slider_set_value( this->_obj, val, anim );
	}

	/* Get slider value
	 * <-  int16_t
	 */
	int16_t getValue( void ){
		return lv_slider_get_value( this->_obj );
	}

	/* Set the anim time
	 * -> uint16_t time in ms
	 */
	void setAnimTime( uint16_t time ){
		lv_slider_set_anim_time( this->_obj, time );
	}

	/* Set the slider type
	 * -> lv_slider_type_t type
	 */
	void setType( lv_slider_type_t type ){
		lv_slider_set_type( this->_obj, type );
	}
};

#endif
