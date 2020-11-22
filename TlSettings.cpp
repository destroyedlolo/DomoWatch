/************************************************
*	Settings tile
*************************************************/

#include "Gui.h"
#include "TlSettings.h"

	/*****
	 * Brighness related
	 *****/

static void brightnessModified( lv_obj_t *obj, lv_event_t event ){
	if(event == LV_EVENT_VALUE_CHANGED){
		Serial.printf("BackLight changed to %d\n", lv_slider_get_value(obj));
		ttgo->bl->adjust( lv_slider_get_value(obj) );
	}
}

LV_IMG_DECLARE(brightness_32px);

TlSettings::TlSettings( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned )
{
	this->brightnessIcon = new Image( this );
	this->brightnessIcon->Set( &brightness_32px );
	this->brightnessIcon->setClickable( false );
	this->brightnessIcon->Align( LV_ALIGN_IN_TOP_LEFT );

	this->brightnessSlider = new Slider( this, NULL, 15,255 );
	this->brightnessSlider->Align( LV_ALIGN_OUT_RIGHT_MID, this->brightnessIcon->getMyself(), 20 );
	this->brightnessSlider->setSize( parent->getWidth() - this->brightnessIcon->getWidth() -30, 16 );
	lv_style_set_bg_color( this->brightnessSlider->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_AQUA );
	this->brightnessSlider->applyStyle();
	this->brightnessSlider->attacheEventeHandler( brightnessModified );
}
