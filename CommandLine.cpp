/************************************************
*	Command Line : Serial command
*
*************************************************/
#include <Arduino.h>
#include <time.h>
#include <sys/time.h>

#include "CommandLine.h"

/* Set the system date
 * format : YYYYMMDD.HHmmSS
 */
void setdate( const String &arg ){
	struct tm tm;

	tm.tm_year = atoi( arg.substring(0,4).c_str() ) - 1900;
	tm.tm_mon = atoi( arg.substring(4,6).c_str() ) - 1;
	tm.tm_mday = atoi( arg.substring(6,8).c_str() );

	tm.tm_hour = atoi( arg.substring(9,11).c_str() );
	tm.tm_min = atoi( arg.substring(11,13).c_str() );
	tm.tm_sec = atoi( arg.substring(13).c_str() );

	tm.tm_isdst = 0;

	struct timeval tv;
	tv.tv_sec = mktime( &tm );
	tv.tv_usec = 0;

	Serial.print( "Setting time to : " );
	Serial.print( ctime(&tv.tv_sec) );
}

const struct _command {
	const char *nom;
	const char *desc;
	void (*func)( const String & );
} commands[] = {
	{ "setDate", "Set date and time : YYYYMMDD.HHMMSS", setdate },
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
		}
	}
}
