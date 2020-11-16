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
	this->updAXP();	// Initial update otherwise alignment will be wrong

	this->_ram = new Label( this );
	this->_ram->setFont( &Ubuntu_16px );
	this->_ram->SetLongTextMode( LV_LABEL_LONG_SROLL_CIRC );
	this->_ram->setWidth( parent->getWidth()/2 );
	this->_ram->Align( LV_ALIGN_OUT_BOTTOM_LEFT, this->_battery, 0,10 );
	this->updRam();

	this->_PSram = new Label( this );
	this->_PSram->setFont( &Ubuntu_16px );
	this->_PSram->SetLongTextMode( LV_LABEL_LONG_SROLL_CIRC );
	this->_PSram->setWidth( parent->getWidth()/2 );
	this->_PSram->Align( LV_ALIGN_OUT_BOTTOM_LEFT, this->_ram );
	this->updPSRam();

		/* Display version */
	this->_version = new Label( this );
	this->_version->setFont( &Ubuntu_16px );
	this->_version->setText( "DomoWatch " VERSION_H );
	this->_version->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->_version->AutoRealign();

	this->updatefields();	// Tile built, initial fields update
}

void TlStatus::updAXP( void ){
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

void TlStatus::updRam( void ){
	String val = "Ram : ";
	val += String( ESP.getFreeHeap() / 1024 );
	val += "/";
	val += String( ESP.getHeapSize() / 1024 );
	val += " (";
	val += String( 100 - ESP.getFreeHeap()*100/ESP.getHeapSize(), DEC);
	val += "%)";

	this->_ram->setText( val.c_str() );
}

void TlStatus::updPSRam( void ){
	String val = "PSRam : ";
	val += String( ESP.getFreePsram() / 1024 );
	val += "/";
	val += String( ESP.getPsramSize() / 1024 );
	val += " (";
	val += String( 100 - ESP.getFreePsram()*100/ESP.getPsramSize(), DEC);
	val += "%)";

	this->_PSram->setText( val.c_str() );
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
