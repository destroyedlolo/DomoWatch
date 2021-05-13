/************************************************
*	Status bar : watch status icons
*
*	Technically it's a container with icons and
*	labels inside
*************************************************/

#ifndef STB_H
#define STB_H

#include "Gui.h"

#include <Container.h>
#include <Label.h>
#include <Image.h>
#include <Button.h>

class StatusBar : public Container {
	/* Step counter */
	Button		*stepButton;
	Image		*stepIcon;
	Label 		*stepCounter;

	/* WiFi */
	Image		*wifiIcon;

	/* Battery */
	Label 		*batPercent;
	Image		*batIcon;
	lv_task_t	*upd_bat_task;	// task to update battery level

public:
	/* status bar constructor
	 * -> mainstyle : style to apply
	 * -> parent, cloned : see Container
	 */
	StatusBar( lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL );

	/* Initialise automation
	 */
	void initAutomation( void );


	/* Update step counter label
	 * -> uint32_t counter : value
	 */
	void updateStepCounter( void );

	/* Update battery level label
	 */
	void updateBatteryLevel( void );

	/* Update battery icon
	 * -> lv_icon_battery_t index : see Gui.h
	 */
	void updateBatteryIcon( Gui::lv_icon_battery_t index );

	/* Update wifi icon as per net_status
	 */
	void updateNetwork( void );
};

#endif
