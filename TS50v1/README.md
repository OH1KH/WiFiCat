Kenwood TS50s cat interface with ESP12e WiFi


CW paddles, dot and dash:
	When ESP starts it blinks it's led 2 times "CW - i". After that push the paddle that you want dots from. Few seconds later "CW - R" is
        blinked to infor that paddle setting time is over. Paddle state is written to eprom, so you do not need to do this always.
        After that several long blinks "CW T-T-T-T-T" is blinked.
        If you need to set up WiFi push both paddles down. Blinking stops and led stays on. Now you have 3 minutes time to connect open network
        "TS50" and open browser http://192.168.4.1 to get into WiFi setup page.
        There you can connect to existing WiFi network and save settings to eeprom. If you do not have WiFi at all you can use WiFiCat from default
        address 192.168.4.1.
	If your WiFi network has changed do "reset" and then a new setup.


        WiFiCAT can set your TX on and off, so you do not need any PTT transistor or relay.        



     This is simple serial-to-tcp system in addition with iambic cq-bug and Kenwood (TS-480) Cw commands KS (keying speed) and KY (keyed text).
     KY is extended so that it supports longer text to send than original Ky command that has a limited buffer size.
     As alternative for KS/KY there is simple Winkeyer emulation (just speed setting and winkey on/off).
    /*
     * winkey open cmd 0x00,0x02 resp 0x5E, 0x57
     * winkey close cmd 0x00,0x023 resp none
     * open: keying as ascii, resp 0x4D , 0x2D , 0x44the text0x4D , 0x2D , 0x40
     * speed 0x02,0xWPM resp none
     * close: keying the text comes out no resp !!! but no speed change possible
     */
     Kenwoods KS/KY commands are default.


     Linux:
     Make directory /dev to your home dir (mkdir dev). Install socat if you have not yet done so.
     Give console command: socat -v -x pty,link=$HOME/dev/vmodem0,raw,echo=0,waitslave tcp:xxx.xxx.xxx.xxx:50000
     
     Where xxx.xxx.xxx.xxx is the IP address you got when connecting this device to your WiFi network ot use 192.168.4.1 if you did not make
     network connection and device's default accesspoin is used for operation.
     port 50000 is used for serial-to-tcp connections and carries commands prom PC to TS50s CAT.
     port 50001 is used for CW speed and text inputs.

     If all is ok you should have now pseudo serial port called $HOME/dev/vmodem0. Because -v option was used you can see traffic between your 
     program and rig. Put the pseudo port to your programs config as "rig serial port". Speed settings does not matter as ESP works with TS50 always
     at same 4800-8-n-2 speed.

     Do same kind of pseudo port for CW interface with console command: socat -v -x pty,link=$HOME/dev/vmodem1,raw,echo=0,waitslave tcp:xxx.xxx.xxx.xxx:50001

     These pseudo ports are closing when program quits. If you need "always ready ports use "while loop" console command:
     while true; do socat pty,link=$HOME/dev/vmodem0,raw,echo=0,waitslave tcp:xxx.xxx.xxx.xxx:50000; done
   
     Set your programs poll rate somewhere over 4sec. You do not need faster info about rig state! Qsos take longer!



     Windows:
     Widows has similar programs than socat for making serial-over-tcp pseudo ports. Sorry, there is no Windozes in house so I can not help further.
     Use Google.


TODO:
   Make ISR based CW. Now rig polling will interrupt iambic keying. And Text to CW commands will stop rig polling during CW output.
