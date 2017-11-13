//-------------------------------------------------------
void theIF() {
  //result strings are global
  //IF00014257337     +00700  0002000108 ;
  char RespIF[200];
  int Rptr;
  char OneRead;
  if (millis()-LastIf > IFmaxValid) { //last IF was more than Xsec ago, take new
  validIF = false;
  repeats =Rtimes;
  while ((validIF == false) && (repeats > 0 )) {
    //writeCli("IF repeats:"+String(repeats));
   writeHost("IF;");
   LastIf = millis();  
   Rptr=0;
   //wait for response
   while ((!Serial.available()) && ((millis()-LastIf) < IFtimeout)) yield(); //wait for response string or timeout
   if (Serial.available()) {
    LastIf = millis();
    while((millis()-LastIf) < IFtimeout) { //wait for end of string or timeout
      if (Serial.available()) {
        OneRead=Serial.read();
  
       if ((OneRead == ';') && (Rptr > 36) && (RespIF[0]=='I') && (RespIF[1]=='F')) {               //end of long enough IF string
       Freq="";
       boolean nozero=false;
       for (int i=2;i<13;i++) {
        if((RespIF[i]!='0') && !nozero) nozero=true; 
        if ( nozero ) Freq = Freq+RespIF[i];
        }
       rit="";
       for (int i=18;i<23;i++) {
        rit = rit+RespIF[i];
        }
       rit_on = RespIF[23];
       Mem_ch = RespIF[26]+RespIF[27];
       TX_on = RespIF[28];
       switch (RespIF[29]) {
        case '1':
         mode_is = "LSB";
         passband = "2400";
        break;
        case '2':
         mode_is = "USB";
         passband = "2400";
        break;
        case '3':
         mode_is = "CW";
         passband = "2400";
        break;
        case '4':
         mode_is = "FM";
         passband = "15000";
        break;
        case '5':
         mode_is = "AM";
         passband = "5000";
        break;
         }//eoswitch
       switch (RespIF[30]) {
        case '0':
         vfo = "VFOA";
        break;
        case '1':
         vfo = "VFOB";
        break;
        case '2':
         vfo = "MEM";
        break;
         }//eoswitch
       split = RespIF[32];
       validIF = true; // received >37  and decoded  
       Serial.flush();
        } //eol IF ; 
    
     if ((OneRead > 31) && (OneRead<127)) {
     RespIF[Rptr] = OneRead;
     Rptr++;
     }
    yield();
      } //if serial avail; 
    } // while reading eol timeout 
   }//1st char in buff 
     repeats--;
   } //while repeats
   
   LastIf = millis();  
  } else validIF = true; // last result under 1,5sec old
  
}
//-------------------------------------------------------

