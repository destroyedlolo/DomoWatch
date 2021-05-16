/************************************************
*	Popup abstraction
*
* Retrieve data from MQTT and then draw single serie chart
*
* Hardcoded for 50 samples
*************************************************/

#include "Gui.h"
#include "TemperatureChart.h"
#include "Network.h"

TemperatureChart::TemperatureChart( lv_obj_t *parent, const char *title, const char *topic ) :
	Chart( 50, parent )
{
	this->setSize( lv_obj_get_width( parent ), lv_obj_get_height( parent ) );
	this->addStyle( popupStyle );
	this->setCaptionString( title );
	this->setClickable( false );	// pass event to upper

	this->serie = this->addSerie( LV_COLOR_RED );

		// Subscribe to response topic
	this->repTopic = "ReponseHistorique/";
	this->repTopic += topic;
	network.MQTTsubscribe( this->repTopic.c_str() );

	String subtopic = topic;
	subtopic += ";50";
Serial.printf("-----> '%s\n", subtopic.c_str() );
	network.MQTTpublishString( "DemandeHistorique", subtopic.c_str() );
}

TemperatureChart::~TemperatureChart(){
	delete this->serie;
}

bool TemperatureChart::msgreceived( const char *topic, const char *payload ){
	if( this->repTopic == topic ){
		Serial.printf("'%s' accepted", topic);
		return true;
	}

	return false;
}
