/************************************************
*	Settings tile
*************************************************/

#ifndef TLSETTINGS_H
#define TLSETTINGS_H

#include "TileView.h"
#include "Container.h"
#include "Label.h"
#include "Image.h"
#include "Slider.h"

class TlSettings : public Container {
	Image		*brightnessIcon;
	Slider		*brightnessSlider;

public:
	/* Settings' tile constructor
	 * -> parent, cloned : see Container
	 */
	TlSettings( TileView *parent, TileView *cloned=NULL );
};

#endif
