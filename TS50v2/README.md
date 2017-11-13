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


    WiFICat emulates rigctld commands of Hamlib. See rigctld manpage for help. Only TS50s specific commands are in use.
    For using this with your programs you should set your rigctld "rig" as type 2 (Hamlib Net) choose right IP address (see WiFiseup) and port 4532
    that is default rigctld port.

    WiFICat allows several connects (set from source) so you can set up cqrlog and wsjtx for this and it really works!
    I have worked several FT8 qsos without any problems.
    If your WiFi network is weak and WiFICat looses it randomly all kinds of wrong CAT issues may happen.

    Again: remember that 4-5sec polling rate for both programs is fast enough for normal qsos.
    At cqrlog you can also set CW keying as "Hamlib" and your CW macros will work.
    WSJT-X use "radio/split: fake" It is the best. You can try with "split" but soon you will notice that wsjtx is too curious and makes funny checks
    all the time.



TODO:
   Make ISR based CW. Now rig polling will interrupt iambic keying. And Text to CW commands will stop rig polling during CW output.
