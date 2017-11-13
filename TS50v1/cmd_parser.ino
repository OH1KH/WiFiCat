
 //-------------------------------------------------------
void ParseCommand(){  
    if (WinKey == 0) {
     if (RigCmd.indexOf("KY",0) > -1 ) {
      if (RigCmd.charAt(2) == ';') {  //cw buffer asked
        RigResp = "KY0;";
        writeCli(RigResp);
       } else { // send cw: this is more flexible than Kenwoods own. String does not need to be caps and length is not fixed 24 bytes
        RigCmd.setCharAt(0,32);
        RigCmd.setCharAt(1,32);
        RigCmd.setCharAt(int(RigCmd.length()-1),32);
        RigCmd.trim();
        if (RigCmd == "") Idit = 0; //emulate pad pressing to stop sending ! Does not work as no multitasking while CW out
         else MorseTxEsp_SSend(cwSpeed,RigCmd); 
       }
       RigCmd = "";
     }
     if (RigCmd.indexOf("KS",0) > -1 ) {
       if (RigCmd.charAt(2) == ';') {  //cw speed asked
        RigResp = "KS0"+String(cwSpeed)+';';
        writeCli(RigResp);
        RigResp = "";
        RigCmd = "";
       } else {   //speed setting
        byte a = RigCmd.substring(2,5).toInt();
        setCWspeed(a); 
       }
       RigCmd ="" ; 
     }




     
    } //winkey=0 
  yield();
}
//------------------------------------------------------- 

/*
 * winkey open cmd 0x00,0x02 resp 0x5E, 0x57
 * winkey close cmd 0x00,0x023 resp none
 * open: keying as ascii, resp 0x4D , 0x2D , 0x44the text0x4D , 0x2D , 0x40
 * speed 0x02,0xWPM resp none
 * close: keying the text comes out no resp !!! but no speed change possible
 */
