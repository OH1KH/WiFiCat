//-------------------------------------------------------
void iambic() {

  do {
  yield();
  // read poti, scale to15-35 wpm to cwSpeed 
  if ( millis() > Nextmillis) {
       cwSpeed = map(analogRead(0), 0, 1023, 8,40);
       delay(1);
       wpmTOduration(cwSpeed);
       Nextmillis = millis()+50;
     }
  
  switch(CWstate){
    case 0:   
      if (Idit) {  // dit grounded
        CWstate = 1;              
      }
      else {
        if (Idah) {  // dah grounded
        CWstate = 3;
        }
      }   
    yield();     
    break;
    case 1: //dit
      ditah(CWstate);
      //now, if dah is pressed go there, else check for dit
      if (Idah ) {  // dah grounded
        CWstate = 3;
      }
      else {
         if (Idit) {  // dit grounded
         CWstate = 1;              
        }
        else {
          CWstate = 0;
        }
      }
    yield();          
    break;   
    case 3:  //dah
      ditah(CWstate);
      //now, if dit is pressed go there, else check for dah
      if (Idit) {  // dit grounded
        CWstate = 1;
      }
      else {
         if (Idah) {  // dah grounded
         CWstate = 3;              
        }
        else {
          CWstate = 0;
        }
      }
    yield();          
    break;
  }//switch   
  
  
    if (!digitalRead(mem1_pin)) {
    //Serial.print(" 1 ");
    //Serial.println(msg1txt);
    
    MorseTxEsp_CSend(cwSpeed,msg1txt);
    while (!digitalRead(mem1_pin)){
      delay(5);
    }
  }
  if (!digitalRead(mem2_pin)) {
    //Serial.print(" 2 ");
    //Serial.println(msg2txt);
    MorseTxEsp_CSend(cwSpeed,msg2txt);
      while (!digitalRead(mem2_pin)){
      delay(5);
    }
  }
  } while (CWstate ); 
}
//-------------------------------------------------------
//-------------------------------------------------------
void tone_on() {
 //analogWrite(CWOUT, CWFREQ);
 digitalWrite(CWOUT, 1);
}
//-------------------------------------------------------

void tone_off() {
 //analogWrite(CWOUT, 0);
 digitalWrite(CWOUT, 0);
}

/* 
1 dah = 3 dits
element space inside letter is 1 dit
space between two letters in same unsigned int is 3 dits
space between two unsigned ints is 7 dits
*/
//-------------------------------------------------------
//space between two  letters is 3 dits, actually just 2 as char decode adds 1dit delay after element
void letterspace() { 
  delay(duration * 2);
}
//-------------------------------------------------------
//space between unsigned ints 7 dits, actually 4 as letterdecode adds 3x dit to end (space between 2 letters in a unsigned int is 3 dits)
void wordspace() { 
  delay(duration * 4);
}
//-------------------------------------------------------
void ditah(int e){
   tone_on();
   delay(duration * e);
   tone_off();
   delay(duration);
   if ( e == 1) { 
     Idit=!digitalRead(CWdit); //reset intrrupt results if no longer pressed
    }
   if ( e == 3) { 
     Idah=!digitalRead(CWdah); //reset intrrupt results if no longer pressed
    }
}
//-------------------------------------------------------
//send char*
void MorseTxEsp_CSend(unsigned int wpm,char *t) {
        wpmTOduration(wpm);
        int x;
        byte y;
        int tl = strlen(t);
        for(x=0;x<tl;x++) {
                char charNo = t[x];
                
                if ( charNo > 96 ) {   //convert to upper case 
                  charNo = charNo - 32;
                  }
                if ( (charNo > 31) and (charNo < 94) ) { //if in table range do decode
                  for(y=0;y<6;y++) {
                   if (morseLib[charNo-32][y] == 0) { // if element is 0 and start decode (y=0) make space
                      if (y == 0) {
                        wordspace();
                        }
                      y=6;
                      } else  { // make element
                      ditah(morseLib[charNo-32][y]);
                      }
                   }    
                  letterspace();
                 }  
        if ((Idit) || (Idah)) { // pressing paddles halt cw string sending
          x = tl;
          Idit=1;
          Idah=1;
         }                 
        }
//Serial.println();
}
//-------------------------------------------------------
// send string
void MorseTxEsp_SSend(unsigned int wpm,String ceewee) { 
char cee[ceewee.length()+1];
ceewee.toCharArray(cee,ceewee.length()+1);
//Serial.println("Converted string to char array, len "+String(ceewee.length()+1));
MorseTxEsp_CSend(wpm,cee);
}
//-------------------------------------------------------
void wpmTOduration(unsigned int wpm) {
/*                                P             A         R            I         S
 wpm is element count "paris" 1+1+3+1+3+1 +3 +1+1+3 +3 +1+1+3+1+1 +3 +1+1+1 +3 1+1+1+1+1  +7 = 49 
 one elemet duration is then  60000ms/wpm*50elements  
 so:
 120wpm ->   60000ms/(120*49) =  10ms/element
 2wpm   ->   60000mc/(2*49)   = 612ms/element 
*/
 duration = 60000/(wpm*49);   
}  
//-------------------------------------------------------

void ICACHE_RAM_ATTR dit_int(){
  Idit = true;
}
//-------------------------------------------------------

void ICACHE_RAM_ATTR dah_int(){
  Idah = true;
}
//-------------------------------------------------------
void setCWspeed(byte a) { 
  if ((a >= CWmin) && (a <= CWmax)) {
    cwSpeed = a;
    if ( eerbyte(3) != cwSpeed) {
      eewbyte(3,cwSpeed);
      eesum();  
    }  
  }
}
//-------------------------------------------------------
