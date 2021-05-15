/************************************************
*	Popup abstraction
*
*	https://github.com/destroyedlolo/DomoWatch/blob/5adfd39c965b49835ff59a6b1ef869d4ca6905ed/TlStatus.cpp
*************************************************/

#ifndef DWPOPUP_H
#define DWPOPUP_H

#include "TemperatureChart.h"

#include <Container.h>

class DWPopup : public Container {
	union {
		TemperatureChart *temperature;
	} popup;

public :
		/* Which data to chart */
	enum Kind {
		SALON,
		JARDIN
	} kind;

	/* DWPopup constructor
	 *
	 *	-> lv_obj_t *parent
	 *	-> enum Kind : which kind of graphics
	 */
	DWPopup( lv_obj_t *, enum Kind );

	/* DWPopup destructor
	 */
	~DWPopup();
};

#endif
