Kenwood TS50s cat interface with ESP12e WiFi

This is a Beta test version. Feel free to make it better and share it (GPL3)!
No warranty.
The author will not be held liable for any damages you make by using these documents and programs.

This program has 2 versions. Common for both programs are the WiFi setup and cw paddle setup.

	NOTE: 	WiFiCat can set your TX on and off, so you do not need any serial 
		port and PTT transistor or relay for it


Folders:

	TS50v1 - based on serial2tcp version
	TS50v2 - based on rigctld emulation
	TS50v3 - based on serial2tcp or serial2udp version. With UDP has also WEBserver
	doc    - some documents
	ISR_CW - first try to make ISR based iambic keyer.


CW paddles, dit and dah:
	
	When WiFiCat starts it blinks it's led 2 times "CW - i".
	After that push the paddle that you want to get dots from. 
	Few seconds later "CW - R" is blinked to inform that paddle setting time is over. 
	Paddle state is written to eprom, so you do not need to do this always.
        
WiFi setup:

	After that several long blinks "CW T-T-T-T-T" is blinked.
	If you need to set up WiFi netwok connection push both paddles down. Blinking stops 
	and led stays on. Now you have 3 minutes time to connect open network "TS50" and open browser 
	Go to http://192.168.4.1 to get into WiFi setup page. Firefox can detect setup page and
	offers button for moving sign up page.
	There you can connect to existing WiFi network and save settings to eeprom. 
	If you do not have WiFi network at all you can use WiFiCat as accesspoint from 
	default address 192.168.4.1.
	If your WiFi network has changed since last setup or you do not have network at all 
	any more do "reset" and in the case of new wifi network, a new setup.


Version depended properties:

TS50v1:

	This is simple serial-to-tcp system in addition with iambic cq-bug and Kenwood (TS-480)
	Cw commands KS (keying speed) and KY (keyed text). KY is extended so that it supports longer 
	text to send than original Ky command that has a limited buffer size.
	As alternative for KS/KY there is simple Winkeyer emulation 
	(just speed setting and winkey on/off).
	/*
	* winkey open cmd 0x00,0x02 resp 0x5E, 0x57
	* winkey close cmd 0x00,0x023 resp none
	* open: keying as ascii, resp 0x4D , 0x2D , 0x44the text0x4D , 0x2D , 0x40
	* speed 0x02,0xWPM resp none
	* close: keying the text comes out no resp !!! but no speed change possible
	*/
	Kenwoods KS/KY commands are in use by default.

	 NOTE:  Rigctld rig model 201 (TS50s) is untested and have several problems.
		- serial speed should be 4800-8-n-2stop as default, nothing else works 
		 (with WiFiCat it does not matter as ESP handles always serial speed)
		- "get_mode command issuse "MD" that TS50s can not understand and answers just "?"
		- also some split related commands have "someting mysterious".
		 (all these are fixed in version TS50v2 emulation code)


	Linux:
	======
	Make directory /dev to your home dir (mkdir dev). Install socat if you have not yet done so.
	Give console command: 
	socat -v -x pty,link=$HOME/dev/vmodem0,raw,echo=0,waitslave tcp:xxx.xxx.xxx.xxx:50000
	Where xxx.xxx.xxx.xxx is the IP address you got when connecting this device to your 
	WiFi network or use 192.168.4.1 if you did not make network connection and device's 
	default setup accesspoint is used for operation.
	port 50000 is used for serial-to-tcp connections and carries commands prom PC to TS50s CAT.
	port 50001 is used for CW speed and text inputs.
	
	If all is ok you should have now pseudo serial port called $HOME/dev/vmodem0. 
	Because -v option was used you can see traffic between your  program and rig. 
	Put the pseudo port to your programs config as "rig serial port". 
	Speed settings does not matter as ESP works with TS50 always at same 4800-8-n-2 speed.
	
	Do same kind of pseudo port for CW interface with console command: 
	socat -v -x pty,link=$HOME/dev/vmodem1,raw,echo=0,waitslave tcp:xxx.xxx.xxx.xxx:50001

        (Replace line's "tcp" with "udp" if using v3 version and UDP in selected. See below.)
	
	These pseudo ports close when program quits. If you need "always ready" ports 
	use "while loop" console command:
	while true; do socat pty,link=$HOME/dev/vmodem0,raw,echo=0,waitslave tcp:xxx.xxx.xxx.xxx:50000; done
	
	Set your programs poll rate somewhere over 4sec. 
	You do not need faster info about rig state! your qsos take longer!


	Windows:
	========
	
	Widows has similar programs than socat for making serial-over-tcp pseudo ports.
	Sorry, there are no Windozes in house so I can not help further.
	Use Google search for finding suitable program..


TS50v2:

	WiFICat emulates rigctld commands of Hamlib. See rigctld manpage for help. 
	Only TS50s specific commands are in use. For using this with your programs you should set
	your  rigctld "rig type" 2 (Hamlib Net) choose right IP address (see WiFi setup) 
	and port 4532 that is default rigctld port.
	
	WiFICat allows several connects (set from source) so you can set up for example
	both cqrlog and wsjtx to use WiFiCat  and it really works!
	I have worked several FT8 qsos with that kind of setup without any problems.
	If your WiFi network is weak and WiFICat looses it randomly all kinds of CAT 
	issues may happen.
	
	Again: remember that 4-5sec, or even longer, polling rate for both programs is fast 
	enough for normal qsos.
	
	At cqrlog preferences you can also set CW keying to use "Hamlib" and your CW macros
	and CW from keyboard will work.
	In WSJT-X config use "radio/split: fake" It is the best. 
	You can try with "split" but soon you will notice that wsjtx is too curious 
	and makes funny checks all the time that makes receiving impossible.


TS50v3:

	Generates only own accesspoint, not able to connct exisiting WiFi networks. (for portable use only, perhaps)
	CW keyer has 2 memories stored in EEPROM.
	CW speed is controlled via potentiometer.
	Some "KY" based commands to modify and send memories or plain text in CW. (Not passed to rig)
	Connection to rig CAT is done either via UDP or TCP at 192.168.4.1 port 4535 using raw data mode.
	(udp/tcp 2 serial).

        If UDP link to rig is used then has also Web server (with captive dns) to set memories and paddle order.
        Handy in expeditions using smart phone's WiFi connect.

        If TCP is used then no Web server can be used (ESP12 property) but CW memories can be modified
        with "KY"-commands.

        Paddle order can always be set (stored in EEPROM) also by pressing paddle that should be the "dit"-one
        at powerup after ESP's internal led blinks 2 times.

TODO:

	For all programs: Make better ISR based CW.
	Now rig polling will interrupt iambic keying. And Text to CW commands will stop rig 
	polling during CW output. Bad!
	Perhaps integrating to ESP32.




