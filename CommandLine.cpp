/************************************************
*	Command Line : Serial command
*
*************************************************/
#include <Arduino.h>
#include <time.h>
#include <sys/time.h>

#include "Gui.h"
#include "CommandLine.h"
#include "Network.h"

/* Set the system date
 * format : YYYYMMDD.HHmmSS
 */
void cmd_setdate( const String &arg ){
	struct tm tm;

	tm.tm_year = atoi( arg.substring(0,4).c_str() ) - 1900;
	tm.tm_mon = atoi( arg.substring(4,6).c_str() ) - 1;
	tm.tm_mday = atoi( arg.substring(6,8).c_str() );

	tm.tm_hour = atoi( arg.substring(9,11).c_str() );
	tm.tm_min = atoi( arg.substring(11,13).c_str() );
	tm.tm_sec = atoi( arg.substring(13).c_str() );

	struct timeval val;
	val.tv_sec = mktime(&tm);
    val.tv_usec = 0;

	settimeofday(&val, NULL);

	ttgo->rtc->syncToRtc();

	Serial.printf("Set date to %s", ctime( &val.tv_sec ));
	Serial.printf("RTC %s\n", ttgo->rtc->formatDateTime( PCF_TIMEFORMAT_YYYY_MM_DD_H_M_S ) );
}

/* display RTC time */
void cmd_rtc( const String & ){
	Serial.printf("RTC %s\n", ttgo->rtc->formatDateTime( PCF_TIMEFORMAT_YYYY_MM_DD_H_M_S ) );
}

/* Reboot the ESP */
void cmd_reboot( const String & ){
	ESP.restart();
}

/* Activate or not the WiFi */
void cmd_wifi( const String &arg ){
	if( arg == "on" )
		network.connect();
	else
		network.disconnect();
}

/* Activate or not the WiFi */
void cmd_MQTT( const String &arg ){
	if( arg == "on" )
		network.MQTTconnect();
	else
		network.MQTTdisconnect();
}

const struct _command {
	const char *nom;
	const char *desc;
	void (*func)( const String & );
} commands[] = {
	{ "setDate", "Set date and time : YYYYMMDD.HHMMSS", cmd_setdate },
	{ "rtc", "get stored RTC time", cmd_rtc },
	{ "reboot", "reboot the watch", cmd_reboot },
	{ "WiFi", "WiFi on/off", cmd_wifi },
	{ "MQTT", "MQTT on/off", cmd_MQTT },
	{ NULL, NULL, NULL }
};

void CommandLine::loop( void ){
	while( Serial.available() ){
		String cmd = Serial.readString();
		const int idx = cmd.indexOf(' ');
		String arg;

		if(idx != -1){ // Separate command and argument
			arg = cmd.substring(idx + 1);
			cmd = cmd.substring(0, idx);
		}

		if( cmd == "?" ){
			if( arg.length() ){	// Help on a particular command
				for( const struct _command *cmd = commands; cmd->nom; cmd++ ){
					if( arg == cmd->nom && cmd->desc ){
						Serial.println( (arg + ":" + cmd->desc).c_str() );
						break;
					}
				}
			} else { // List known commands
				Serial.print("Known commands :");
				for( const struct _command *cmd = commands; cmd->nom; cmd++ ){
					Serial.print( " " );
					Serial.print( cmd->nom );
				}
				Serial.println( "" );
			}
		} else for( const struct _command *icmd = commands; icmd->nom; icmd++ ){
			if( cmd == icmd->nom ){
				icmd->func( arg );
			}
		}
		lv_disp_trig_activity(NULL); // avoid twatch sleeping
	}
}
