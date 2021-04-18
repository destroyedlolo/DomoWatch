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
	xSemaphoreGive( this->status_mutex );
	gui->updateNetwork();
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
		if( this->getStatus() != net_status_t::WIFI_NOT_CONNECTED )
			this->setStatus( net_status_t::WIFI_NOT_CONNECTED );

		return net_status_t::WIFI_NOT_CONNECTED;
	} else {
		if( this->getStatus() == net_status_t::WIFI_NOT_CONNECTED )
			this->setStatus( net_status_t::WIFI_CONNECTED );

		return this->getStatus();
	}
}

/* Tasks: https://www.freertos.org/implementing-a-FreeRTOS-task.html
 *
 * mutex : https://www.freertos.org/Real-time-embedded-RTOS-mutexes.html
 * 	- take : acquire the mutex
 * 	- give : release the mutex
 * 
 */

void connectTask( void * ){
	vTaskDelay( 1000 );
	network.setStatus( Network::net_status_t::WIFI_CONNECTED );
	Serial.println("Network is connected");
	vTaskDelete( NULL );	// existing the task
}

void Network::connect( void ){
	this->setStatus( net_status_t::WIFI_CONNECTING );
	Serial.println("Network is connecting");
	xTaskCreate( connectTask, "connectTask",
		8000,	// stack
		NULL,	// no parameter
		2,		// priority
		NULL	// no task handle
	);
}

void Network::disconnect( void ){
	this->setStatus( net_status_t::WIFI_NOT_CONNECTED );
	Serial.println("Network is disconnected");
}

Network network;

