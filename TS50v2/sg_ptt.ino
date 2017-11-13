
//-------------------------------------------------------
   
void get_ptt() {
    // get TX state
    theIF();
    if (validIF) {
    if (longch == '\n')
       writeCli(TX_on+'\n');
     else {
      if (longch == '+') longch = '\n';
      writeCli("get_ptt:"+String(longch)+"PTT:"+String(longch)+TX_on+String(longch)+"RPRT 0\n");
      }
     } else {  if (longch == '\n') writeCli("RPRT -1\n");
         else writeCli("get_ptt:"+String(longch)+"PTT:"+String(longch)+"RPRT 1\n");
       }
} 
//-------------------------------------------------------
void set_ptt() {
    // stat/stop TX
    String rep ="";
    char pt = RigCmd.charAt(0);
    switch (pt) {
      case '0': {
            writeHost("RX;");
            if (longch == '\n')
                rep="RPRT 0\n";
              else {
                 if (longch == '+') longch = '\n';
                rep="set_ptt:"+String(longch)+"PTT:"+String(longch)+pt+String(longch)+"RPRT 0\n";
              }
            }
       break;
      case '1': {
           writeHost("TX;");
           if (longch == '\n')
                rep="RPRT 0\n";
              else {
                 if (longch == '+') longch = '\n';
                 rep="set_ptt:"+String(longch)+"PTT:"+String(longch)+pt+String(longch)+"RPRT 0\n";
              }
            }     
       break;
      default :{
         if (longch == '\n')
                rep="RPRT -1\n";
              else {
                 if (longch == '+') longch = '\n';
                 rep="set_ptt:"+String(longch)+"PTT:"+String(longch)+pt+String(longch)+"RPRT -1\n";
              }
      }     
     break;  
     } // T  
    writeCli(rep); 
}
//-------------------------------------------------------
