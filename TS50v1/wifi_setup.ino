//-------------------------------------------------------
void wifiSetup() {
   //Local intialization. Once its business is done, there is no need to keep it around
    digitalWrite(OnBoardLed, LedOn);
      while  ( digitalRead(TRIGGER_PIN) == LOW ) delay(5);
    digitalWrite(OnBoardLed, LedOff);
    delay(200);
    digitalWrite(OnBoardLed, LedOn);
 
    WiFiManager wifiManager;

 
    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    wifiManager.setTimeout(180);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);
    
    if (!wifiManager.startConfigPortal(HOST)) {
       delay(2000);
      //reset and try again, or maybe put it to deep sleep
      //reset settings - for testing
      wifiManager.resetSettings();
      led_N();
      ESP.reset();
      delay(5000);
      }
    //if you get here you have connected to the WiFi 
      led_R();
      
  } //Configuration portal end

