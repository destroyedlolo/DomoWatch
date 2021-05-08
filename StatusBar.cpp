/************************************************
*	Status bar : status icons
*************************************************/

#include "StatusBar.h"
#include "Network.h"

	/****
	 * callbacks
	 ***/

static void stepClicked( lv_obj_t *, lv_event_t event ){
	if(event == LV_EVENT_CLICKED){
		Serial.println("RAZ podo");
		ttgo->bma->resetStepCounter();
		gui->updateStepCounter();
	}
}

	/*****
	 * GUI
	 *****/

LV_IMG_DECLARE(foot_16px);

StatusBar::StatusBar( lv_obj_t *parent, const lv_obj_t *cloned ) : 
	Container( parent, cloned )
{
	this->addStyle( gui->getMainStyle() );	// Copy gui style

		/***
		 * Customize style of the bar
		 ***/

	this->setBgOpacity( LV_OPA_20 );
	this->setSize( LV_HOR_RES, BARHEIGHT );

		/***
		 * Step counter related 
		 ***/

		/* Create of button above step counter to reset it */
//	this->stepButton = new Button( *gui, this );
	this->stepButton = new Button( this );
	this->stepButton->addStyle( gui->getMainStyle() );
	this->stepButton->setLayout( LV_LAYOUT_ROW_MID );	// child are horizontally aligned
	this->stepButton->setSize( this->getWidth() / 2, this->getHeight() );
	this->stepButton->Align( LV_ALIGN_IN_LEFT_MID );	// it is itself aligned on the left
	this->stepButton->setBgOpacity( LV_OPA_0 );

		/* Only an image of a foot */
	this->stepIcon = new Image( this->stepButton );
	this->stepIcon->Set( &foot_16px );
	this->stepIcon->setClickable( false );	// Pass click to the parent

static Style st;
st.setTextColor( LV_COLOR_WHITE );
// lv_style_set_text_color( st.getStyle(), LV_OBJ_PART_MAIN, LV_COLOR_WHITE );

//	this->stepCounter = new Label( *gui, this->stepButton );
	this->stepCounter = new Label( this->stepButton );
	this->stepCounter->addStyle( gui->getMainStyle() );
	this->stepCounter->setText( "??????" );
	this->stepCounter->setClickable( false );	// Pass click to the parent

	this->stepButton->attacheEventeHandler( stepClicked );


this->stepButton->dumpObj();
this->stepCounter->dumpObj();
}
