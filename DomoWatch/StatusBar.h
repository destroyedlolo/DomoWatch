/************************************************
*	Status bar : status icons
*
*	Technically it's a container with icons and
*	labels inside
*************************************************/

#ifndef STB_H
#define STB_H

#include "Gui.h"
#include "Container.h"
#include "Label.h"

#define BARHEIGHT 30

class StatusBar : public Container {
	Gui 		*gui;

	Label 		*batpercent;
	lv_task_t	*upd_bat_task;	// task to update battery level

	Label 		*stepcounter;

public:
	/* status bar constructor
	 * -> Gui * : the gui main class
	 * -> parent, cloned : see Container
	 */
	StatusBar( Gui *, lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL );

	/* Update step counter label
	 * -> uint32_t counter : value
	 */
	void updateStepCounter( uint32_t counter );

	/* Update battery level label
	 */
	void updateBatteryLevel( void );

	/* Initialise automation
	 * 	-> batfunc : callback to be launched to update battery level
	 */
	void initAutomation( void (*)(lv_task_t *) );
};

#endif
