/************************************************
*	Status bar : watch status icons
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
#include "Button.h"

class StatusBar : public Container {
	/* Step counter */
	Button		*stepButton;
	Image		*stepIcon;
	Label 		*stepCounter;

	/* Battery */
	Label 		*batPercent;
	Image		*batIcon;
	lv_task_t	*upd_bat_task;	// task to update battery level

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
	 */
	void initAutomation( void );
};
#endif

