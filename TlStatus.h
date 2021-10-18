/************************************************
*	Status tile
*************************************************/

#ifndef TLSET_H
#define TLSET_H

#include <TileView.h>
#include <Container.h>
#include <Label.h>
#include <BarGauge.h>

class TlStatus : public Container {
	Label		*_battery;
	Label		*_boottime;
	Label		*_bootreason;
	Label		*_version;

	Label		*_ram;
	BarGauge	*_ramg;
	Label		*_PSram;
	BarGauge	*_PSramg;

	lv_task_t	*upd_task;	// Task to update the Gui

public:
	/* Status' tile constructor
	 * -> parent, cloned : see Container
	 */
	TlStatus( TileView *parent=NULL, TileView *cloned=NULL );

	/* update field */
	void updatefields( void );

	/* Initialise automation
	 */
	void initAutomation( void );

private:
	void updAXP( bool init=false );
	void updRam( bool init=false );
	void updPSRam( bool init=false );
};

#endif
