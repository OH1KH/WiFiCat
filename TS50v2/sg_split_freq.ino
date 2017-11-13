
// case 'i': get_split_freq(); break; 
// case 'I': set_split_freq(); break;   
//-------------------------------------------------------
void get_split_freq() {
   // get TX split freq
   String Ans = "RPRT -1\n";
   String retFreq = "0";
    //first get RX vfo
    theIF();
    // Freq,rit,rit_on,xit_on,Mem_ch,TX_on,mode_is,vfo,split 
    if (validIF) { 
     // then get opposite (TX) vfo frequency
     if ( vfo != "MEM" ) {
       if ( vfo == "VFOA" ) retFreq = GetFreq("FB"); else retFreq = GetFreq("FA");    
       Ans = "RPRT 0\n";
    } //not mem
   }// valid if
   
     if (longch == '\n') {
       writeCli(retFreq+ "\n");
       writeCli(Ans);
      }
     else {
      if (longch == '+') longch = '\n';
      writeCli("get_split_freq:"+String(longch)+"TX Frequency: "+retFreq+String(longch)+Ans);
      }
 }
//-------------------------------------------------------
void set_split_freq() {
  String Ans = "RPRT -1\n";
  String retFreq = RigCmd ;
    // set  split TX vfo 
   if (validFreq()) {
   //first get RX vfo
    theIF();
    // Freq,rit,rit_on,xit_on,Mem_ch,TX_on,mode_is,vfo,split 
    if (validIF) { 
     // then set opposite (TX) vfo frequency
     if ( vfo != "MEM" ) {
       if ( vfo == "VFOA" ) {
         writeHost("FB"+RigCmd+";");
         Ans = "RPRT 0\n";
       } else { 
         writeHost("FA"+RigCmd+";");
         Ans = "RPRT 0\n";
       }
     } // if !Mem
    } // valid IF
   } //falid freq
  if (longch == '\n')
       writeCli(Ans);
     else {
      if (longch == '+') longch = '\n';
      writeCli("set_split_freq:"+String(longch)+"TX Frequency: "+retFreq+String(longch)+Ans);
      }
 } 
//-------------------------------------------------------

