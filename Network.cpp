/************************************************
*	Networking
*************************************************/

	/* 
	 * Include my own home network definition 
	 * ...
	 */
#if defined __has_include
#	if __has_include("Maison.h")
#	endif
#endif

	/*
	 * ... otherwise, define yours
	 */
#ifndef WIFI_SSID
#	define WIFI_SSID "YourSSID"
#	define WIFI_PASSWORD "YourPassword"
#endif

#include "Network.h"

net_status_t net_status;

