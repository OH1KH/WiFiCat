
void loop() { 
  //if ((millis()-TimeCw) > PerCw){
   iambic();
  //  TimeCw = millis();
  //}
  if ((millis()-TimeCon) > PerData) {
     readCli(); //process requests
     TimeData = millis();
  }
  if ((millis()-TimeCon) > PerCon) {
     MDNS.update();
     serveTCP();//check connects and disconnects
     TimeCon = millis();
  }
}


