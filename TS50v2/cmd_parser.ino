
 //-------------------------------------------------------
void ParseCommand(){
char starts;

for (int l=0;l<(RigCmd.length());l++) {
  starts = RigCmd.charAt(l);
  if ( (starts !='+') && (starts !=',') && (starts !='|') && (starts !=';') && (starts != 92) )  {
   //accept only selected short commands queued 
   switch (starts) {
   
   case 'l': get_level();  break;    
   case 't': get_ptt();    break;  
   case 'f': get_freq();   break; 
   case 'm': get_mode();   break;    
   case 'v': get_vfo();    break; 
   case 'j': get_rit();    break; 
   case 'i': get_split_freq(); break; 
   case 'x': get_split_mode(); break;    
   case 's': get_split_vfo();  break; 
   case 'q': serverClients[CliNr].stop(); break;  
   case 'Q': serverClients[CliNr].stop();  break;  
    
   default : ParsePart(); break; 
   }
  } else ParsePart(); // if/not short cmd
 } //for loop
 
}

 //-------------------------------------------------------    
void ParsePart() { 
  char First = RigCmd.charAt(0);
  
  if ((First =='+') || (First ==',') || (First =='|') || (First ==';')) {
    longch = First;
    RigCmd.setCharAt(0,32); //set it to space:
    RigCmd.trim(); // trim away
    First = RigCmd.charAt(0); // new first
  } else longch = '\n';
  
// we know now first, leave only rest of command
    RigCmd.setCharAt(0,32); //set it to space:
    RigCmd.trim(); // trim away

 //supported rigctld long/short commands
 switch (First) {
  
   case 'b': send_morse(); break; 
   case 'L': set_level();  break; 
   case 'l': get_level();  break;    
   case 't': get_ptt();    break; 
   case 'T': set_ptt();    break;  
   case 'f': get_freq();   break; 
   case 'F': set_freq();   break; 
   case 'm': get_mode();   break;    
   case 'M': set_mode();   break;    
   case 'v': get_vfo();    break; 
   case 'V': set_vfo();    break;
   case 'j': get_rit();    break; 
   case 'i': get_split_freq(); break; 
   case 'I': set_split_freq(); break;    
   case 'x': get_split_mode(); break;    
   case 'X': set_split_mode(); break;   
   case 's': get_split_vfo();  break; 
   case 'S': set_split_vfo();  break;  
   case 'q': serverClients[CliNr].stop(); break;  
   case 'Q': serverClients[CliNr].stop();  break;  
   
    //backslash = longmode
   case 92 : 
    { 
     if (RigCmd.indexOf("dump_caps",0) > -1 ) dump_caps(); //tst ok
     if (RigCmd.indexOf("dump_state",0) > -1 ) dump_state(); //tst ok
 
 //--------------------cut long command away leave last letter---------------------------------------
     if (RigCmd.indexOf("get_freq",0) > -1 )  get_freq(); //tst ok
     if (RigCmd.indexOf("get_mode",0) > -1 ) get_mode(); //tst ok
     if (RigCmd.indexOf("get_vfo",0) > -1 ) get_vfo();  //tst ok
     if (RigCmd.indexOf("get_rit",0) > -1 ) get_rit();  // tst ok
     if (RigCmd.indexOf("get_ptt",0) > -1 ) get_ptt();  // tst ok
     if (RigCmd.indexOf("get_split_freq",0) > -1 ) get_split_freq();
     if (RigCmd.indexOf("get_split_mode",0) > -1 ) get_split_mode();
     if (RigCmd.indexOf("get_split_vfo",0) > -1 ) get_split_vfo();
     if (RigCmd.indexOf("get_level",0) > -1 ) get_level();            //tst ok
     
     if (RigCmd.indexOf("set_freq",0) > -1 ) { for (int i=0;i<8;i++) { RigCmd.setCharAt(0,32);RigCmd.trim();} set_freq();} //tst ok
     if (RigCmd.indexOf("set_mode",0) > -1 ) { for (int i=0;i<8;i++) { RigCmd.setCharAt(0,32);RigCmd.trim();} set_mode();} //tst ok
     if (RigCmd.indexOf("set_ptt",0) > -1 ) { for (int i=0;i<7;i++) { RigCmd.setCharAt(0,32);RigCmd.trim();} set_ptt();}  // tst ok
     if (RigCmd.indexOf("set_vfo",0) > -1 ) { for (int i=0;i<7;i++) { RigCmd.setCharAt(0,32);RigCmd.trim();} set_vfo();}  // tst ok
     if (RigCmd.indexOf("set_split_freq",0) > -1 ) { for (int i=0;i<14;i++) { RigCmd.setCharAt(0,32);RigCmd.trim();} set_split_freq();} //tst ok  
     if (RigCmd.indexOf("set_split_mode",0) > -1 )  { for (int i=0;i<14;i++) { RigCmd.setCharAt(0,32);RigCmd.trim();} set_split_mode();} //tst ok
     if (RigCmd.indexOf("set_split_vfo",0) > -1 )  { for (int i=0;i<13;i++) { RigCmd.setCharAt(0,32);RigCmd.trim();} set_split_vfo();} //tst ok
     if (RigCmd.indexOf("set_level",0) > -1 ) { for (int i=0;i<9;i++){ RigCmd.setCharAt(0,32);RigCmd.trim();} set_level();} //only KEYSPD  tst ok
     
     if (RigCmd.indexOf("send_morse",0) > -1 ) { for (int i=0;i<10;i++) { RigCmd.setCharAt(0,32);RigCmd.trim();} send_morse();}  //tst ok
    } 
    break;  //longmode command
  
   default : writeCli("RPRT -11\n"); break; 
  }//switch First
  yield();
  RigCmd = "";
 }//parse part
//------------------------------------------------------- 
