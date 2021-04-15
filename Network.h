/************************************************
*	Networking
*************************************************/
#ifndef NETWORK_H
#define NETWORK_H

class Network {
public:
	enum net_status_t {
		WIFI_NOT_CONNECTED = 0,
		WIFI_CONNECTING,
		WIFI_BUSY,		// Something is on way
		WIFI_CONNECTED
	};

private:
	enum net_status_t status;

public:
	Network() : status( WIFI_NOT_CONNECTED ){}

		/* Set status and update the Gui */
	void setStatus( enum net_status_t v );

		/* Get the logical status */
	enum net_status_t getStatus( void ){
		return this->status;
	}

		/* get the status based on status field but also
		 * ensure the network is really connected
		 */
	enum net_status_t getRealStatus( void );

		/* (dis)connect to the WiFi
		 * the GUI is updated
		 */
	bool connect( void );
	void disconnect( void );
};

extern Network network;

#endif
