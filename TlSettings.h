/************************************************
*	Settings tile
*************************************************/

#ifndef TLSET_H
#define TLSET_H

#include "Container.h"
#include "Label.h"

class TlSettings : public Container {
	Label		*_version;

public:
	TlSettings( lv_obj_t *parent=NULL, const lv_obj_t *cloned=NULL );
};

#endif
