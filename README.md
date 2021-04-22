<p align="center">
<img src="https://img.shields.io/github/last-commit/destroyedlolo/DomoWatch.svg?style=for-the-badge" />
&nbsp;
<img src="https://img.shields.io/github/license/destroyedlolo/DomoWatch.svg?style=for-the-badge" />
</p>
<hr/>

# DomoWatch

The final goal of this project is to controle my smart home automation using TTGO TWatch.

In addition, the code has been made in **POO way** and being **very modular** (and commented as well) : should be a good starting point for new projects.

## Install

This project can be build using **Arduino IDE** and needs :
- [Xinyuan-LilyGO /TTGO_TWatch_Library](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library) and dependancies to be installed
- [emToile](https://github.com/destroyedlolo/emToile) 

## Code convention

  - **header files** (\*.h) are containing class definition and **C++ ones** the implementation of heavy methods. They have the same name as the class implemented.
  - **Tl\* files** are for tiles definitions which are ordered inside Gui's constructor (**Gui.cpp**)

## User guide

### Configuration

It looks to me very tedious to modify some configuration using a so tiny screen. Consequently some parameters can only be set using the serial console or, worst, simply hard coded in the source.

#### Networking

**Wifi SSID** and corresponding **Password** are hardcoded in *Network.cpp*.
Don't forget to comment out
```
   #include <Maison.h>
```
which is for my very own usage.

##### To connect
click on the WiFi icon on the status bar :
- **White** : not connected (including after disconnect)
- **Red** : an error occurred (please disconnect and then reconnect)
- **Orange** : Connection in progress
- **Green** : Connected (click on the icon again to disconnect)

### GUI

The **main tile** only displays the current date and time with a **status bar** containing *battery level* and a *step counter*. Clicking on it to reset.

```
			 ~~~~~~~~~~~~
			l Net stuff  l
			 ~~~~~~~~~~~~
			     /l\
			      l
			     \l/
	 --------	 -------------
	| Status | <->	| Time & Date |
	 -------- 	 -------------
			    /|\
			     |
			    \|/
			 -----------
			| Scrn set  |
			 -----------
```
  - On its left, the *status tile* that is displaying some watch's figures (consumption, RAM, ...).
  - On its bottom, the *Screen settings* : take in account it's value is not stored but in memory. In case of a reboot, previous value is lost.
  - On its top, the network related tile. It is only available when the WiFi is connected

### Power management

DomoWatch has 2 sleep mode to save battery power.

### Light Sleep Mode

In this mode, only energy hungry peripherals are stopped :
  - the screen is off
  - the CPU is frozen but the memory is kept as it was

*Advantages* : very fast, the watch is waken up within a second

*Battery lifetime* : in this mode, with an average usage and keeping network disabled, the battery expectancies is a bit less than 2 days.

*Enter in light sleep* : 
  - short (< 1.5 seconds by default) press on the bezel button.
  - let the screen inactive the configured period.
 
 The inactive period can have 2 values, and both can be set in the screen settings
  - a *shorter* one is used when the network is off (30s by default)
  - a *longer* one is used when the network is activated (60s by default)

In case a network transaction is on way (like time synchronisation), a 3rd *unlimited* period is used to let the transaction to complete.

*Waking up* :
  - press the bezel button
  - put the watch screen in front of your face
 
### Deep sleep

*Currently disabled as it is trashing the touchscreen*

In this mode, the everything is stopped but obviously the real time clock and the step counter.

*Disadvantage* : the watch tooks 3 seconds to wake up.

*Wakeup* :
  - press the bezel button

### shutdown

The watch can be totally shutdown by holding the bezel button for 6 seconds.

(useful in case the watch is freezing)

### Commands line

Most of settings (date & time, networks, ...) can be set using the console command line. The special command '**?**' without argument display the list a known commands. With an argument, a short description of the given command.

	?
	Known commands : setDate rtc reboot
	? setDate
	setDate:Set date and time : YYYYMMDD.HHMMSS

---

This code is inspirited from examples coming with TTGO_TWatch_Library made by *Lewis He*.
Icons came from 
* [sharandac/My-TTGO-Watch's project](https://github.com/sharandac/My-TTGO-Watch/)
* [http://www.icons101.com/](http://www.icons101.com/)
