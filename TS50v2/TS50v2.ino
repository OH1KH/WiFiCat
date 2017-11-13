#define vers "v2.1"

/*
 * v1.0 only short commands
 * v 1.1 long commands added)
 * v 2.0 new command parser, splitted source
 * v 2.1 tested commands
 * v 2.2 fixed syntax
 * v 2.3 setting dit and dah padles reverse by ptessing wanted dit at start
 *       accessing WiFisetup also by pressing dit and dah padless while led blinks at start
 *       saving dit and dah paddles and cwspeed to eeprom, and reding them at start
 * ToDo:
 * wsjtx can not set freq (read is ok)
*/
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#define TRIGGER_PIN 0              // access to WiFimanager when onboard led is blinking 
#define OnBoardLed 2               //ESP's onboard led pin (Check! May vary)
#define LedOn LOW                 //ESP's onboard led illuminated (Check! May vary, too)
#define LedOff HIGH               //ESP's onboard led NOT illuminated 
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManage


//how many clients should be able to telnet to this TS50 
//recommend: USE 1. If you need more connects stack rigctlds. Put one running @ localhost and tell that
//it's "rig" is "-m2" (rigctld) and  point it to TS50-WiFi with proper IP address:
//  rigctld -m2 --set-conf=rig_pathname=192.168.60.220:4532,timeout=3000 -vvvvv

#define MAX_SRV_CLIENTS 3
WiFiServer server(4532);  // this is standard rigctld's port
WiFiClient serverClients[MAX_SRV_CLIENTS];
uint8_t CliNr; //client #nr
//set wifi hostname  !!
char HOST[5] = "TS50";

String RigCmd = "";          //received command from telnet
char longch = '\n';

unsigned long TimeCon;        //timeslots for events (ms)
#define PerCon 10
unsigned long TimeCw;
#define PerCw 1
unsigned long TimeData;
#define PerData 5

unsigned long Lastf;
unsigned long LastIf = 0;    // last time IF; is issued, we trust values under sec old.
boolean validIF = false;
String Freq,rit,rit_on,xit_on,Mem_ch,TX_on,mode_is,passband,vfo,split = ""; // values from IF-command
#define Rtimes 3
int repeats = Rtimes;    //repeats to try command (down count)
#define RepDelay 10  //wait ms between repeats.
#define IFtimeout 900 //timeout for response ms
#define IFmaxValid 1000  //how old IF result we accept without new poll (set this big for debug:)
boolean ok = false;

#define EETOP 5             //bytes reserverd from eeprom
#define ditpad 4                //iambic keyer pad pins
#define dahpad 5
#define CWOUT 14                  //cw keying out
#define CWmax 50                  //WPM
#define CWmin 5
#define CwTurnInit 5000             //turns to keep in looping iambic after no paddle detection
int CwTurns =CwTurnInit;;                  
byte CWdit = ditpad;                 //iambic keyer pad pins default
byte CWdah = dahpad;
byte Idit = 1;                  // keyer state pressed set by interrupts
byte Idah = 1;
unsigned int cwSpeed = 22;     
byte CWstate = 0;		//keyer state
unsigned int duration;
char morseLib[63] [6]= {
                        { 0,0,0,0,0,0 }, // <space>
                        { 1,1,3,3,1,0 }, // !
                        { 0,0,0,0,0,0 }, // "
                        { 0,0,0,0,0,0 }, // #
                        { 0,0,0,0,0,0 }, // $
                        { 0,0,0,0,0,0 }, // %
                        { 0,0,0,0,0,0 }, // &
                        { 0,0,0,0,0,0 }, // '
                        { 0,0,0,0,0,0 }, // (
                        { 0,0,0,0,0,0 }, // )
                        { 0,0,0,0,0,0 }, // *
                        { 0,0,0,0,0,0 }, // +
                        { 3,3,1,1,3,3 }, // ,
                        { 3,1,1,1,1,3 }, // -
                        { 1,3,1,3,1,3 }, // .
                        { 3,1,1,3,1,0 }, // /
                        { 3,3,3,3,3,0 }, //0
                        { 1,3,3,3,3,0 }, //1
                        { 1,1,3,3,3,0 }, //3
                        { 1,1,1,3,3,0 }, //3
                        { 1,1,1,1,3,0 }, //4
                        { 1,1,1,1,1,0 }, //5
                        { 3,1,1,1,1,0 }, //6
                        { 3,3,1,1,1,0 }, //7
                        { 3,3,3,1,1,0 }, //8
                        { 3,3,3,3,1,0 }, //9
                        { 0,0,0,0,0,0 }, // :
                        { 0,0,0,0,0,0 }, // ;
                        { 0,0,0,0,0,0 }, // <
                        { 3,1,1,1,3,0 }, // =
                        { 0,0,0,0,0,0 }, // >
                        { 1,1,3,3,1,1 }, // ?
                        { 0,0,0,0,0,0 }, // @
                        { 1,3,0,0,0,0 }, //a
                        { 3,1,1,1,0,0 }, //b
                        { 3,1,3,1,0,0 }, //c
                        { 3,1,1,0,0,0 }, //d
                        { 1,0,0,0,0,0 }, //e
                        { 1,1,3,1,0,0 }, //f
                        { 3,3,1,0,0,0 }, //g
                        { 1,1,1,1,0,0 }, //h
                        { 1,1,0,0,0,0 }, //i
                        { 1,3,3,3,0,0 }, //j
                        { 3,1,3,0,0,0 }, //k
                        { 1,3,1,1,0,0 }, //l
                        { 3,3,0,0,0,0 }, //m
                        { 3,1,0,0,0,0 }, //n
                        { 3,3,3,0,0,0 }, //o
                        { 1,3,3,1,0,0 }, //p
                        { 3,3,1,3,0,0 }, //q
                        { 1,3,1,0,0,0 }, //r
                        { 1,1,1,0,0,0 }, //s
                        { 3,0,0,0,0,0 }, //t
                        { 1,1,3,0,0,0 }, //u
                        { 1,1,1,3,0,0 }, //v
                        { 1,3,3,0,0,0 }, //w
                        { 3,1,1,3,0,0 }, //x
                        { 3,1,3,3,0,0 }, //y
                        { 3,3,1,1,0,0 }, //z
                        { 1,3,3,1,3,0 }, // [ Å (a with round)
                        { 3,3,3,1,0,0 }, // \ Ö (o with dots)
                        { 1,3,1,3,0,0 }, // ] Ä (a with dots)
                   };
//-------------------------------------------------------
//------------------------------------------------------




/* 
  Ts-50 CAT interface. (c) OH1KH -2017
  This work is based on several examples collected together
  
   -WiFiTelnetToSerial - Copyright (c) 2015 Hristo Gochkov. 
    This file is part of the ESP8266WiFi library for Arduino environment.
  
   -WiFiManager          //https://github.com/tzapu/WiFiManager

  - SoftwareSerial Implementation of the Arduino software serial for ESP8266.
    Copyright (c) 2015-2016 Peter Lerup. All rights reserved.

  This program and it's parts are free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  A  copy of the GNU Lesser General Public License can be found from
  the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

