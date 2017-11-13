
//-------------------------------------------------------
void writeHost(String command) {
  //host is rig  @ serial
  Serial.print(command);
  delay(RepDelay);//delay before next command
}
//------------------------------------------------------
void readHostWriteCli() {
 //check UART for data and pass it to tcp
 CliNr = 0;
  if(Serial.available()){
      if (serverClients[CliNr] && serverClients[CliNr].connected()){
        while(Serial.available()) serverClients[CliNr].write(  Serial.read() );
        yield();
      } 
  } // if rig sent something
}
//------------------------------------------------------
void readCliWriteHost () {
    CliNr = 0;
    if (serverClients[CliNr] && serverClients[CliNr].connected()){
      if(serverClients[CliNr].available()){
        //get data from the telnet client and push it to the UART
        while(serverClients[CliNr].available()) Serial.write(  serverClients[CliNr].read()  );
      }
    }
}
