/************************************************
*	Popup abstraction
 ************************************************/

#include "DWPopup.h"
#include "Gui.h"

static void closePopup( lv_obj_t *, lv_event_t event ){
	if( event == LV_EVENT_CLICKED )
		gui->closePopup();
}

DWPopup::DWPopup( lv_obj_t *parent, enum Kind k ) :
	Container( parent ), kind( k )
{
	this->setSize( lv_obj_get_width( parent ), lv_obj_get_height( parent ) );

		// Close this popup
	this->setClickable( true );
	this->attacheEventeHandler( closePopup );
Serial.println("DWPopup()");

	switch( this->kind ){
	case SALON :
	case JARDIN :
		this->popup.temperature = new TemperatureChart( this->getMyself(), "Salon", "TSalon" );
		break;
	}
	
}

DWPopup::~DWPopup(){
	switch( this->kind ){
	case SALON :
	case JARDIN :
		delete this->popup.temperature;
		break;
	}
}

bool DWPopup::msgreceived( const char *topic, const char *payload ){
		switch( this->kind ){
	case SALON :
	case JARDIN :
		return this->popup.temperature->msgreceived( topic, payload );
		break;
	}

	return false;
}
