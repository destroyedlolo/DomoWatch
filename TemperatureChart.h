/************************************************
*	Popup abstraction
*
* Retrieve data from MQTT and then draw single serie chart
*************************************************/

#ifndef TEMPERATURE_CHART_H
#define TEMPERATURE_CHART_H

#include <Chart.h>
#include <Container.h>

class TemperatureChart : public Chart {
	Chart::Serie *serie;
	String repTopic;

public:
	/* Temperature char constructor
	 * -> Container &
	 * -> const char *title : title to display
	 * -> const char *probe : probe name that will be added in topics
	 */
	TemperatureChart( lv_obj_t *parent, const char *title, const char *topic );

	~TemperatureChart();

	/* MQTT message received
	 * -> const char *topic
	 * -> const char *payload
	 * <- is the message accepted. If not, the message may
	 * 	be passed to further handlers
	 */
	bool msgreceived( const char *topic, const char *payload );
};

#endif
