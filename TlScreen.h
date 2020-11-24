/************************************************
*	Screen's settings tile
*************************************************/

#ifndef TLSCREEN_H
#define TLSCREEN_H

#include "TileView.h"
#include "Container.h"
#include "Label.h"
#include "Image.h"
#include "Slider.h"

class TlScreen : public Container {
	Container	*brightnessCont;
	Image		*brightnessIcon;
	Slider		*brightnessSlider;

	Container	*saverCont;
	Image		*saverIcon;
	Slider		*saverSlider;
	Label		*saverLabel;

public:
	/* Screen's tile constructor
	 * -> parent, cloned : see Container
	 */
	TlScreen( TileView *parent, TileView *cloned=NULL );
};

#endif
