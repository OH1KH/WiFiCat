#ifdef USE_UDP
void rx_udp() {
  noBytes = udp.parsePacket(); 
  if ( noBytes ) {
    remote_IP = udp.remoteIP();
    remote_Port = udp.remotePort();
    udp.read(packetBuffer,noBytes);
    in_data();
    if (RxBufPtr == 0 ){ 
      udp.flush();
     }
  }
  yield();
}
#endif
void tx_resp() {  
   
   if (Serial.available() > 0) {
    char c = Serial.read();
    sendBuffer[TxBufPtr] = c;
    TxBufPtr++;
    //Serial.print(c);
    if (TxBufPtr>UDP_PACKET_SIZE-1) {TxBufPtr = UDP_PACKET_SIZE-1;} //no overflow
   }
   
   if (sendBuffer[TxBufPtr-1] == ';') {
      sendBuffer[TxBufPtr] = '\0';
      #ifdef USE_UDP
      tx_udp();
      #else
      tx_tcp();
      #endif
      TxBufPtr =0;
   } 
}
#ifdef USE_UDP
void tx_udp() {
      udp.beginPacket(remote_IP, remote_Port);
      udp.write(sendBuffer,TxBufPtr);
      udp.endPacket(); 
}
#endif

boolean check_cmd() {
 /* Commands:
  *  KY-1   :send Memory 1
  *  KY-2   :send Memory 2
  *  KY+1YYY: Set Memory 1 to YYY
  *  KY+2YYY: Set Memory 2 to YYY
  *  KYabcde: Send CW abcde
  */


  boolean IsCmd = false;

  if ( (char(CWoutBuffer[0]) == 'K') && (char(CWoutBuffer[1]) =='Y')) { 
    
   if (char(CWoutBuffer[2]) =='-') {  
              if (char(CWoutBuffer[3]) == '1') {
                MorseTxEsp_CSend(cwSpeed,msg1txt);
                }
              if (char(CWoutBuffer[3]) == '2') {
                MorseTxEsp_CSend(cwSpeed,msg2txt);
                }
    } else { // -
      if (char(CWoutBuffer[2]) == '+') {
              if (char(CWoutBuffer[3]) == '1') {
                for (int i=0;i<100;i++) {
                  if (char(CWoutBuffer[4+i])!= ';') {
                  msg1txt[i] = CWoutBuffer[4+i]; 
                  } else {
                  msg1txt[i] = '\0';
                  eewcharA(4, msg1txt, 100);
                  break;
                  }
                }
             } else {
             if (char(CWoutBuffer[3]) == '2') {
               for (int i=0;i<100;i++) {
                  if (char(CWoutBuffer[4+i])!= ';') {
                  msg2txt[i] = CWoutBuffer[4+i]; 
                  } else {
                  msg2txt[i] = '\0';
                  eewcharA(105,msg2txt, 100);
                  break;
                  } 
               }
             }
            } 
      } else { // +
      for (int f=2;f<UDP_PACKET_SIZE;f++) {
       if (char(CWoutBuffer[f])!= ';') {
         CWoutBuffer[f-2] = CWoutBuffer[f]; 
         } else {
          CWoutBuffer[f-2] = '\0';
          MorseTxEsp_CSend(cwSpeed,CWoutBuffer);
          }
      }
     }
  } //+
   IsCmd = true;
 } // K && Y
 return IsCmd;
} 

void in_data() {
    for (int f=0;f<noBytes;f++) {
      if ((packetBuffer[f] > 31) && (packetBuffer[f] < 127)) { 
       CWoutBuffer[RxBufPtr] = packetBuffer[f];
       RxBufPtr++;
       if (RxBufPtr>UDP_PACKET_SIZE-1) {RxBufPtr = UDP_PACKET_SIZE-1;} //no overflow
       if (char(packetBuffer[f]) == ';') {  
        CWoutBuffer[RxBufPtr] = '\0';
        if (!check_cmd()) {
          for (int i=0;i<RxBufPtr;i++) {
            Serial.print(CWoutBuffer[i]);
            delay(1);
            }       
          }
        RxBufPtr = 0;
        break;
       }
     } // if ascii
    }  
}

