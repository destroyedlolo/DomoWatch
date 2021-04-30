/************************************************
*	Networking
*************************************************/
#ifndef NETWORK_H
#define NETWORK_H

#include <AsyncMqttClient.h>

class Network {
public:
	enum net_status_t {
		WIFI_NOT_CONNECTED = 0,
		WIFI_FAILED,	// Failed attempt to connect or to communicate
		WIFI_CONNECTING,
		WIFI_MQTT,		// MQTT connected as well to the broker
		WIFI_CONNECTED
	};

		/*	Which capacities are enabled
		 *
		 * Dev note : use _BV() macro to get corresponding value
		 */
	enum net_capacity_bits {
		NET_CAP_WIFI = 0,	// WiFi connected
		NET_CAP_MQTT		// %QTT connected
	};
	typedef uint8_t net_capacity_t;

private:
		/* Network status */
	enum net_status_t status;
	SemaphoreHandle_t status_mutex;

		/* Slave tasks */
	uint32_t STCounter;
	SemaphoreHandle_t STC_mutex;

	AsyncMqttClient mqttClient;

public:
	Network();

		/* Set status and update the Gui */
	void setStatus( enum net_status_t v );

		/* Get the logical status */
	enum net_status_t getStatus( void );

		/* get the status based on status field but also
		 * ensure the network is really connected
		 */
	enum net_status_t getRealStatus( void );

		/* Determine if the network is active (in application
		 * point of view) or not
		 *
		 * Thie network is active ONLY if it's connected
		 */
	bool isActive( enum net_status_t v = (enum net_status_t)-1 );

		/* get which capacity is enabled (bitwise) */
	net_capacity_t getCapacity( void );

		/* Slave tasks counter
		 *
		 * Notez-bien : these function may lock status
		 */
	void increaseSTC( void );
	void decreaseSTC( void );
	bool isSlaveTaskRunning( void );

		/* WiFi related
		 * the GUI is updated
		 */
	void connect( void );
	void disconnect( void );

		/* MQTT related
		 * the GUI is updated
		 */
	void MQTTconnect( void );
	void MQTTdisconnect( bool force=false );
	bool MQTTconnected( void );
	uint16_t MQTTsubscribe(const char* topic, uint8_t qos=0);
};

extern Network network;

#endif
