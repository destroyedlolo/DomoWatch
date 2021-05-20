/************************************************
*	Popup abstraction
*
* Retrieve data from MQTT and then draw single serie chart
*
* Notez-Bien :
* 	- As only integer can be stored, temperature are
*	 	multiplied by 100
* 	- Hardcoded for 50 samples
*************************************************/

#include "Gui.h"
#include "TemperatureChart.h"
#include "Network.h"

TemperatureChart::TemperatureChart( lv_obj_t *parent, const char *title, const char *topic ) :
	Chart( 50, parent )
{
	this->setSize( lv_obj_get_width( parent ), lv_obj_get_height( parent ) );
	this->addStyle( popupStyle );
		// Let space for ticks (left)
		// and on top / bottom to let them displayed
	this->setPadding( 10,10,50,0, LV_CHART_PART_BG );
	this->yTicksLen( 0, 0 );

	this->setCaptionString( title );
	this->setClickable( false );	// pass event to upper

	this->serie = this->addSerie( LV_COLOR_RED );

		// Subscribe to response topic
	this->repTopic = "ReponseHistorique/";
	this->repTopic += topic;
	network.MQTTsubscribe( this->repTopic.c_str() );

	String subtopic = topic;
	subtopic += ";50";
	network.MQTTpublishString( "DemandeHistorique", subtopic.c_str() );
}

TemperatureChart::~TemperatureChart(){
	network.MQTTunsubscribe( this->repTopic.c_str() );
	delete this->serie;
}

bool TemperatureChart::msgreceived( const char *topic, const char *payload ){
	if( this->repTopic == topic ){
		Serial.printf("'%s' accepted", topic);

			// Feed with received data
		const char *p = payload;
		int bidon;
		float val, min = 10000, max = -10000;

		for(; p; p = strchr(p, '\n') ){
			p++;
			sscanf(p, "%d\t%f", &bidon, &val);
//			Serial.printf("---> %f\n", val);
			this->serie->Insert( val * 100 );

			if( val < min )
				min = val;
			if( val > max )
				max = val;
		}
		this->serie->setRange( min*100, max*100 );

		Serial.printf( "Temperature chart : %f -> %f \n", min, max );

		char t[ 12 ];	// (5 t°)*2 + 1*'\n' + 1
		sprintf(t, "%.2f\n%.2f", max, min );
		this->yTicks( t );

/*
		String ticks((float)max/100);
		ticks += "\n";
		ticks += (float)min/100;
		this->yTicks( ticks.c_str() );
*/

		return true;
	}

	return false;
}
