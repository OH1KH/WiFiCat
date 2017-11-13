

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
 }  
//-------------------------------------------------------
void readCli() {
  boolean rdy = false; 
  //check TCP clients for commands
  for(CliNr = 0; CliNr < MAX_SRV_CLIENTS; CliNr++){
    if (serverClients[CliNr] && serverClients[CliNr].connected()){
      if(serverClients[CliNr].available()){
        
        //get data from the telnet client and push it to the UART
        while(serverClients[CliNr].available()) { // serve clients in order
          unsigned long LastCh = millis();
          
          while( (serverClients[CliNr].available() > 0)  && ((millis()-LastCh) < IFtimeout)) { //wait for end of string or timeout
             char OneRead=serverClients[CliNr].read();
             if ((OneRead == '\n')) {               //end of string
               ParseCommand();
               RigCmd = ""; 
              } 
            if ((OneRead > 31) && (OneRead < 127))  RigCmd += OneRead;   
            yield();
          } // while char
          
           RigCmd = ""; //flush if not complete otherwise already flushed at parser         
        } // while curr client
        
      } // if client nr has data
      
    } // if client nr is connected
    
  } // for all clients
}
//-------------------------------------------------------
void writeCli(String answer) {
  // client is TCP
  size_t len = answer.length() +1;
  char sbuf[len];
  answer.toCharArray(sbuf,len);
  if (serverClients[CliNr] && serverClients[CliNr].connected()){
    serverClients[CliNr].write(sbuf, len-1);
    yield();
   }
}

//-------------------------------------------------------
void writeHost(String command) {
   // host is rig @ serial
    Serial.print(command);
    delay(RepDelay);//delay before next command
    
}

