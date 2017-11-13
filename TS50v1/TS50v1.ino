#define vers "v1.2"

/*
 * v1.0 
 * v 1.1   KY/KS emulation in TS50 level to get cw keying from preset memories.(note this needs fix also to rigctl ts550 desciptions= Can send morese Y)
 * v 1.2   winkey emulation (basic) and rigctld emulation for cw
 * ToDo:
 * multiatasking
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
#define MAX_SRV_CLIENTS 1 // as simulates serial line, only one client bossible.!!!
#define PORT 50000

WiFiServer server( PORT ); // define telnet port here
WiFiClient serverClients[MAX_SRV_CLIENTS];
WiFiServer cerver( PORT + 1); // define telnet port here
WiFiClient cerverClients[MAX_SRV_CLIENTS];
uint8_t CliNr = 0; //client #nr // as simulates serial line, only one client bossible.!!!
//set wifi hostname  !! array len string+1
char HOST[5] = "TS50";

String RigCmd = "";          //received command from telnet
String RigResp = ""; 

#define RepDelay 5  //wait reply ms after sent serial cmd

#define EETOP 5             //bytes reserved from eeprom
#define ditpad 4                //iambic keyer pad pins
#define dahpad 5
#define CWOUT 14                  //cw keying out
#define CWmax 50                  //WPM
#define CWmin 5
byte WinKey = 0;                  //very basic winkey emulation (speed + text) on/off (off kenwood KSwpm, KYtext in use)
byte WinState =0;                   //keyed text state 0=none, 1= started, 
byte CWdit = ditpad;                 //iambic keyer pad pins default
byte CWdah = dahpad;
byte Idit = 1;                  // keyer state pressed set by interrupts
byte Idah = 1;
unsigned int cwSpeed = 22;     
byte CWstate = 0;		//keyer state
String  msg ="";                //text to send
String  msg1 ="";
String  msg2 ="";
String  msg3 ="";
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

