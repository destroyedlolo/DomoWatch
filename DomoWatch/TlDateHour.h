/************************************************
*	Main tile : Hour and date
*************************************************/

#ifndef DTHR_H
#define DTHR_H

#include "Gui.h"
#include "Tile.h"

class TlDateHour /* : public Tile */ {
	Gui *gui;

	lv_style_t	timestyle,
				datestyle;
	
	lv_obj_t 	*contener,
				*timelabel,
				*datelabel;

	lv_task_t	*upd_task;	// Task to update the Gui

	int 		prev_day;	// Previous day of the year

public:
	/* Date & hour tile's constructor
	 * -> tileview : list of tiles to add too
	 * -> Gui * : the gui main class
	 */
	TlDateHour( Gui * );

	/* update field */
	void updateTime( void );

	/* Initialise automation
	 * 	-> func : callback to be launched
	 */
	void initAutomation( void (*)(lv_task_t *) );
};

#endif
