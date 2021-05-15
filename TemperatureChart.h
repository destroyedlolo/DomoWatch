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

public:
	/* Temperature char constructor
	 * -> Container &
	 * -> const char *title : title to display
	 * -> const char *probe : probe name that will be added in topics
	 */
	TemperatureChart( lv_obj_t *parent, const char *title, const char *topic );

	~TemperatureChart();
};

#endif
