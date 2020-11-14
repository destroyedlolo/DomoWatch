/************************************************
*	Main tile : Hour and date
*************************************************/

#ifndef TLDTHR_H
#define TLDTHR_H

// #include "TileView.h"
#include "Container.h"
#include "Label.h"

class TlDateTime : public Container {
	Container	*cont;

	Label		*timelabel,
				*datelabel;

	lv_task_t	*upd_h_task;	// Task to update the Gui

	int 		daynum;		// number of the day of the year
public:
	/* DateTime tile constructor
	 * -> mainstyle : style to apply
	 * -> parent, cloned : see Container
	 */
/*D
	TlDateTime( TileView *parent=NULL, TileView *cloned=NULL );
*/
	TlDateTime( Container *parent=NULL, Container *cloned=NULL );

	/* update field */
	void updateTime( void );

	/* Initialise automation
	 */
	void initAutomation( void );
};
#endif
