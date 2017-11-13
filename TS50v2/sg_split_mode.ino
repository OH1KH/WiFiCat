
//  case 'x': get_split_mode(); break;    
//   case 'X': set_split_mode(); break;   
//  Get 'TX Mode' and 'TX Passband'. 
//------------------------------------------------------- 
void get_split_mode() {
    // get split TX mode bandwidth
    boolean o = true;
    int myvfo = 0;
    theIF();
    // Freq,rit,rit_on,xit_on,Mem_ch,TX_on,mode_is,vfo,split 
    if (validIF) {     //get oppisite vfo on (is TX)
      if (vfo == "VFOA") {
        writeHost("FN1;"); //set RX vfo as B
        myvfo = 1;
      }
      if (vfo == "VFOB") {
        writeHost("FN0;"); //set RX vfo as A
        myvfo = 2;
      }
      if (vfo == "MEM")  o = false;     
      } else { o = false; }

    LastIf = 0;     
    theIF(); //get oppsite vfo info FIXset timimg !!!!!
    if (validIF && (o == true)) {    
     if (longch == '\n')
       writeCli(mode_is+"\n"+passband+"\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("get_split_mode:"+String(longch)+"TX Mode: "+mode_is+String(longch)+"TX Passband: "+passband+String(longch)+"RPRT 0\n");
      }
     } else {  
       if (longch == '\n') writeCli("RPRT -1\n");
         else {
          if (longch == '+') longch = '\n';
          writeCli("get_split_mode:"+String(longch)+"TX Mode: "+String(longch)+"TX Passband: "+String(longch)+"RPRT -1\n");      
         }  
       }  
     if (o ) {      
      if (myvfo == 2) {
        writeHost("FN1;"); //return RX vfo as A
      }
      if (myvfo == 1) {
        writeHost("FN0;"); //return RX vfo as B
      }           
     }
}

//-------------------------------------------------------    
void set_split_mode() {
   // get split TX mode bandwidth
    boolean o = true;
    int myvfo = 0;
    
   if (RigCmd.charAt(0) == '?') {
    String Ans2 = "AM CW USB LSB FM"; 
    String Ans = "RPRT 0\n";
    if (longch == '\n') {
       writeCli(Ans2+"\n");
       writeCli(Ans);
     }
     else {
      if (longch == '+') longch = '\n';
      writeCli("set_split_mode: ?"+String(longch)+Ans2+String(longch)+Ans);
      }
    return;}
    
    theIF();
    // Freq,rit,rit_on,xit_on,Mem_ch,TX_on,mode_is,vfo,split 
    if (validIF) {     //get oppisite vfo on (is TX)
      if (vfo == "VFOA") {
        writeHost("FN1;"); //set RX vfo as B
        myvfo = 1;
      }
      if (vfo == "VFOB") {
        writeHost("FN0;"); //set RX vfo as A
        myvfo = 2;
      }
      if (vfo == "MEM")  o = false;     
      } else { o = false; }
        
     if (o && ModeSet()) {  //mode set by set_mode()
         
     if (longch == '\n'){
        writeCli("RPRT 0\n"); 
     }
     else {
      if (longch == '+') longch = '\n';
      writeCli("set_split_mode: "+RigCmd+(longch)+"RPRT 0\n");
      }
    
     } else {  if (longch == '\n') writeCli("RPRT -1\n");
         else       
         writeCli("set_split_mode: "+RigCmd+String(longch)+"RPRT -1\n");
       }  
       
    if (o ) {   
      if (myvfo == 2) {
        writeHost("FN1;"); //return RX vfo as A
      }
      if (myvfo == 1) {
        writeHost("FN0;"); //return RX vfo as B
      }      
    }     
}
  //------------------------------------------------------- 
