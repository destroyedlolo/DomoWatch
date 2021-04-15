/************************************************
*	Networking
*************************************************/

#include "Gui.h"

#include <WiFi.h>

	/* 
	 * Include my own home network definition 
	 * ...
	 */
/*
#if defined __has_include
#	if __has_include(<Maison.h>)
#		include <Maison.h>
#	endif
#endif
*/

/* well, __has_include() seems not working with Arduino IDE.
 * comment tout this include and define your granding bellow
 */
#include <Maison.h>

	/*
	 * ... otherwise, define yours
	 */
#ifndef WIFI_SSID
#	define WIFI_SSID "YourSSID"
#	define WIFI_PASSWORD "YourPassword"
#endif

#include "Network.h"

void Network::setStatus( enum net_status_t v ){
	this->status = v;
	gui->updateNetwork();
}

enum Network::net_status_t Network::getRealStatus( void ){
	if(WiFi.status() != WL_CONNECTED){
		if( this->status != net_status_t::WIFI_NOT_CONNECTED )
			this->setStatus( net_status_t::WIFI_NOT_CONNECTED );

		return net_status_t::WIFI_NOT_CONNECTED;
	} else {
		if( this->status == net_status_t::WIFI_NOT_CONNECTED )
			this->setStatus( net_status_t::WIFI_CONNECTED );

		return this->status;
	}
}

Network network;

