/************************************************
*	Networking
*************************************************/
#ifndef NETWORK_H
#define NETWORK_H

enum net_status_t {
	NOT_CONNECTED = 0,
	CONNECTING,
	CONNECTED
};

extern net_status_t net_status;

#endif
