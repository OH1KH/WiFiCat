
void led_R() {
      digitalWrite(OnBoardLed, LedOff); 
      delay(1000);
      digitalWrite(OnBoardLed, LedOn);
      delay(200);
      digitalWrite(OnBoardLed, LedOff);
      delay(200);
      digitalWrite(OnBoardLed, LedOn);  //CW R
      delay(600);
      digitalWrite(OnBoardLed, LedOff);
      delay(200);
      digitalWrite(OnBoardLed, LedOn);
      delay(200);
      digitalWrite(OnBoardLed, LedOff);
      delay(500);
}

void led_N() {
      digitalWrite(OnBoardLed, LedOff); 
      delay(1000);
      digitalWrite(OnBoardLed, LedOn);  //CW N
      delay(600);
      digitalWrite(OnBoardLed, LedOff);
      delay(200);
      digitalWrite(OnBoardLed, LedOn);
      delay(200);
      digitalWrite(OnBoardLed, LedOff);
      delay(500);
}

void led_I() {
      digitalWrite(OnBoardLed, LedOff); 
      delay(1000);
      digitalWrite(OnBoardLed, LedOn);
      delay(200);
      digitalWrite(OnBoardLed, LedOff); // CW_I
      delay(200);
      digitalWrite(OnBoardLed, LedOn);
      delay(200);
      digitalWrite(OnBoardLed, LedOff);
      delay(1000);
}
void led_E() {
      digitalWrite(OnBoardLed, LedOff); 
      delay(1000);
      digitalWrite(OnBoardLed, LedOn);
      delay(200);
      digitalWrite(OnBoardLed, LedOff); // CW_E
      delay(1000);
}
