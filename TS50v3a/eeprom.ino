

//-------------------------------------------------------
boolean eecheck() {
  byte check = 255;
  //all bytes same as checksum gives false checksum
  boolean allsame = true;
  for (int address = 1;address <= EETOP; address++){
    if (EEPROM.read(address) != check) {
      allsame =false;
      break;
    }
  }
  if (allsame) {
    #ifdef DEBUG
     Serial.print(F("All bytes same as check "));Serial.print(check);Serial.println(F(" position 0"));
    #endif 
  return false;
  }
   
  check = 0;
  boolean empty = true;

// byte 0 xored checksum of whole area
 for (int address = 1;address <= EETOP; address++){ 
    check = EEPROM.read(address) ^ check;
    if (empty) {
      empty = (EEPROM.read(address) == '\0');
     }
    }

  #ifdef DEBUG
   Serial.print(F("Checksum/count "));Serial.print(EEPROM.read(0));Serial.print(F("/"));Serial.print(check);
  #endif 
    
  if (EEPROM.read(0) != check) { 
    return false;
    } else { 
      
     if (!empty){ 
       return true;
      } else { 
        return false;
        }
     }
}
//-------------------------------------------------------
void eesum() {
 //write checksum to 0
  byte check = 0;
  for (int address = 1;address <= EETOP; address++){ check = EEPROM.read(address) ^ check;};
  EEPROM.write(0,check); 
  #ifdef DEBUG
   Serial.print(F("writing eeprom check byte "));Serial.print(check);Serial.println(F(" position 0"));
  #endif 
  EEPROM.commit(); //note here the commit!
}
//-------------------------------------------------------lowByte(dat)
boolean eestart() {
  boolean ret = true;
  #ifdef DEBUG
   Serial.println(F("Reserve EEPROM"));
  #endif 
  EEPROM.begin(EETOP); 
  #ifdef DEBUG
   Serial.println(F("check EEPROM "));
  #endif 
  if (!eecheck()) {
      #ifdef DEBUG
       Serial.println(F("Is NOT ok. Writing 0 to EEPROM"));
      #endif 
      for (int address = 0;address <= EETOP; address++){ EEPROM.write(address,0); };
      EEPROM.commit(); //note here the commit!
      ret = false;
     } else {
     #ifdef DEBUG
      Serial.println(F(" Reading EEPROM is ok."));
     #endif 
   } 
 return ret;
}

//-------------------------------------------------------
//-----------------WRITES--------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
// write byte to pos NOTE must checksummed separeately
void eewbyte(int pos, byte dat) {
 #ifdef DEBUG
  Serial.print(F("writing eeprom byte "));Serial.print(dat);Serial.print(F(" position "));Serial.println(pos);
 #endif 
 EEPROM.write(pos, dat);  
 EEPROM.commit();
}
//-------------------------------------------------------
// write int (2xbyte) to pos NOTE must checksummed separeately
void eewint(int pos, int dat) {
 #ifdef DEBUG 
  Serial.print(F("writing eeprom int "));Serial.print(dat);Serial.print(F(" position "));Serial.println(pos); 
 #endif 
 EEPROM.write(pos, highByte(dat)); 
 #ifdef DEBUG
  Serial.print(F("writing eeprom int HI "));Serial.print(highByte(dat));Serial.print(F(" position "));Serial.println(pos);
 #endif 
 EEPROM.write(pos +1, lowByte(dat)); 
 #ifdef DEBUG
  Serial.print(F("writing eeprom int LO "));Serial.print(lowByte(dat));Serial.print(F(" position "));Serial.println(pos+1);
 #endif 
 EEPROM.commit();
}
//-------------------------------------------------------
// write char(array size given) to pos NOTE must checksummed separeately
void eewcharA(int pos, char dat[], int sz) {
  int i = 0;
  do {
    #ifdef DEBUG
    Serial.print(F("writing eeprom "));Serial.print(byte(dat[i]));Serial.print(F(" position "));Serial.println(pos);
    #endif 
    EEPROM.write(pos, byte(dat[i]));
     i++;
     pos++;
    } while  ((i < sz) && (dat[i]!='\0'));
  EEPROM.write(pos,'\0');  
    #ifdef DEBUG
    Serial.print(F("writing eeprom "));Serial.print(byte(dat[i]));Serial.print(F(" position "));Serial.println(pos);
    #endif 
  EEPROM.commit();
}
//-------------------------------------------------------
//-----------------READS---------------------------------
//-------------------------------------------------------
// read int (2xbyte) from pos
int eerint(int pos) {
 byte eerh = EEPROM.read(pos); // HI byte
 #ifdef DEBUG
  Serial.print(F("reading eeprom int HI "));Serial.print(eerh);Serial.print(F(" from position "));Serial.println(pos);
 #endif 
 byte eerl = EEPROM.read(pos+1);
 #ifdef DEBUG
 Serial.print(F("reading eeprom int LO "));Serial.print(eerl);Serial.print(F(" from position "));Serial.println(pos+1);
 #endif 
 int eer = word(eerh,eerl);
 if((eer>>15) == 1) eer = eer - 65536;
 #ifdef DEBUG
  Serial.print(F("reading eeprom int RESULT "));Serial.print(eer);Serial.print(F(" from position "));Serial.println(pos); 
 #endif 
return eer;
}
//-------------------------------------------------------
// read byte from pos 
byte eerbyte(int pos) {
 byte eer = EEPROM.read(pos);
 #ifdef DEBUG
  Serial.print(F("reading eeprom byte "));Serial.print(eer);Serial.print(F(" from position "));Serial.println(pos);
 #endif 
 return eer;
}
//-------------------------------------------------------
// read to global char(array size given) from pos 
void eercharA(int pos, char dat[], int sz) {
  int i = 0;
  do  {
     dat[i] = eerbyte(pos);
     i++;
     pos++;
     
  }while  ((i < sz) && (dat[i-1]!='\0'));

}
  
