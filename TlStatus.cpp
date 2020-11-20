/************************************************
*	Settings tile
*************************************************/

#include "Gui.h"
#include "TlStatus.h"
#include "Version.h"
#include "font.h"

LV_FONT_DECLARE(Ubuntu_16px);

TlStatus::TlStatus( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned )
{

	this->_battery = new Label( this );
	this->_battery->setFont( &Ubuntu_16px );
	this->_battery->SetLongTextMode( LV_LABEL_LONG_BREAK );	// Has to be done BEFORE setWidth()
	this->_battery->setWidth( parent->getWidth() );	// otherwise, it is ignored
	this->_battery->Align( LV_ALIGN_IN_TOP_LEFT, (const lv_obj_t *)NULL, 0,5 );
	this->updAXP( true );	// Initial update otherwise alignment will be wrong

	this->_ram = new Label( this );
	this->_ram->setFont( &Ubuntu_16px );
	this->_ram->SetLongTextMode( LV_LABEL_LONG_SROLL_CIRC );
	this->_ram->setWidth( parent->getWidth()/2 - 5 );
	this->_ram->Align( LV_ALIGN_OUT_BOTTOM_LEFT, this->_battery, 0,10 );

	this->_ramg = new BarGauge( this );
	lv_style_set_radius( this->_ramg->getStyle(), LV_OBJ_PART_MAIN, 8 );	// set custom style
	lv_style_set_bg_color( this->_ramg->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_AQUA );
	lv_style_set_bg_opa( this->_ramg->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_100 );
/*
	lv_style_set_bg_color( this->_ramg->getStyle(), LV_BAR_PART_INDIC, LV_COLOR_BLUE);
	lv_style_set_text_color( this->_ramg->getStyle(), LV_BAR_PART_INDIC, LV_COLOR_BLACK );
	lv_style_set_bg_opa( this->_ramg->getStyle(), LV_BAR_PART_INDIC, LV_OPA_100 );
*/
	lv_style_set_border_width( this->_ramg->getStyle(), LV_OBJ_PART_MAIN, 1 );
	lv_style_set_text_color( this->_ramg->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_WHITE );
	this->_ramg->applyStyle();
	this->_ramg->setSize( parent->getWidth()/2 -10, 16 );
	this->_ramg->setPosXY( parent->getWidth()/2 +5, this->_ram->getY() );
//	this->_ramg->Align( LV_ALIGN_OUT_RIGHT_TOP, this->_ram );
	this->updRam( true );

	this->_PSram = new Label( this );
	this->_PSram->setFont( &Ubuntu_16px );
	this->_PSram->SetLongTextMode( LV_LABEL_LONG_SROLL_CIRC );
	this->_PSram->setWidth( parent->getWidth()/2 - 5 );
	this->_PSram->Align( LV_ALIGN_OUT_BOTTOM_LEFT, this->_ram );

	this->_PSramg = new BarGauge( this );
	this->_PSramg->copyStyle( this->_ramg->getStyle() );	// Apply the same style
	this->_PSramg->applyStyle();
	this->_PSramg->setSize( parent->getWidth()/2 -10, 16 );
	this->_PSramg->setPosXY( parent->getWidth()/2 +5, this->_PSram->getY() );
//	this->_PSramg->Align( LV_ALIGN_OUT_RIGHT_TOP, this->_PSram );

	this->updPSRam( true );

		/* Display version */
	this->_version = new Label( this );
	this->_version->setFont( &Ubuntu_16px );
	this->_version->setText( "DomoWatch " VERSION_H );
	this->_version->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->_version->AutoRealign();
}

void TlStatus::updAXP( bool init ){
	String val = "Voltage : ";
	if( init )
		val += "?.??";
	else
		val += ttgo->power->getBattVoltage()/1000;
	val += "V\n";

	val += (ttgo->power->isChargeing() and !init) ? "Charging :" : "Discharging : ";
	if( init )
		val += "???.??";
	else
		val += ttgo->power->isChargeing() ? ttgo->power->getBattChargeCurrent() : ttgo->power->getBattDischargeCurrent();
	val += "mA\n";

	val += "Temp : ";
	if( init )
		val += "??.??";
	else
		val += ttgo->power->getTemp();
#if 0	// Seems note used on TWatch 2020
	val += " / TSTemp : ";
	if( init )
		val += "??.??";
	else
		val += ttgo->power->getTSTemp();
#endif
	val += "\nVBus : ";
	if( init )
		val += "??.??V / ???.??mA";
	else if( ttgo->power->isVBUSPlug() ){
		val += ttgo->power->getVbusVoltage() / 1000;
		val += "V / ";
		val += ttgo->power->getVbusCurrent();
		val += "mA";
	}

	this->_battery->setText( val.c_str() );
}

void TlStatus::updRam( bool init ){
	String val = "Ram : ";
	if( init )
		val += "???8/???? (???%)";
	else {
		val += String( ESP.getFreeHeap() / 1024 );
		val += "/";
		val += String( ESP.getHeapSize() / 1024 );
		val += " (";
		val += String( 100 - ESP.getFreeHeap()*100/ESP.getHeapSize(), DEC);
		val += "%)";
	}

	this->_ram->setText( val.c_str() );
	this->_ramg->setValue( 100 - ESP.getFreeHeap()*100/ESP.getHeapSize() );
}

void TlStatus::updPSRam( bool init ){
	String val = "PSRam : ";
	if( init )
		val += "???8/???? (???%)";
	else {
		val += String( ESP.getFreePsram() / 1024 );
		val += "/";
		val += String( ESP.getPsramSize() / 1024 );
		val += " (";
		val += String( 100 - ESP.getFreePsram()*100/ESP.getPsramSize(), DEC);
		val += "%)";
	}

	this->_PSram->setText( val.c_str() );
	this->_PSramg->setValue( 100 - ESP.getFreePsram()*100/ESP.getPsramSize() );
}

void TlStatus::updatefields( void ){
	this->updAXP();
	this->updRam();
	this->updPSRam();
}

static void cbUpd( lv_task_t *tsk ){
	((TlStatus *)(tsk->user_data))->updatefields();
}

void TlStatus::initAutomation( void ){
	this->upd_task = lv_task_create( cbUpd, 1500, LV_TASK_PRIO_LOW, this );
}
