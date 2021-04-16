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

Network::Network() : status( WIFI_NOT_CONNECTED ){
	assert( this->status_mutex = xSemaphoreCreateMutex() );	// Initialize mutex object
}

void Network::setStatus( enum net_status_t v ){
	xSemaphoreTake( this->status_mutex, portMAX_DELAY );
	this->status = v;
	gui->updateNetwork();
	xSemaphoreGive( this->status_mutex );
}

enum Network::net_status_t Network::getStatus( void ){
	enum net_status_t res;
	xSemaphoreTake( this->status_mutex, portMAX_DELAY );
	res = this->status;
	xSemaphoreGive( this->status_mutex );
	return res;
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

/* Tasks: https://www.freertos.org/implementing-a-FreeRTOS-task.html
 *
 * mutex : https://www.freertos.org/Real-time-embedded-RTOS-mutexes.html
 * 	- take : acquire the mutex
 * 	- give : release the mutex
 * 
 */
void Network::connect( void ){
}

Network network;

