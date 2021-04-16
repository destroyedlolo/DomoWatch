/************************************************
*	Inter tasks communication
*
*	(including IRQ exchanges)
*************************************************/
#ifndef INTERTASKCOM_H
#define INTERTASKCOM_H

#include <freertos/event_groups.h>

	/*******
	* Signaling
	 *******/

	/* IRQ own codes are running in dedicated environment so
	 * are having reduced API and must run as fast as possible.
	 * Consequently, they are only positioning some flags and 
	 * actions are done in handler that are running  as normal
	 * tasks.
	 */

#define WATCH_IRQ_AXP		_BV(0)	// IRQ from the power management
#define WATCH_IRQ_BMA		_BV(1)	// IRQ from movements

	/* subtasks signaling */

#define WATCH_WIFI_STATUS	_BV(2)	// WIFI status need to be refreshed

extern EventGroupHandle_t itc_signals;	// defined in .ino

#endif
