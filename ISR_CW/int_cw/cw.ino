//-------------------------------------------------------
void iambic() {  //this will be the timer int routine!
  
  wpmTOduration(cwSpeed);
 
   if (CWstate ==0){   
      if (Idit == 0) {  // dit grounded
        CWstate = 2;  //1 +  element lenght       
      } else {
        delayMicroseconds(5);
        if (Idah == 0) {  // dah grounded
         CWstate = 4 ;
         }
        }      
       }

    
   switch(CWstate){    
    
    case 2: //dit
      CWstate = 1;
      timer0_write(ESP.getCycleCount() + duration ); // 80MHz(80000000L) == 1sec 
      digitalWrite(CWOUT, HIGH);        
    break;   
    
    case 4:
     CWstate = 3;
      timer0_write(ESP.getCycleCount() + duration * 3 ); // 80MHz(80000000L) == 1sec 
      digitalWrite(CWOUT, HIGH);       
    break; 


    case 1:
       elementspace();
       Idit=digitalRead(CWdit); //reset intrrupt results if no longer pressed
            //now, if dah is pressed go there, else check for dit
      if (Idah == 0) {  // dah grounded
        CWstate = 4;
      }
      else {
         if (Idit == 0) {  // dit grounded
         CWstate = 2;              
        }
       else {
         CWstate = 0;
        }
      }
    break;
       
    case 3:
       elementspace();
       Idah=digitalRead(CWdah); //reset intrrupt results if no longer pressed
       if (Idit == 0) {  // dit grounded
        CWstate = 2;
      }
      else {
         if (Idah == 0) {  // dah grounded
         CWstate = 4;              
        }
        else {
          CWstate = 0;
         }
      } 
    break;
    
    default:  //if nothing hits (means 0, 1 or 3, then) run next time after element delay continiously
      elementspace();
    break;
    }//switch   
 
}
//-------------------------------------------------------
void elementspace(){
      digitalWrite(CWOUT, LOW);
      timer0_write(ESP.getCycleCount() + duration); // 80MHz(80000000L) == 1sek
      CWstate = 0;
   }   
//-------------------------------------------------------
/* 
 *  1 dah = 3 dits
 *  element space inside letter is 1 dit
 *  space between two letters in same unsigned int is 3 dits
 *  space between two unsigned ints is 7 dits
 */
//-------------------------------------------------------
//space between two  letters is 3 dits, actually just 2 as char decode adds 1dit delay after element
void letterspace() { 
  CWstate = 2;
  timer0_write(ESP.getCycleCount() + 2L); // wake up timer
  //here set 
}
//-------------------------------------------------------
//space between unsigned ints 7 dits, actually 4 as letterdecode adds 3x dit to end (space between 2 letters in a unsigned int is 3 dits)
void wordspace() { 
 CWstate =4;
 timer0_write(ESP.getCycleCount() + 2L); // wake up timer
}

//-------------------------------------------------------
//explore one character elemet to ditahs here
//-------------------------------------------------------

//-------------------------------------------------------
//called from pad interrupt or expolore character
void ditah () {  
  if (CWstate == 0) return;
  
  if (CWstate > 127) {
    CWstate = CWstate & B1000000;
    timer0_write(ESP.getCycleCount() + duration * CWstate ); // 80MHz(80000000L) == 1sec
    digitalWrite(CWOUT, 1);
    CWstate = 1; //set space between CWstates
  } else {
    timer0_write(ESP.getCycleCount() + duration * CWstate); // 80MHz(80000000L) == 1sec
    CWstate = 0;
    digitalWrite(CWOUT, 0);
  }
}
//-------------------------------------------------------
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
                  Serial.write(charNo);
                  for(y=0;y<6;y++) {
                   if (morseLib[charNo-32][y] == 0) { // if element is 0 and start decode (y=0) make space
                      if (y == 0) {
                        wordspace();
                        }
                      y=6;
                      } else  { // make element
                      //ditah(morseLib[charNo-32][y]);
                      }
                   }    
                  letterspace();
                 }  
        if ((Idit == 0) || (Idah == 0)) { // pressing paddles halt cw string sending
          x = tl;
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
/*                                            P             A         R            I         S
 * wpm is element count of word "paris " 1+1+3+1+3+1 +3 +1+1+3 +3 +1+1+3+1+1 +3 +1+1+1 +3 1+1+1+1+1  +7 = 49 
 * one elemet duration is then  60000ms/wpm*50elements  
 * so:
 * 120wpm ->   60000ms/(120*49) =  10ms/element
 * 2wpm   ->   60000mc/(2*49)   = 612ms/element 
 * 
 * 80'000'000 cycles (80MHz) per second -> 80000 cycles per millisecond 
 * duration is now cycles for timer to count
 */

 duration = 80000*(60000/(wpm*49));   
}  
//-------------------------------------------------------
void dit_int(){
  Idit = 0;
}
//-------------------------------------------------------
void dah_int(){
  Idah = 0;
}
//-------------------------------------------------------

//-------------------------------------------------------
void setCWspeed(byte a) { 
  if ((a >= CWmin) && (a <= CWmax)) {
    cwSpeed = a;
//    if ( eerbyte(3) != cwSpeed) {
//      eewbyte(3,cwSpeed);
//      eesum();  
//    }  
  }
}
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
