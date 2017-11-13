Kenwood TS50s cat interface with ESP12e WiFi

This is a Beta test version. Feel free to make it better and share it (GPL3)!
No warranty.
The author will not be held liable for any damages you make by using these documents and programs.

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


TODO:

	For both programs: Make ISR based CW.
	Now rig polling will interrupt iambic keying. And Text to CW commands will stop rig 
	polling during CW output. Bad!




