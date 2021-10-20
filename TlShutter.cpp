/************************************************
*	Shutter tile
*************************************************/

#include "Gui.h"
#include "Network.h"
#include "TlShutter.h"

LV_IMG_DECLARE(up_64px);
LV_IMG_DECLARE(my_64px);
LV_IMG_DECLARE(down_64px);

static const char *shutterslabel[3] = {
	"Parents\nBureau\nCuisine\nChat",
	"Cheminée\nSalon\nBalcon",
	"Amis\nOcéane\nJoris"
};

static const char *shutterstopics[3][4] = {
	{		// Rdc
		"maison/Volet/chParents",
		"maison/Volet/Buro",
		"maison/Volet/Cuisine/Fenetre",
		"maison/Volet/Cuisine/Chat"
	}, {	// Salon
		"maison/Volet/Salon/Cheminee",
		"maison/Volet/Salon/Fenetre",
		"maison/Volet/Salon/Balcon"
	}, {	// 1er
		"maison/Volet/chAmis",
		"maison/Volet/chOceane",
		"maison/Volet/chJoris"
	}
};

static void stairChanged(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_VALUE_CHANGED){
		Serial.printf( "Stair changed to : %d\n", lv_dropdown_get_selected( obj ));
		gui->updStair();
	}
}

void TlShutter::updStair( void ){
	this->_shutter->setChoices( shutterslabel[ this->_stair->getSelected() ] );
	this->_shutter->setSelected( 0 );
}

const char *TlShutter::whichShutterTopic( void ){
	return shutterstopics[ this->_stair->getSelected() ][ this->_shutter->getSelected() ];
}

static void upClicked( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.printf( "up -> '%s'\n", gui->whichShutterTopic() );
		network.MQTTpublishString( gui->whichShutterTopic(), "Up" );
	}
}

static void myClicked( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.printf( "my -> '%s'\n", gui->whichShutterTopic() );
		network.MQTTpublishString( gui->whichShutterTopic(), "My" );
	}
}

static void downClicked( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.printf( "down -> '%s'\n", gui->whichShutterTopic() );
		network.MQTTpublishString( gui->whichShutterTopic(), "Down" );
	}
}

TlShutter::TlShutter( TileView *parent, TileView *cloned ) :
	Container( parent, cloned )
{

		/* Stair selector */
	this->_stair = new DropDown( stairsStyle, this );
	this->_stair->setChoices( "Rdc\nSalon\n1er", true );
	this->_stair->Align( LV_ALIGN_IN_LEFT_MID, (const lv_obj_t *)NULL, 0, -10 );

	this->_stair->attacheEventeHandler( stairChanged );

		/* Shutter selector */
	this->_shutter = new DropDown( stairsStyle, this );
	this->_shutter->Align( LV_ALIGN_OUT_BOTTOM_MID, this->_stair, 0, 20);
	this->_shutter->setChoices( shutterslabel[0] );

		/* Movements */
	this->upIcon = new Image( this );
	this->upIcon->Set( &up_64px );
	this->upIcon->Align( LV_ALIGN_IN_TOP_RIGHT );
	this->upIcon->setClickable( true );


	this->upIcon->attacheEventeHandler( upClicked );

	this->myIcon = new Image( this );
	this->myIcon->Set( &my_64px );
	this->myIcon->Align( LV_ALIGN_IN_RIGHT_MID );
	this->myIcon->setClickable( true );

	this->myIcon->attacheEventeHandler( myClicked );

	this->downIcon = new Image( this );
	this->downIcon->Set( &down_64px );
	this->downIcon->Align( LV_ALIGN_IN_BOTTOM_RIGHT );
	this->downIcon->setClickable( true );

	this->downIcon->attacheEventeHandler( downClicked );
}
