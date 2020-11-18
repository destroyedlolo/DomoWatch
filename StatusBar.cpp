/************************************************
*	Status bar : status icons
*************************************************/

#include "StatusBar.h"

LV_IMG_DECLARE(foot_16px);

StatusBar::StatusBar( lv_style_t *mainstyle, lv_obj_t *parent, const lv_obj_t *cloned ) : 
	Container( parent, cloned ),
	prev_idx( Gui::LV_ICON_UNKNOWN )
{
	this->copyStyle( mainstyle );	// Copy gui style

		/* Customize style */
	lv_style_set_bg_opa(this->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_20);
	this->setSize( LV_HOR_RES, BARHEIGHT );
	this->applyStyle();

		/* Step counter related */
	this->stepIcon = new Image( this );
	this->stepIcon->Set( &foot_16px );
	this->stepIcon->Align( LV_ALIGN_IN_LEFT_MID );

	this->stepCounter = new Label( this );
	this->stepCounter->Align( LV_ALIGN_OUT_RIGHT_MID, this->stepIcon->getMyself(), 5 );
	this->stepCounter->setText( "0" );
	this->stepCounter->AutoRealign();

		/* Battery related */
	this->batPercent = new Label( this );
	this->batPercent->setText( "100%" );
	this->batPercent->Align( LV_ALIGN_IN_RIGHT_MID );
	this->batPercent->AutoRealign();

	this->batIcon = new Image( this );
	this->batIcon->Set( LV_SYMBOL_BATTERY_FULL );
	this->batIcon->Align( LV_ALIGN_OUT_LEFT_MID,  this->batPercent->getMyself(), -5);
	this->batPercent->AutoRealign();
}

void StatusBar::updateStepCounter(uint32_t counter){
	this->stepCounter->setText( String(counter).c_str() );
}

void StatusBar::updateBatteryLevel( void ){
	this->batPercent->setText( (String(ttgo->power->getBattPercentage())+'%').c_str() );
	this->updateBatteryIcon( Gui::LV_ICON_UNKNOWN );	// Rethink the icon
}

void StatusBar::updateBatteryIcon( Gui::lv_icon_battery_t index ){
	lv_color_t color = LV_COLOR_WHITE;	// Default color
	if( index == Gui::LV_ICON_UNKNOWN && ttgo->power->isChargeing() )	// Are we charging ?
		index = Gui::LV_ICON_CHARGE;
	
	if( index == Gui::LV_ICON_CHARGE )
		color = LV_COLOR_BLUE;
	else if( index >= Gui::LV_ICON_CALCULATION ){	// Determine as per battery level
		int level = ttgo->power->getBattPercentage();
		if(level > 95){
			color = LV_COLOR_GREEN;
			index = Gui::LV_ICON_BAT_FULL;
		} else if(level > 80)
			index = Gui::LV_ICON_BAT_3;
		else if(level > 45)
			index = Gui::LV_ICON_BAT_2;
		 else if(level > 20){
			color = LV_COLOR_ORANGE;
			index = Gui::LV_ICON_BAT_1;
		} else {
			color = LV_COLOR_RED;
			index = Gui::LV_ICON_BAT_EMPTY;
		}
	}

	if( this->prev_idx == index )	// Same icon index so no need to redraw
		return;
	this->prev_idx = index;

	this->batIcon->Recolor( color );	// Apply the color

	static const char *icons[] = {LV_SYMBOL_BATTERY_EMPTY, LV_SYMBOL_BATTERY_1, LV_SYMBOL_BATTERY_2, LV_SYMBOL_BATTERY_3, LV_SYMBOL_BATTERY_FULL, LV_SYMBOL_CHARGE};
	this->batIcon->Set( icons[index] );	// And the icon
}

static void cbUpdBat( lv_task_t *tsk ){
	((StatusBar *)(tsk->user_data))->updateBatteryLevel();
}

void StatusBar::initAutomation( void ){
	this->upd_bat_task = lv_task_create( cbUpdBat, 30000, LV_TASK_PRIO_LOWEST, this );
}
