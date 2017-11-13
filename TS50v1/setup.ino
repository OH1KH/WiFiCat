

void setup() {
  delay(100);
  RigCmd.reserve(100);
  //serial setup
  Serial.setTimeout(2000);
  Serial.begin(4800,SERIAL_8N2);
  while(!Serial) { } // Wait for serial to initialize.
  //wifi setup
  wifi_station_set_hostname(HOST);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN,HIGH);
  delay(10);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(OnBoardLed, OUTPUT);
  digitalWrite(OnBoardLed, LedOff);
   /* eeprom
    *  0 checsum
    *  1 cwdit paddle
    *  2 cwdah paddle
    *  3 cwspeed
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
      a = eerbyte(3);  
     if ((a >= CWmin) && (a <= CWmax)) cwSpeed = a; // must be in limits
   }
   //MorseTxEsp_setup
  pinMode(CWOUT,OUTPUT);
  pinMode(CWdit,INPUT);
  pinMode(CWdah,INPUT);
  led_I();
  for (byte l=0;l<10;l++) {
  if (digitalRead(CWdah) == LOW ) {  //want reverse pads = keep wanted dit down during restart
    byte a = CWdit;
    CWdit = CWdah; //iambic keyer pad pins swithed
    CWdah = a; 
    l=30; //exit loop
   }
   delay(100);
  }
  led_R();
  if((eerbyte(1) != CWdit ) || (eerbyte(2) != CWdah )){ //write if changed
     eewbyte(1,CWdit);
     eewbyte(2,CWdah);
     eesum();
   }
  //wifisetup
  for (int i=0; i<150; i++) { 
     if (i%10 == 0 )  digitalWrite(OnBoardLed, LedOn);
     if (i%20 == 0 )  digitalWrite(OnBoardLed, LedOff);
     delay(50);
     //trigger pin or both paddles down to enter wifi setup.
     if (( digitalRead(TRIGGER_PIN) == LOW ) || ( (digitalRead(CWdit)==LOW) && (digitalRead(CWdah)==LOW) ))  wifiSetup(); 
   }
  digitalWrite(OnBoardLed, LedOff);
  delay(100);
  
  //morse interrupts
  attachInterrupt(CWdit,dit_int,FALLING);
  attachInterrupt(CWdah,dah_int,FALLING);

  //rigctld emualtion tcp start    
  WiFi.hostname(HOST);
  if (!MDNS.begin(HOST)) led_N; //Serial.println("Error setting up MDNS responder!");

  server.begin();
  server.setNoDelay(true);
  delay(5);
  cerver.begin();
  cerver.setNoDelay(true);
  delay(5);
  MDNS.addService("telnet","tcp", PORT); 
  MDNS.addService("telnet","tcp", PORT + 1); 
  
  writeHost("ID;");

}

