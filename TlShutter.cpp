#include "Gui.h"
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
	}
}

static void myClicked( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.printf( "my -> '%s'\n", gui->whichShutterTopic() );
	}
}

static void downClicked( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.printf( "down -> '%s'\n", gui->whichShutterTopic() );
	}
}

TlShutter::TlShutter( TileView *parent, TileView *cloned ) :
	Container( parent, cloned )
{
		/* Stair selector */
	this->_stair = new DropDown( this );
	this->_stair->setChoices( "Rdc\nSalon\n1er", true );
	this->_stair->Align( LV_ALIGN_IN_LEFT_MID, (const lv_obj_t *)NULL, 0, -10 );

		/* Add custom style to add background */
	lv_style_set_radius( this->_stair->getStyle(), LV_OBJ_PART_MAIN, 5 );
	lv_style_set_bg_color( this->_stair->getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_GRAY );
	lv_style_set_bg_opa( this->_stair->getStyle(), LV_OBJ_PART_MAIN, LV_OPA_70 );
	lv_style_set_border_width( this->_stair->getStyle(), LV_OBJ_PART_MAIN, 1 );
	this->_stair->applyStyle();

		/* Shutter selector */
	this->_shutter = new DropDown( this );
	this->_shutter->Align( LV_ALIGN_OUT_BOTTOM_MID, this->_stair, 0, 20);
	this->_shutter->copyStyle( this->_stair->getStyle() );
	this->_shutter->applyStyle();
	this->_shutter->setChoices( shutterslabel[0] );

	this->_stair->attacheEventeHandler( stairChanged );

		/* Movements */
	this->upButton = new Button( this );
	this->upButton->Align( LV_ALIGN_IN_TOP_RIGHT );
	this->upButton->setFit( LV_FIT_TIGHT );
	this->upButton->AutoRealign();
	this->upButton->setPadding(0);

	this->upIcon = new Image( this->upButton );
	this->upIcon->Set( &up_64px );
	this->upIcon->setClickable( false );	// Pass click to the parent

	this->upButton->attacheEventeHandler( upClicked );

	this->myButton = new Button( this );
	this->myButton->Align( LV_ALIGN_IN_RIGHT_MID );
	this->myButton->setFit( LV_FIT_TIGHT );
	this->myButton->AutoRealign();
	this->myButton->setPadding(0);

	this->myIcon = new Image( this->myButton );
	this->myIcon->Set( &my_64px );
	this->myIcon->setClickable( false );	// Pass click to the parent

	this->myButton->attacheEventeHandler( myClicked );

	this->downButton = new Button( this );
	this->downButton->Align( LV_ALIGN_IN_BOTTOM_RIGHT );
	this->downButton->setFit( LV_FIT_TIGHT );
	this->downButton->AutoRealign();
	this->downButton->setPadding(0);

	this->downIcon = new Image( this->downButton );
	this->downIcon->Set( &down_64px );
	this->downIcon->setClickable( false );	// Pass click to the parent

	this->downButton->attacheEventeHandler( downClicked );
}
