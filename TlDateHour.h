/************************************************
*	Main tile : Hour and date
*************************************************/

#ifndef TLDTHR_H
#define TLDTHR_H

#include "Gui.h"
#include "Tile.h"
#include "Label.h"

class TlDateHour : public Tile {
	
	Label		*timelabel,
				*datelabel;

	lv_task_t	*upd_h_task;	// Task to update the Gui

	int 		prev_day;	// Previous day of the year

public:
	/* Date & hour tile's constructor
	 * -> Gui * : the gui main class
	 */
	TlDateHour( Gui * );

	/* update field */
	void updateTime( void );

	/* Initialise automation
	 * 	-> timefunc : callback to be launched to update time
	 */
	void initAutomation( void (*)(lv_task_t *) );
};

#endif
