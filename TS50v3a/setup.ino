void setup(){
Serial.begin(4800,SERIAL_8N2);
Serial.print(WiFi.softAPConfig(local_IP, gateway, subnet) ? ";" : "");
Serial.print(WiFi.softAP(HOST) ? ";" : "");


  pinMode(mem1_pin, INPUT_PULLUP);
  pinMode(mem2_pin, INPUT_PULLUP);  
  pinMode(OnBoardLed, OUTPUT);
  digitalWrite(OnBoardLed, LedOff);
   /* eeprom
    *  0 checsum
    *  1 cwdit paddle
    *  2 cwdah paddle
    *  3 
    *  4 msg1txt (101bytes)
    *  5 msg1txt (101bytes)
    */
   if (eestart()) {
     byte a = eerbyte(1);  // zero eeprom has checksum ok !!
     byte b = eerbyte(2);
     if ((((a == ditpad) || (a == dahpad)) && (a != b)) && (((b == ditpad) || (b == dahpad)) && (a != b))) { //must be in limits and not equal
        CWdit = a;
        CWdah = b; 
      } else {            //iambic keyer pad pins default
        CWdit = ditpad;
        CWdah = dahpad;       
      }
     
     //mem 1 & 2
      eercharA(4, msg1txt, 100);
      eercharA(105,msg2txt, 100);
   }
   //MorseTxEsp_setup
  pinMode(CWOUT,OUTPUT);
  pinMode(CWdit,INPUT);
  pinMode(CWdah,INPUT);
  led_I();
  for (byte l=0;l<15;l++) {
  if (digitalRead(CWdah) == LOW ) {  //want reverse pads = keep wanted dit down during restart
    byte a = CWdit;
    CWdit = CWdah; //iambic keyer pad pins swithed
    CWdah = a; 
    led_R();
    l=20; //exit loop
   }
   delay(100);
  }
  if((eerbyte(1) != CWdit ) || (eerbyte(2) != CWdah )){ //write if changed
     eewbyte(1,CWdit);
     eewbyte(2,CWdah);
     eesum();
   }

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  Serial.print(dnsServer.start(53, "*", local_IP) ? ";" : ""); //prints CAT eol (;) of ok else nothing
  Serial.println(MDNS.begin(HOST) ? ";" : "");
  
 #ifdef USE_UDP   
  servo.on("/", handleRoot);
  servo.on("/action", handleAction);
  servo.onNotFound(handleNotFound);
  servo.begin();  
  MDNS.addService("http", "tcp", 80);

  udp.begin(udpPort);
#else
  server.begin(); 
  server.setNoDelay(true);
  MDNS.addService("telnet","tcp", 4535);
#endif

 //morse interrupts
 attachInterrupt(digitalPinToInterrupt(CWdit),dit_int,FALLING);
 attachInterrupt(digitalPinToInterrupt(CWdah),dah_int,FALLING);

 delay(500);
 Serial.print("ID;");
 
}
