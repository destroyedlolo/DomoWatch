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
		Serial.printf("BackLight changed to %d\n", lv_slider_get_value(obj));
		ttgo->bl->adjust( lv_slider_get_value(obj) );
	}
}

LV_IMG_DECLARE(brightness_32px);


	/******
	 * Saver related
	 ******/

LV_IMG_DECLARE(time_32px);
LV_FONT_DECLARE(Ubuntu_16px);

/* As the label's lv_obj_t is part of another object which is not accessible for this
 * callback (or with accessor outside this file, the easiest way is to create this 
 * old style pointer.
 */
lv_obj_t 	*saverLbl;

static void saverModified( lv_obj_t *obj, lv_event_t event ){
	if(event == LV_EVENT_VALUE_CHANGED){
		char res[11];
		snprintf(res,11, "%2u seconds", lv_slider_get_value(obj));
		lv_label_set_text( saverLbl, res );

		inactive_counter = lv_slider_get_value(obj) * 1000;

		Serial.printf("Saver changed to %d\n", lv_slider_get_value(obj));
	}
}


TlScreen::TlScreen( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned )
{

		/* Brightness */
	this->brightnessCont = new Container( this );
	this->brightnessCont->setSize( this->getWidth(), 40 );
	this->brightnessCont->Align( LV_ALIGN_IN_TOP_MID );
	this->brightnessCont->setClickable( false );	// Pass click to the parent

	this->brightnessIcon = new Image( this->brightnessCont );
	this->brightnessIcon->Set( &brightness_32px );
	this->brightnessIcon->setClickable( false );
	this->brightnessIcon->Align( LV_ALIGN_IN_TOP_LEFT );

	this->brightnessSlider = new Slider( this->brightnessCont, NULL, 15,255 );
	this->brightnessSlider->Align( LV_ALIGN_OUT_RIGHT_MID, this->brightnessIcon->getMyself(), 20 );
	this->brightnessSlider->setSize( parent->getWidth() - this->brightnessIcon->getWidth() -30, 10 );
	lv_style_set_bg_color( this->brightnessSlider->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_AQUA );
	this->brightnessSlider->applyStyle();
	this->brightnessSlider->setValue( ttgo->bl->getLevel() );
	this->brightnessSlider->attacheEventeHandler( brightnessModified );

		/* Saver */
	this->saverCont = new Container( this );
	this->saverCont->setSize( this->getWidth(), 60 );
	this->saverCont->Align( LV_ALIGN_OUT_BOTTOM_MID, this->brightnessCont );
	this->saverCont->setClickable( false );	// Pass click to the parent

	this->saverIcon = new Image( this->saverCont );
	this->saverIcon->Set( &time_32px );
	this->saverIcon->setClickable( false );
	this->saverIcon->Align( LV_ALIGN_IN_LEFT_MID );

	this->saverSlider = new Slider( this->saverCont, NULL, 15,75 );
	this->saverSlider->setSize( parent->getWidth() - this->saverIcon->getWidth() -30, 10 );
	this->saverSlider->Align( LV_ALIGN_OUT_RIGHT_TOP, this->saverIcon->getMyself(), 20 );

	lv_style_set_bg_color( this->saverSlider->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_AQUA );
	this->saverSlider->applyStyle();
	this->saverSlider->setValue( inactive_counter/1000 );	// Set initial value
	this->saverSlider->attacheEventeHandler( saverModified );

	this->saverLabel = new Label( this->saverCont );
	this->saverLabel->setFont( &Ubuntu_16px );
	this->saverLabel->applyStyle();
	this->saverLabel->setText( (String( inactive_counter/1000 ) + " seconds").c_str() );
	this->saverLabel->AutoRealign();
	this->saverLabel->Align( LV_ALIGN_OUT_BOTTOM_MID, this->saverSlider->getMyself(), 20 );
	this->saverLabel->setClickable( false );	// Pass click to the parent

	saverLbl = this->saverLabel->getMyself();

/*	Debug
	this->brightnessCont->dumpObj("brightnessCont");
	this->brightnessIcon->dumpObj("brightnessIcon");
	this->brightnessSlider->dumpObj("brightnessSlider");
	this->saverCont->dumpObj("saverCont");
	this->saverIcon->dumpObj("saverIcon");
	this->saverSlider->dumpObj("saverSlider");
*/
}
