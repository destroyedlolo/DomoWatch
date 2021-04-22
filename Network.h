/************************************************
*	Networking
*************************************************/
#ifndef NETWORK_H
#define NETWORK_H

class Network {
public:
	enum net_status_t {
		WIFI_NOT_CONNECTED = 0,
		WIFI_FAILED,	// Failed attempt to connect or to communicate
		WIFI_CONNECTING,
		WIFI_BUSY,		// Something is on way
		WIFI_CONNECTED
	};

private:
		/* Network status */
	enum net_status_t status;
	SemaphoreHandle_t status_mutex;

		/* Slave tasks */
	uint32_t STCounter;
	SemaphoreHandle_t STC_mutex;

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
		 * or busy.
		 */
	bool isActive( enum net_status_t v = (enum net_status_t)-1 );

		/* Slave tasks counter
		 *
		 * Notez-bien : these function may lock status
		 */
	void increaseSTC( void );
	void decreaseSTC( void );
	bool isSlaveTaskRunning( void );

		/* (dis)connect to the WiFi
		 * the GUI is updated
		 */
	void connect( void );
	void disconnect( void );
};

extern Network network;

#endif
