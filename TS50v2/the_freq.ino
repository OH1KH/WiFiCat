
//-------------------------------------------------------
boolean validFreq() { //validate frequency and add 0s

   RigCmd.trim(); //remove all but freq
   int l=RigCmd.indexOf('.');
   if (l>-1) RigCmd = RigCmd.substring(0,l); //remove decimals
   if (RigCmd.toInt()> 0) { 
       while ( RigCmd.length() < 11 ) RigCmd = "0"+RigCmd; //pad 0 to get 11 chrs
     return true;  
   } else { return false;}
}  

//-------------------------------------------------------
String GetFreq(String v) { //asks freq of given vfo (FA, FB)returns to client
  char RespF[200];
  int Rptr;
  char OneRead;
  unsigned long Lastf;
  boolean validF = false;
  repeats = Rtimes;
  String Vfreq = "RPRT -1\n";
  while ((validF == false) && (repeats > 0 )) {
   writeHost(v+";");
   Lastf = millis();  
   Rptr=0;
   //wait for response
   while ((!Serial.available()) && ((millis()-Lastf) < IFtimeout)) yield(); //wait for response string or timeout
   if (Serial.available())  {
    LastIf = millis();
    while( ((millis()-Lastf) < IFtimeout)) { //wait for end of string or timeout
      if (Serial.available())  {
        OneRead=Serial.read();
      
        if ((OneRead == ';') && (Rptr > 10) && (RespF[0]=='F')) {               //end of long enough F string
        Vfreq="";
        boolean nozero=false;
        for (int i=2; i<13; i++) {
          if((RespF[i]!='0') && !nozero) nozero=true; //strip leading 0s
          if ( nozero ) Vfreq = Vfreq+RespF[i];
         }      
       validF = true;
       Serial.flush();
      } //end of response
    
    if ((OneRead > 31) && (OneRead<127)) {
      RespF[Rptr] = OneRead;
      Rptr++;
     }
    
     yield();
      }//one read 
    } //read chars to timeout
   } //if first char in buffer
    repeats--;
  } //repeats
  return Vfreq;
}


