

//-------------------------------------------------------
void send_morse() {// send string as CW 
    if (longch == '\n')
     writeCli("RPRT 0\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("send_morse: "+longch+RigCmd+longch+"RPRT 0\n");
     }
    MorseTxEsp_SSend(cwSpeed,RigCmd);
    }
//-------------------------------------------------------
