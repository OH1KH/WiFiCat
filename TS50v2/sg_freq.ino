
//-------------------------------------------------------
void get_freq() {
   // get operating frequency
    theIF();
    if (validIF) {
    if (longch == '\n')
       writeCli(Freq+"\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("get_freq:"+String(longch)+"Frequency: "+Freq+String(longch)+"RPRT 0\n");
      }
     } else {  if (longch == '\n') writeCli("RPRT -1\n");
         else       
         writeCli("get_freq:"+String(longch)+"Frequency: "+String(longch)+"RPRT -1\n");
       }
    }
//------------------------------------------------------- 
void set_freq() {
   // set operating frequency
   String Ans = "RPRT -1\n";
   String retFreq = "";
   if (validFreq()) {
      retFreq = RigCmd;
      while ( (retFreq.charAt(0)=='0') && (retFreq.length()>0)) { //remove leading zeroes from answer
        retFreq.setCharAt(0,32);
        retFreq.trim();
      }
   theIF();
    // Freq,rit,rit_on,xit_on,Mem_ch,TX_on,mode_is,vfo,split 
    if (validIF) {
      if( vfo == "VFOA") {
        writeHost("FA"+RigCmd+";");
        Ans= "RPRT 0\n";
      } else {
        if( vfo == "VFOB") {
        writeHost("FB"+RigCmd+";");
         Ans= "RPRT 0\n";          
        } 
      }
    } //validIF
   }//valid Freq
   if (longch == '\n')
       writeCli(Ans);
     else {
      if (longch == '+') longch = '\n';
      writeCli("set_freq:"+String(longch)+"Frequency: "+retFreq+String(longch)+Ans);
      }
  }
//-------------------------------------------------------

