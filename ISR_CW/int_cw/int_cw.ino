#define LED 2


#define ditpad 4               //iambic keyer pad pins
#define dahpad 5
#define CWOUT 15                  //cw keying out
#define CWmax 50                  //WPM
#define CWmin 5
byte WinKey = 0;                  //very basic winkey emulation (speed + text) on/off (off kenwood KSwpm, KYtext in use)
byte WinState =0;                   //keyed text state 0=none, 1= started, 
byte CWdit = ditpad;                 //iambic keyer pad pins default
byte CWdah = dahpad;
byte Idit = 1;                  // keyer state pressed set by interrupts
byte Idah = 1;
unsigned int cwSpeed = 22;     
byte CWstate = 0;    //keyer state
unsigned long duration;
char morseLib[63] [6]= {
                        { 0,0,0,0,0,0 }, // <space>
                        { 1,1,3,3,1,0 }, // !
                        { 1,3,1,1,3,1 }, // " RR
                        { 0,0,0,0,0,0 }, // #
                        { 0,0,0,0,0,0 }, // $
                        { 0,0,0,0,0,0 }, // %
                        { 0,0,0,0,0,0 }, // &
                        { 0,0,0,0,0,0 }, // '
                        { 3,1,3,3,1,0 }, // ( KN
                        { 3,1,3,3,1,3 }, // ) KK
                        { 0,0,0,0,0,0 }, // *
                        { 1,3,1,3,1,0 }, // + AR
                        { 3,3,1,1,3,3 }, // ,
                        { 3,1,1,1,1,3 }, // - DU
                        { 1,3,1,3,1,3 }, // .
                        { 3,1,1,3,1,0 }, // /
                        { 3,3,3,3,3,0 }, //0
                        { 1,3,3,3,3,0 }, //1
                        { 1,1,3,3,3,0 }, //3
                        { 1,1,1,3,3,0 }, //3
                        { 1,1,1,1,3,0 }, //4
                        { 1,1,1,1,1,0 }, //5
                        { 3,1,1,1,1,0 }, //6
                        { 3,3,1,1,1,0 }, //7
                        { 3,3,3,1,1,0 }, //8
                        { 3,3,3,3,1,0 }, //9
                        { 3,1,3,3,1,0 }, // : KN
                        { 1,3,1,3,0,0 }, // ; AA
                        { 1,3,1,3,1,0 }, // < AR
                        { 3,1,1,1,3,0 }, // = BT
                        { 1,1,1,3,1,3 }, // > SK
                        { 1,1,3,3,1,1 }, // ?
                        { 1,3,3,1,3,1 }, // @ AC
                        { 1,3,0,0,0,0 }, //a
                        { 3,1,1,1,0,0 }, //b
                        { 3,1,3,1,0,0 }, //c
                        { 3,1,1,0,0,0 }, //d
                        { 1,0,0,0,0,0 }, //e
                        { 1,1,3,1,0,0 }, //f
                        { 3,3,1,0,0,0 }, //g
                        { 1,1,1,1,0,0 }, //h
                        { 1,1,0,0,0,0 }, //i
                        { 1,3,3,3,0,0 }, //j
                        { 3,1,3,0,0,0 }, //k
                        { 1,3,1,1,0,0 }, //l
                        { 3,3,0,0,0,0 }, //m
                        { 3,1,0,0,0,0 }, //n
                        { 3,3,3,0,0,0 }, //o
                        { 1,3,3,1,0,0 }, //p
                        { 3,3,1,3,0,0 }, //q
                        { 1,3,1,0,0,0 }, //r
                        { 1,1,1,0,0,0 }, //s
                        { 3,0,0,0,0,0 }, //t
                        { 1,1,3,0,0,0 }, //u
                        { 1,1,1,3,0,0 }, //v
                        { 1,3,3,0,0,0 }, //w
                        { 3,1,1,3,0,0 }, //x
                        { 3,1,3,3,0,0 }, //y
                        { 3,3,1,1,0,0 }, //z
                        { 1,3,3,1,3,0 }, // [ Å (a with round)
                        { 3,3,3,1,0,0 }, // \ Ö (o with dots)
                        { 1,3,1,3,0,0 }, // ] Ä (a with dots)
                   };
//-------------------------------------------------------

void setup(){
   delay(100);
  Serial.begin(115200);
  Serial.setTimeout(2000);
  while (!Serial) {};
  Serial.println("Serial ready");
  pinMode(LED, OUTPUT);
     //MorseTxEsp_setup
  pinMode(CWOUT,OUTPUT);
  pinMode(CWdit,INPUT);
  pinMode(CWdah,INPUT);
  Serial.println("I/O set ready");
  for (byte l=0;l<10;l++) {
  if (digitalRead(CWdah) == LOW ) {  //want reverse pads = keep wanted dit down during restart
    byte a = CWdit;
    CWdit = CWdah; //iambic keyer pad pins swithed
    CWdah = a; 
    l=30; //exit loop
   }
   delay(100);
  } 
  Serial.println("Paddle swap ready");
  //morse interrupts
  attachInterrupt(CWdit,dit_int,FALLING);
  attachInterrupt(CWdah,dah_int,FALLING);
  Serial.println("Paddle ints set"); 
  wpmTOduration(cwSpeed);
 
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(iambic);
  timer0_write(ESP.getCycleCount() + duration ); //this should initiate rolling
  interrupts();
  Serial.println("timer0 int set");
 
}


void loop(){
  Serial.print(Idit);
  Serial.print(" ");
  Serial.print(Idah);
  Serial.print(" ");
  Serial.println(CWstate);
  delay(500);
}
