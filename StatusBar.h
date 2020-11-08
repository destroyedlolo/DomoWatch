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
#include "Image.h"

class StatusBar : public Container {
	Label 		*batPercent;
	Image		*batIcon;
	lv_task_t	*upd_bat_task;	// task to update battery level

	Image		*stepIcon;
	Label 		*stepCounter;

public:
	/* status bar constructor
	 * -> mainstyle : style to apply
	 * -> parent, cloned : see Container
	 */
	StatusBar( lv_style_t *mainstyle, lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL );

	/* Update step counter label
	 * -> uint32_t counter : value
	 */
	void updateStepCounter( uint32_t counter );

	/* Update battery level label
	 */
	void updateBatteryLevel( void );

	/* Update battery icon
	 * -> lv_icon_battery_t index : see Gui.h
	 */
	void updateBatteryIcon( Gui::lv_icon_battery_t index );

	/* Initialise automation
	 * 	-> batfunc : callback to be launched to update battery level
	 */
	void initAutomation( void );
};
#endif
