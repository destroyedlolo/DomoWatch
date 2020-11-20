/************************************************
*	Status bar : status icons
*************************************************/

#include "StatusBar.h"
#include "MsgBox.h"

LV_IMG_DECLARE(foot_16px);

StatusBar::StatusBar( lv_style_t *mainstyle, lv_obj_t *parent, const lv_obj_t *cloned ) : 
	Container( parent, cloned )
{
	this->copyStyle( mainstyle );	// Copy gui style

		/***
		 * Customize style of the bar
		 ***/

	lv_style_set_bg_opa(this->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_20);
	this->setSize( LV_HOR_RES, BARHEIGHT );
	this->applyStyle();


		/***
		 * Step counter related 
		 ***/

		/* Create of button above step counter to reset it */
	this->stepButton = new Button( this );
//	this->stepButton->setHeight( BARHEIGHT );
	this->stepButton->setLayout( LV_LAYOUT_ROW_MID );	// child are horizontally aligned
	this->stepButton->setFit( LV_FIT_TIGHT );	// Its size is the one of it's child
	this->stepButton->Align( LV_ALIGN_IN_LEFT_MID );	// it is itself aligned on the left
	lv_style_set_bg_opa( this->stepButton->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_0 );
	this->stepButton->applyStyle();
/* Needed ?
	lv_btn_set_style( this->_btn, LV_BTN_STYLE_REL, &this->_transp_style);
	lv_btn_set_style( this->_btn, LV_BTN_STYLE_PR, &this->_transp_style);
*/

		/* Only an image of a foot */
	this->stepIcon = new Image( this->stepButton );
	this->stepIcon->Set( &foot_16px );
	this->stepIcon->setClickable( false );	// Pass click to the parent

	this->stepCounter = new Label( this->stepButton );
	this->stepCounter->setText( "0" );
	this->stepCounter->AutoRealign();
	this->stepCounter->setClickable( false );	// Pass click to the parent

		/***
		 * Battery related
		 *
		 * No need for a button around as no action is possible
		 * so using "classical" alignment
		 ***/
	this->batPercent = new Label( this );	// Battery value
	this->batPercent->setText( "100%" );
	this->batPercent->Align( LV_ALIGN_IN_RIGHT_MID );
	this->batPercent->AutoRealign();

	this->batIcon = new Image( this );		// corresponding icon
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
		} else if(level > 80){
			color = LV_COLOR_GREEN;
			index = Gui::LV_ICON_BAT_3;
		} else if(level > 45)
			index = Gui::LV_ICON_BAT_2;
		 else if(level > 20){
			color = LV_COLOR_ORANGE;
			index = Gui::LV_ICON_BAT_1;
		} else {
			color = LV_COLOR_RED;
			index = Gui::LV_ICON_BAT_EMPTY;
		}
	}

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
