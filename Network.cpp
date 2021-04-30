/************************************************
*	Networking
*
*	References used here :
*
* 	mutex : 
*	-------
*
* 	https://www.freertos.org/Real-time-embedded-RTOS-mutexes.html
* 		- take : acquire the mutex
* 		- give : release the mutex
*
*
*	WiFi events :
*	-------------
*
* From ESP32, .../libraries/WiFi/examples/WiFiClientEvents/WiFiClientEvents.ino
*
* NOTE : if not disabled, the WiFi may (or not) reconnect automatically at wakeup.
* Otherwise, the WiFi is disabled by the code bellow.
*
*************************************************/

#include "Gui.h"
#include "InterTskCom.h"
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

#	define BROKER_HOST "Broker_Host_OrIP"
#	define BROKER_PORT 1883
#endif

#include "Network.h"

Network network;

	/* Receiving random network notification
	 * for debugging purposes.
	 */

static void debugWiFiEvent(WiFiEvent_t event){
	Serial.printf("[WiFi-event] event: %d\n", event);

    switch (event) {
        case SYSTEM_EVENT_WIFI_READY: 
            Serial.println("WiFi interface ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case SYSTEM_EVENT_STA_START:
            Serial.println("WiFi client started");
            break;
        case SYSTEM_EVENT_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case SYSTEM_EVENT_AP_START:
            Serial.println("WiFi access point started");
            break;
        case SYSTEM_EVENT_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case SYSTEM_EVENT_GOT_IP6:
            Serial.println("IPv6 is preferred");
            break;
        case SYSTEM_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case SYSTEM_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
        default:
			break;
    }
}

static void getConnecting( WiFiEvent_t event, WiFiEventInfo_t info ){
	network.setStatus( Network::net_status_t::WIFI_CONNECTING );
}

static void getConnected( WiFiEvent_t event, WiFiEventInfo_t info ){
	network.setStatus( Network::net_status_t::WIFI_CONNECTED );
}

static void getStop( WiFiEvent_t event, WiFiEventInfo_t info ){
	network.setStatus( Network::net_status_t::WIFI_NOT_CONNECTED );

	// Remove old configuration
	// seems mandatory otherwise the connection seems random if
	// the watch position changed
	WiFi.disconnect(true);
}

static void getDisconnected( WiFiEvent_t event, WiFiEventInfo_t info ){
	network.setStatus( Network::net_status_t::WIFI_FAILED );
	WiFi.mode(WIFI_OFF);
}


static void getMQTTConnected( bool ){
    Serial.println("MQTT Connected");
	network.setStatus( Network::net_status_t::WIFI_MQTT);

		/* Subscriptions are lost when mQTT get disconnected.
		 * Consequently, we need to resubmit everything even
		 * if we did it before connection loss
		 */
   	Serial.println("Subscribing ...");
	gui->subscribe();
}

static void getMQTTDisconnected( AsyncMqttClientDisconnectReason r ){
	network.setStatus( WiFi.status() == WL_CONNECTED ? Network::net_status_t::WIFI_CONNECTED : Network::net_status_t::WIFI_NOT_CONNECTED );

    Serial.print("MQTT Disconnected due to ");
	switch(r){
	case AsyncMqttClientDisconnectReason::TCP_DISCONNECTED : Serial.println( "TCP_DISCONNECTED" ); break;
	case AsyncMqttClientDisconnectReason::MQTT_UNACCEPTABLE_PROTOCOL_VERSION : Serial.println( "MQTT_UNACCEPTABLE_PROTOCOL_VERSION" ); break;
	case AsyncMqttClientDisconnectReason::MQTT_IDENTIFIER_REJECTED : Serial.println( "MQTT_IDENTIFIER_REJECTED" ); break;
	case AsyncMqttClientDisconnectReason::MQTT_SERVER_UNAVAILABLE : Serial.println( "MQTT_SERVER_UNAVAILABLE" ); break;
	case AsyncMqttClientDisconnectReason::MQTT_MALFORMED_CREDENTIALS : Serial.println( "MQTT_MALFORMED_CREDENTIALS" ); break;
	case AsyncMqttClientDisconnectReason::MQTT_NOT_AUTHORIZED : Serial.println( "MQTT_NOT_AUTHORIZED" ); break;
	case AsyncMqttClientDisconnectReason::ESP8266_NOT_ENOUGH_SPACE : Serial.println( "ESP8266_NOT_ENOUGH_SPACE" ); break;
	case AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT : Serial.println( "TLS_BAD_FINGERPRINT" ); break;
	default :
		Serial.println( "Unknown" );
	}
}

static void getMQTTMessage( char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total ){
	char t[len+1];
	strncpy(t, payload, len);
	t[len] = 0;

	Serial.printf("Received t:'%s' m:'%s'\n", topic, t);
	gui->msgreceived( topic, t );
}

Network::Network() : status( WIFI_NOT_CONNECTED ), STCounter(0){
		// Initialize mutex object
	assert( this->status_mutex = xSemaphoreCreateMutex() );
	assert( this->STC_mutex = xSemaphoreCreateMutex() );

	WiFi.onEvent( debugWiFiEvent );
	WiFi.onEvent( getConnecting, WiFiEvent_t::SYSTEM_EVENT_WIFI_READY );	// hardware ready to connect
	WiFi.onEvent( getConnected, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP );		// WiFi fully connected
	WiFi.onEvent( getDisconnected, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED );	// get disconnected
	WiFi.onEvent( getStop, WiFiEvent_t::SYSTEM_EVENT_STA_STOP );			// WiFi switch off

	this->mqttClient.onConnect(getMQTTConnected);
	this->mqttClient.onDisconnect(getMQTTDisconnected);
	this->mqttClient.onMessage(getMQTTMessage);
	this->mqttClient.setServer(BROKER_HOST, BROKER_PORT);
}

void Network::setStatus( enum net_status_t v ){

	xSemaphoreTake( this->status_mutex, portMAX_DELAY );
	this->status = v;
	xSemaphoreGive( this->status_mutex );
	gui->updateNetwork();

	xEventGroupSetBits( itc_signals, WATCH_WIFI_CHANGED );
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

bool Network::isActive( enum net_status_t v ){
	if( v == (enum net_status_t)-1 )
		v = this->status;	// No need to mutex as we won't block if a value is provided as argument

	return( v == net_status_t::WIFI_MQTT || v == net_status_t::WIFI_CONNECTED );
}

Network::net_capacity_t Network::getCapacity( void ){
	net_capacity_t res = 0;

	switch( this->getStatus() ){
	case WIFI_CONNECTED :
		res = _BV(NET_CAP_WIFI);
		break;
	case WIFI_MQTT :
		res = _BV(NET_CAP_WIFI) | _BV(NET_CAP_MQTT);
		break;
	default :
		;
	}

	return res;
}

void Network::connect( void ){
	Serial.println("Network is connecting");
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void Network::disconnect( void ){
	if( this->MQTTconnected() )
		this->MQTTdisconnect( true );	// Enforce MQTT to disconnect (to be clean)

	WiFi.mode(WIFI_OFF);
	Serial.println("Network is disconnecting");
}

void Network::MQTTconnect( void ){
	this->mqttClient.connect();
}

void Network::MQTTdisconnect( bool force ){
	this->mqttClient.disconnect( force );
}

bool Network::MQTTconnected( void ){
	return(this->mqttClient.connected());
}

uint16_t Network::MQTTsubscribe(const char* topic, uint8_t qos){
	return(this->mqttClient.subscribe( topic, qos ));
}

void Network::increaseSTC( void ){
	xSemaphoreTake( this->STC_mutex, portMAX_DELAY );
	if( ++(this->STCounter) == 1 )	// First task launched
		xEventGroupSetBits( itc_signals, WATCH_WIFI_CHANGED );	// Request GUI update
	xSemaphoreGive( this->STC_mutex );
}

void Network::decreaseSTC( void ){
	xSemaphoreTake( this->STC_mutex, portMAX_DELAY );
	if( !(--this->STCounter) )	// Last task finished
		xEventGroupSetBits( itc_signals, WATCH_WIFI_CHANGED );	// Request GUI update
	xSemaphoreGive( this->STC_mutex );
}

bool Network::isSlaveTaskRunning( void ){
	xSemaphoreTake( this->STC_mutex, portMAX_DELAY );
	bool res = (this->STCounter != 0);
	xSemaphoreGive( this->STC_mutex );

	return res;
}

/* Obsolete code : try to use background tasks.
 * For Wifi event if far better for now
 *
 * Tasks: https://www.freertos.org/implementing-a-FreeRTOS-task.html
 * 
 */

#if 0	// Trying with background tasks
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
#endif

