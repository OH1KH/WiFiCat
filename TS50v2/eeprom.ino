


//-------------------------------------------------------
boolean eecheck() {
  byte check = 0;
// byte 0 xored checksum of whole area
 for (int address = 1;address <= EETOP; ++address){ check = EEPROM.read(address) ^ check;};
  if (EEPROM.read(0) != check) { return false; } else { return true; };
}
//-------------------------------------------------------
void eesum() {
 //write checksum to 0
  byte check = 0;
  for (int address = 1;address <= EETOP; ++address){ check = EEPROM.read(address) ^ check;};
  EEPROM.write(0,check); 
  //Serial.println("writing eeprom check byte "+String(check)+" position 0");
  EEPROM.commit(); //note here the commit!
}
//-------------------------------------------------------
boolean eestart() {
  boolean ret = true;
  //Serial.println("Reserve EEPROM");
  EEPROM.begin(EETOP); 
  //Serial.println("check EEPROM ");
  if (!eecheck()) {
      //Serial.println("Is NOT ok. Writing 0 to EEPROM");
      for (int address = 0;address <= EETOP; ++address){ EEPROM.write(address,0); };
      EEPROM.commit(); //note here the commit!
      ret = false;
     } else {
     //Serial.println(" Reading EEPROM is ok.");
   } 
}

//-------------------------------------------------------
//-----------------WRITES--------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
// write byte to pos NOTE must checksummed separeately
void eewbyte(int pos, byte dat) {
 //Serial.println("writing eeprom byte "+String(dat)+" position "+String(pos));
 EEPROM.write(pos, dat);  
 EEPROM.commit();
}
//-------------------------------------------------------
// write int (2xbyte) to pos NOTE must checksummed separeately
void eewint(int pos, int dat) {
 //Serial.println("writing eeprom int "+String(dat)+" position "+String(pos)); 
 EEPROM.write(pos, highByte(dat)); 
 //Serial.println("writing eeprom int HI "+String(highByte(dat))+" position "+String(pos));
 EEPROM.write(pos +1, lowByte(dat)); 
 //Serial.println("writing eeprom int LO "+String(lowByte(dat))+" position "+String(pos+1));
 EEPROM.commit();
}

//-------------------------------------------------------
//-----------------READS---------------------------------
//-------------------------------------------------------
// read int (2xbyte) from pos
int eerint(int pos) {
 byte eerh = EEPROM.read(pos); // HI byte
 //Serial.println("reading eeprom int HI "+String(eerh)+" from position "+String(pos)); 
 byte eerl = EEPROM.read(pos+1);
 //Serial.println("reading eeprom int LO "+String(eerl)+" from position "+String(pos+1));
 int eer = word(eerh,eerl);
 if((eer>>15) == 1) eer = eer - 65536;
 //Serial.println("reading eeprom int RESULT "+String(eer)+" from position "+String(pos)); 
return eer;
}
//-------------------------------------------------------
// read byte from pos 
byte eerbyte(int pos) {
 byte eer = EEPROM.read(pos);
 //Serial.println("reading eeprom byte "+String(eer)+" from position "+String(pos));
 return eer;
}
//-------------------------------------------------------

