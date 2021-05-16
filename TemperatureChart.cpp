/************************************************
*	Popup abstraction
*
* Retrieve data from MQTT and then draw single serie chart
*
* Hardcoded for 50 samples
*************************************************/

#include "Gui.h"
#include "TemperatureChart.h"

TemperatureChart::TemperatureChart( lv_obj_t *parent, const char *title, const char *topic ) :
	Chart( 50, parent )
{
	this->setSize( lv_obj_get_width( parent ), lv_obj_get_height( parent ) );
	this->addStyle( popupStyle );
	this->setCaptionString( title );

	this->serie = this->addSerie( LV_COLOR_RED );

			this->serie->Insert( 0 );
			this->serie->Insert( 10 );
			this->serie->Insert( 20 );
			this->serie->Insert( 20 );
			this->serie->Insert( 20 );
			this->serie->Insert( 40 );
			this->serie->Insert( 60 );
			this->serie->Insert( 70 );
			this->serie->Insert( 90 );
			this->serie->Insert( 100 );

	this->setClickable( false );
}

TemperatureChart::~TemperatureChart(){
	delete this->serie;
}
