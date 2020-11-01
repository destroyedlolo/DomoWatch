/************************************************
*	Status bar : status icons
*************************************************/

#include "StatusBar.h"

LV_IMG_DECLARE(foot_16px);

lv_obj_t *icon;

StatusBar::StatusBar( Gui *g, lv_obj_t *parent, const lv_obj_t *cloned ) : Container( parent, cloned ), gui( g ){
	this->CopyStyle( gui->getStyle() );	// Copy gui style

		/* Custom style */
	lv_style_set_bg_opa(this->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_20);
	this->SetSize( LV_HOR_RES, BARHEIGHT );
	this->ApplyStyle();

		/* Battery related */
	this->batPercent = new Label( dynamic_cast<GfxObject *>(this) );
	this->batPercent->SetText( "100%" );
	this->batPercent->Align( LV_ALIGN_IN_RIGHT_MID );
	this->batPercent->AutoRealign();

	this->batIcon = new Image( dynamic_cast<GfxObject *>(this) );
	this->batIcon->Set( LV_SYMBOL_BATTERY_FULL );
	this->batIcon->Align( LV_ALIGN_OUT_LEFT_MID,  **(this->batPercent), -5);
	this->batPercent->AutoRealign();

		/* Step counter */
	this->stepIcon = new Image( dynamic_cast<GfxObject *>(this) );
	this->stepIcon->Set( &foot_16px );
	this->stepIcon->Align( LV_ALIGN_IN_LEFT_MID );

	this->stepCounter = new Label( this );
	this->stepCounter->Align( LV_ALIGN_OUT_RIGHT_MID, **(this->stepIcon), 5 );
	this->stepCounter->SetText( "0" );
	this->stepCounter->AutoRealign();
}

void StatusBar::updateStepCounter(uint32_t counter){
	this->stepCounter->SetText( String(counter).c_str() );
}

void StatusBar::updateBatteryLevel( void ){
	this->batPercent->SetText( (String(ttgo->power->getBattPercentage())+'%').c_str() );
}

void StatusBar::updateBatteryIcon( Gui::lv_icon_battery_t index ){
	static const char *icons[] = {LV_SYMBOL_BATTERY_EMPTY, LV_SYMBOL_BATTERY_1, LV_SYMBOL_BATTERY_2, LV_SYMBOL_BATTERY_3, LV_SYMBOL_BATTERY_FULL, LV_SYMBOL_CHARGE};

	this->batIcon->Set( icons[index] );
}

void StatusBar::initAutomation( void (*func)(lv_task_t *) ){
	this->upd_bat_task = lv_task_create( func, 30000, LV_TASK_PRIO_LOWEST, NULL );
}