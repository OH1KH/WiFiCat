
void rx_resp() {
#ifdef USE_UDP
  rx_udp();
#else
  serveTCP();//check connects and disconnects
  rx_tcp();
#endif
}

#ifndef USE_UDP
//-------------------------------------------------------
void serveTCP() {
   
  //check if there is client connecting
  if (server.hasClient()){
      if (!client1.connected()){
        if (client1) client1.stop();
        client1 = server.available();
        //Serial.print("New client");
      }
    
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
    //Serial.print("Disc");
  }
 }  
//-------------------------------------------------------
void rx_tcp() {
    
  //check TCP clients for commands
    
    if (client1.connected()){
      if(client1.available()){
          while (client1.available() > 0) { 
             packetBuffer[0]=client1.read();
             noBytes = 1;
             in_data();
             yield();
           } // while char    
      } // if client nr has data
    } // if client nr is connected
  
  
}
//-------------------------------------------------------
void tx_tcp() {
  if ( client1.connected()){
    client1.write(sendBuffer,TxBufPtr);
    yield();
   }
}
#endif
