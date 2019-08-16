#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

#define OnBoardLed 2               //ESP's onboard led pin (Check! May vary)
#define LedOn LOW                 //ESP's onboard led illuminated (Check! May vary, too)
#define LedOff HIGH               //ESP's onboard led NOT illuminated 

#define EETOP 220             //bytes reserved from eeprom
#define ditpad 4            //iambic keyer pad pins
#define dahpad 5
#define mem1_pin 13           //keyer mem1
#define mem2_pin 12           //keyer mem2
#define CWOUT 14            //cw keying out
#define CWmax 50            //WPM
#define CWmin 5

#define USE_UDP  //comment out if you like to use tcp connect. NOTE !!! -> TCP and web server do not work at same time.

DNSServer dnsServer;

#ifdef USE_UDP
ESP8266WebServer servo(80);
WiFiUDP udp;
#else
WiFiServer server(4535);
WiFiClient client1;
#endif
 
const int UDP_PACKET_SIZE = 200;     
byte packetBuffer[UDP_PACKET_SIZE];  //buffer to hold packets
char sendBuffer[UDP_PACKET_SIZE] = {0};
char CWoutBuffer[UDP_PACKET_SIZE] = {0};
byte TxBufPtr = 0;
byte RxBufPtr = 0;

unsigned long Nextmillis = 0;
char vers[]= "1.0";
char HOST[] = "TS50";
byte CWdit = ditpad;        //iambic keyer pad pins default
byte CWdah = dahpad;
bool Idit = false;              // keyer state pressed set by interrupts
bool Idah = false;
unsigned int cwSpeed = 22;     
byte CWstate = 0;    //keyer state
char  msg1txt[101] ="";                //text to send
char  msg2txt[101] ="";
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

IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
IPAddress remote_IP(224,0,0,1); // multicast until set by receiving command
int remote_Port = 4535; // changes by receiving command
int udpPort = 4535; //port to listen serial commands              
int noBytes;
//-------------------------------------------------------
// introducing of procedures and functions @ other tabs
//-----------------------------------------------------
//-------------------------------------------------------
//cw
//-------------------------------------------------------
void iambic();
void tone_on();
void tone_off();
void letterspace() ;
void wordspace();
void ditah(int e);
void MorseTxEsp_CSend(unsigned int wpm,char *t);
void MorseTxEsp_SSend(unsigned int wpm,String ceewee);
void wpmTOduration(unsigned int wpm);
void ICACHE_RAM_ATTR dit_int();
void ICACHE_RAM_ATTR dah_int();
void setCWspeed(byte a);
//-------------------------------------------------------
//eeprom
boolean eecheck();
void eesum();
boolean eestart();
void eewbyte(int pos, byte dat);
void eewint(int pos, int dat);
void eewcharA(int pos, char * dat, int sz);
int eerint(int pos);
byte eerbyte(int pos);
void eercharA(int pos, char * dat, int sz);
//-------------------------------------------------------
// led_cw
void led_R();
void led_N();
void led_I();
void led_E();
//------------------------------------------------------
//tcp
void rx_resp();
#ifndef USE_UDP
void serveTCP();
void rx_tcp();
void tx_tcp();
#endif
//------------------------------------------------------
//udp
#ifdef USE_UDP
void rx_udp();
#endif
void tx_resp(); 
#ifdef USE_UDP
void tx_udp(); 
#endif
boolean check_cmd();
void in_data() ;
//------------------------------------------------------
//web
void handleRoot();
void handleAction();
void handleNotFound();
//-------------------------------------------------------
void setup();
//------------------------------------------------------
void loop();

