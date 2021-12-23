/************************************************
*	Screen's settings tile
*************************************************/

#include "Gui.h"
#include "TlScreen.h"

	/*****
	 * Brighness related
	 *****/

static void brightnessModified( lv_obj_t *obj, lv_event_t event ){
	if(event == LV_EVENT_VALUE_CHANGED){
		static char buff[5];
		Slider::refreshKnob(obj, buff, 5, "%3u");
		Serial.printf("BackLight changed to %d\n", lv_slider_get_value(obj));
		ttgo->bl->adjust( bl_lev = lv_slider_get_value(obj) );
	}
}

LV_IMG_DECLARE(brightness_32px);


	/******
	 * Savers related
	 ******/

LV_IMG_DECLARE(time_32px);
LV_IMG_DECLARE(wifi_32px);
LV_FONT_DECLARE(Ubuntu_16px);

static void saverModified( lv_obj_t *obj, lv_event_t event ){
	if(event == LV_EVENT_VALUE_CHANGED){
		static char buff[5];
		Slider::refreshKnob(obj, buff, 5, "%2u");
		inactive_counter = lv_slider_get_value(obj) * 1000;

		Serial.printf("Saver changed to %d\n", lv_slider_get_value(obj));
	}
}

static void wifiModified( lv_obj_t *obj, lv_event_t event ){
	if(event == LV_EVENT_VALUE_CHANGED){
		static char buff[5];
		Slider::refreshKnob(obj, buff, 5, "%2u");
		inactive_wifi_counter = lv_slider_get_value(obj) * 1000;

		Serial.printf("WiFi Saver changed to %d\n", lv_slider_get_value(obj));
	}
}

	/*********
	 * Wakeup related
	 *********/

static void mvtWakeupChanged( lv_obj_t *obj, lv_event_t event ){
	if(event == LV_EVENT_VALUE_CHANGED){
		mvtWakeup = lv_checkbox_is_checked( obj );
		Serial.println( mvtWakeup ? "Movement wakeup activated" : "Movement wakeup disabled");
	}
}


TlScreen::TlScreen( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned )
{
		/* We need from the bottom : this tile is bellow the main one so we need
		 * some free space on its top as dragging area
		 */

		/* Accelerometer wakeup ? */
	this->wakeupFromMouvement = new Checkbox( this );
	this->wakeupFromMouvement->Align( LV_ALIGN_IN_BOTTOM_LEFT );
	this->wakeupFromMouvement->setText( "Wakeup by movement");
	this->wakeupFromMouvement->setChecked( mvtWakeup );
	this->wakeupFromMouvement->attachEventHandler( mvtWakeupChanged );

		/* Saver when WiFi enabled */
	this->wifiCont = new Container( mainStyle, this->getMyself() );
	this->wifiCont->setSize( this->getWidth(), 40 );
	this->wifiCont->Align( LV_ALIGN_OUT_TOP_LEFT, this->wakeupFromMouvement );
	this->wifiCont->setClickable( false );	// Pass click to the parent

	this->wifiIcon = new Image( this->wifiCont );
	this->wifiIcon->Set( &wifi_32px );
	this->wifiIcon->setClickable( false );
	this->wifiIcon->Align( LV_ALIGN_IN_LEFT_MID );

	this->wifiSlider = new Slider( sliderStyle, this->wifiCont, NULL, 10,75, true );
	this->wifiSlider->setSize( parent->getWidth() - this->wifiIcon->getWidth() -30, 10 );
	this->wifiSlider->Align( LV_ALIGN_OUT_RIGHT_MID, this->wifiIcon->getMyself(), 20 );
	this->wifiSlider->setValue( inactive_wifi_counter/1000 );	// Set initial value
	this->wifiSlider->attachEventHandler( wifiModified );
	wifiModified( this->wifiSlider->getMyself(), LV_EVENT_VALUE_CHANGED );

		/* Saver */
	this->saverCont = new Container( mainStyle, this->getMyself() );
	this->saverCont->setSize( this->getWidth(), 40 );
	this->saverCont->Align( LV_ALIGN_OUT_TOP_LEFT, this->wifiCont );
	this->saverCont->setClickable( false );	// Pass click to the parent

	this->saverIcon = new Image( this->saverCont );
	this->saverIcon->Set( &time_32px );
	this->saverIcon->setClickable( false );
	this->saverIcon->Align( LV_ALIGN_IN_LEFT_MID );

	this->saverSlider = new Slider( sliderStyle, this->saverCont, NULL, 10,75, true );
	this->saverSlider->setSize( parent->getWidth() - this->saverIcon->getWidth() -30, 10 );
	this->saverSlider->Align( LV_ALIGN_OUT_RIGHT_MID, this->saverIcon->getMyself(), 20 );
	this->saverSlider->setValue( inactive_counter/1000 );	// Set initial value
	this->saverSlider->attachEventHandler( saverModified );
	saverModified( this->saverSlider->getMyself(), LV_EVENT_VALUE_CHANGED );

		/* Brightness */
	this->brightnessCont = new Container( mainStyle, this->getMyself() );
	this->brightnessCont->setSize( this->getWidth(), 40 );
	this->brightnessCont->Align( LV_ALIGN_OUT_TOP_LEFT, this->saverCont );
	this->brightnessCont->setClickable( false );	// Pass click to the parent

	this->brightnessIcon = new Image( this->brightnessCont );
	this->brightnessIcon->Set( &brightness_32px );
	this->brightnessIcon->setClickable( false );
	this->brightnessIcon->Align( LV_ALIGN_IN_TOP_LEFT );

	this->brightnessSlider = new Slider( sliderStyle, this->brightnessCont, NULL, 15,255, true );
	this->brightnessSlider->Align( LV_ALIGN_OUT_RIGHT_MID, this->brightnessIcon->getMyself(), 20 );
	this->brightnessSlider->setSize( parent->getWidth() - this->brightnessIcon->getWidth() -30, 10 );
	this->brightnessSlider->setValue( ttgo->bl->getLevel() );
	this->brightnessSlider->attachEventHandler( brightnessModified );
	brightnessModified( this->brightnessSlider->getMyself(), LV_EVENT_VALUE_CHANGED );

/*	Debug
	this->brightnessCont->dumpObj("brightnessCont");
	this->brightnessIcon->dumpObj("brightnessIcon");
	this->brightnessSlider->dumpObj("brightnessSlider");
	this->saverCont->dumpObj("saverCont");
	this->saverIcon->dumpObj("saverIcon");
	this->saverSlider->dumpObj("saverSlider");
*/
}
