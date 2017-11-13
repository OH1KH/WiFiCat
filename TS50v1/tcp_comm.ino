

//-------------------------------------------------------
void serveTCP() {
   
  //check if there are any new clients connecting
  if (server.hasClient()){
    for(CliNr = 0; CliNr < MAX_SRV_CLIENTS; CliNr++){
      //find free/disconnected spot
      if (!serverClients[CliNr] || !serverClients[CliNr].connected()){
        if(serverClients[CliNr]) serverClients[CliNr].stop();
        serverClients[CliNr] = server.available();
        // Serial.print("New client: #"); Serial.println(CliNr);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }

   
  //check if there are any new clients connecting
  if (cerver.hasClient()){
    for(CliNr = 0; CliNr < MAX_SRV_CLIENTS; CliNr++){
      //find free/disconnected spot
      if (!cerverClients[CliNr] || !cerverClients[CliNr].connected()){
        if(cerverClients[CliNr]) cerverClients[CliNr].stop();
        cerverClients[CliNr] = cerver.available();
        // Serial.print("New client: #"); Serial.println(CliNr);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient cerverClient = cerver.available();
    cerverClient.stop();
  }
 }  
//-------------------------------------------------------
boolean readCli() {
  boolean r = false;
  char OneRead;
  //check TCP client for commands, just the only one !!
    CliNr = 0;
    if (cerverClients[CliNr] && cerverClients[CliNr].connected()){
      if(cerverClients[CliNr].available()){
        //get data from the telnet client 
          while(cerverClients[CliNr].available() > 0) { 
             OneRead=char(cerverClients[CliNr].read());
             //check winmode cmds first
               if (OneRead == 0) { //open / close
                if (cerverClients[CliNr].available() > 0) { 
                  OneRead=char(cerverClients[CliNr].read());
                  if (OneRead== 2) {  //open
                      WinKey = 1;
                      RigResp = "^W";  //0x5E, 0x67
                      writeCli(RigResp);
                      RigCmd ="";
                      led_I();
                  }
                  if (OneRead == 3) { // close
                      WinKey = 0;
                      RigCmd ="";
                      led_N();
                  }  
                }
             } //case 0x00
             if (OneRead == 2) { // set speed
               if (cerverClients[CliNr].available() > 0) { 
                  OneRead=char(cerverClients[CliNr].read());
                  setCWspeed(byte(OneRead));
               RigCmd ="";
               led_E();
              }
             } // case 0x02
             
              
           if (WinKey == 0) {
            if ((OneRead > 31) && (OneRead < 127))  RigCmd += OneRead; 
             r = (OneRead == ';');
             if (r) {
              RigCmd.trim();
              return r;
              }
             } 
             
             if (WinKey == 1) { //winkey mode
              if ((OneRead > 31) && (OneRead < 127)) {
                if (WinState == 0) {
                  writeCli("M-D"); // '0x4D'+'0x2D'+'0x44';
                  WinState = 1;
                }
                  RigCmd += OneRead;
              }
             }
             
            yield();
          } // while char 
      } // while client has data
      if ((WinKey ==1) && (WinState == 1)) {
          MorseTxEsp_SSend(cwSpeed,RigCmd); 
          RigResp = "M-@"; //0x4D'+'0x2D'+'0x40';
          writeCli(RigResp);
          RigCmd ="";
          WinState = 0;
      } else return r; 
    }//while connected
   RigCmd = "";
   return r;     
}
//-------------------------------------------------------
void writeCli(String answer) {
  // client is TCP
  CliNr = 0;
  size_t len = answer.length() +1;
  char sbuf[len];
  answer.toCharArray(sbuf,len);
  if (cerverClients[CliNr] && cerverClients[CliNr].connected()){
    cerverClients[CliNr].write(sbuf, len-1);
    yield();
   }
}




