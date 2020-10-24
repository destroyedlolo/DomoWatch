/************************************************
*	Status bar : status icons
*************************************************/

#include "StatusBar.h"

StatusBar::StatusBar( Gui *g, lv_obj_t *parent, const lv_obj_t *cloned ) : Container( parent, cloned ), gui( g ){
	this->CopyStyle( gui->getStyle() );	// Copy gui style

		/* Custom style */
	lv_style_set_bg_opa(this->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_20);
	this->SetSize( LV_HOR_RES, BARHEIGHT );
	this->ApplyStyle();

		/* Battery related */
	this->batpercent = new Label( this );
	this->batpercent->SetText( "100%" );
	this->batpercent->Align( LV_ALIGN_IN_RIGHT_MID );
	this->batpercent->AutoRealign();

		/* Step counter */
	this->stepcounter = new Label( this );
	this->stepcounter->Align( LV_ALIGN_IN_LEFT_MID );
	this->stepcounter->SetText( "0" );
	this->stepcounter->AutoRealign();
}

void StatusBar::updateStepCounter(uint32_t counter){
	this->stepcounter->SetText( String(counter).c_str() );
}

void StatusBar::updateBatteryLevel( void ){
	this->batpercent->SetText( (String(ttgo->power->getBattPercentage())+'%').c_str() );
}

void StatusBar::initAutomation( void (*func)(lv_task_t *) ){
	this->upd_bat_task = lv_task_create( func, 30000, LV_TASK_PRIO_LOWEST, NULL );
}
