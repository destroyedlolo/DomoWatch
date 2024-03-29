/************************************************
*	Settings tile
*************************************************/
#include "Gui.h"
#include "TlStatus.h"
#include "Version.h"

LV_FONT_DECLARE(Ubuntu_16px);

TlStatus::TlStatus( TileView *parent, TileView *cloned ) : 
	Container( parent, cloned )
{

	this->_battery = new Label( this );
	this->_battery->setLongTextMode( LV_LABEL_LONG_BREAK );	// Has to be done BEFORE setWidth()
	this->_battery->setWidth( parent->getWidth() );	// otherwise, it is ignored
	this->_battery->Align( LV_ALIGN_IN_TOP_LEFT, (const lv_obj_t *)NULL, 0,5 );
	this->updAXP( true );	// Initial update otherwise alignment will be wrong

	this->_ram = new Label( this );
	this->_ram->setLongTextMode( LV_LABEL_LONG_SROLL_CIRC );
	this->_ram->setWidth( parent->getWidth()/2 - 5 );
	this->_ram->Align( LV_ALIGN_OUT_BOTTOM_LEFT, this->_battery, 0,10 );

	this->_ramg = new BarGauge( gaugeStyle, this );
	this->_ramg->setSize( parent->getWidth()/2 -10, 16 );
	this->_ramg->setPosXY( parent->getWidth()/2 +5, this->_ram->getY() );
	this->updRam( true );

	this->_PSram = new Label( this );
	this->_PSram->setLongTextMode( LV_LABEL_LONG_SROLL_CIRC );
	this->_PSram->setWidth( parent->getWidth()/2 - 5 );
	this->_PSram->Align( LV_ALIGN_OUT_BOTTOM_LEFT, this->_ram );

	this->_PSramg = new BarGauge( gaugeStyle, this );
	this->_PSramg->setSize( parent->getWidth()/2 -10, 16 );
	this->_PSramg->setPosXY( parent->getWidth()/2 +5, this->_PSram->getY() );
	this->updPSRam( true );

		/* Display version */
	this->_version = new Label( this );
	this->_version->seTexttFont( &Ubuntu_16px );
	this->_version->setText( "DomoWatch " VERSION_H );
	this->_version->Align( LV_ALIGN_IN_BOTTOM_MID );
	this->_version->AutoRealign();

		/* Display last boot time and reason
		 * This information will be set once at GUI creation
		 */

	this->_bootreason = new Label( this );
	this->_bootreason->seTexttFont( &Ubuntu_16px );
	this->_bootreason->setText( getResetCause() );
	this->_bootreason->Align( LV_ALIGN_OUT_TOP_MID, this->_version );
	this->_bootreason->AutoRealign();

	static char buf[29];
	time_t now;
	struct tm  info;

	time( &now );
	localtime_r( &now, &info );
	strftime( buf, sizeof(buf), "Boot at %H:%M on %d %b %Y", &info );

	this->_boottime = new Label( this );
	this->_boottime->seTexttFont( &Ubuntu_16px );
	this->_boottime->setText( buf );
	this->_boottime->Align( LV_ALIGN_OUT_TOP_MID, this->_bootreason );
	this->_boottime->AutoRealign();
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
#else	// From BMA chip
	val += " / Amb : ";
	if( init )
		val += "??.??";
	else
		val += ttgo->bma->temperature();
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
