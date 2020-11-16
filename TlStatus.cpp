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
	this->_battery->Align( LV_ALIGN_IN_TOP_LEFT );
	this->_battery->AutoRealign();


		/* Display version */
	this->_version = new Label( this );
	this->_version->setFont( &Ubuntu_16px );
	this->_version->setText( "DomoWatch " VERSION_H );
	this->_version->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->_version->AutoRealign();

	this->updatefields();	// Tile built, initial fields update
}

void TlStatus::updatefields( void ){
	String val = "Voltage : ";
	val += ttgo->power->getBattVoltage()/1000;
	val += "V\n";

	val += ttgo->power->isChargeing() ? "Charging :" : "Discharging : ";
	val += ttgo->power->isChargeing() ? ttgo->power->getBattChargeCurrent() : ttgo->power->getBattDischargeCurrent();
	val += "mA\n";

	val += "Temp : ";
	val += ttgo->power->getTemp();
	val += " / TSTemp : ";
	val += ttgo->power->getTSTemp();

	val += "\nVBus : ";
	if( ttgo->power->isVBUSPlug() ){
		val += ttgo->power->getVbusVoltage() / 1000;
		val += "V / ";
		val += ttgo->power->getVbusCurrent();
		val += "mA";
	}

	this->_battery->setText( val.c_str() );
}

static void cbUpd( lv_task_t *tsk ){
	((TlStatus *)(tsk->user_data))->updatefields();
}

void TlStatus::initAutomation( void ){
	this->upd_task = lv_task_create( cbUpd, 1000, LV_TASK_PRIO_LOW, this );
}
