/************************************************
*	Main tile : Hour and date
*************************************************/

#ifndef TLDTHR_H
#define TLDTHR_H

#include "Container.h"
#include "Label.h"

class TlDateTime : public Container {
	Label		*timelabel,
				*datelabel;

	int 		daynum;		// number of the day of the year
public:
	TlDateTime( lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL );
};

#endif
