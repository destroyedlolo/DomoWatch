/************************************************
*	Status tile
*************************************************/

#ifndef TLSET_H
#define TLSET_H

#include "TileView.h"
#include "Container.h"
#include "Label.h"

class TlStatus : public Container {
	Label		*_battery;
	Label		*_version;

	lv_task_t	*upd_task;	// Task to update the Gui

public:
	/* Settings' tile constructor
	 * -> parent, cloned : see Container
	 */
	TlStatus( TileView *parent=NULL, TileView *cloned=NULL );

	/* update field */
	void updatefields( void );

	/* Initialise automation
	 */
	void initAutomation( void );
};

#endif
