//   case 's': get_split_vfo();  break; 
//   case 'S': set_split_vfo();  break;  
//   Get 'Split' mode, '0' or '1', and 'TX VFO'

//-------------------------------------------------------
void get_split_vfo() {
    // get split & TX vfo
    //first get RX vfo
    byte oke = 0; //false
    theIF();
    // Freq,rit,rit_on,xit_on,Mem_ch,TX_on,mode_is,vfo,split 
    if (validIF) { 
      
       if (split == "0") { 
        oke = 1;  // ok no split no vfo switch
        } else { //split on
         if ( vfo != "MEM" ) {
           if ( vfo == "VFOA" ) writeHost("FN1;"); else writeHost("FN0;");  
          LastIf = 0; //must recall here immediately 
          theIF();
          // Freq,rit,rit_on,xit_on,Mem_ch,TX_on,mode_is,vfo,split 
          if (validIF) oke = 4; //ok and vfo switched
            
        } else oke = 2; //not ok, no vfo switched
       } //split on
       
     } //valid if1 

     
    if ((oke == 1) || (oke == 4)) { 
       if (longch == '\n')
       writeCli(split+"\n"+vfo+"\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("get_split_vfo:"+String(longch)+"Split: "+split+String(longch)+"TX VFO: "+vfo+String(longch)+"RPRT 0\n");
      }
     } 
 
     if ((oke == 0) || (oke == 2)) { 
        if (longch == '\n') writeCli("RPRT -1\n");
         else  {
          if (longch == '+') longch = '\n';     
          writeCli("get_split_vfo:"+String(longch)+"Split: "+String(longch)+"TX VFO: "+String(longch)+"RPRT -1\n");
       }
     }

     
    if  (oke == 4) { //return vfo
     if (vfo =="VFOA") writeHost("FN1;"); else writeHost("FN0;");
   }
}
//-------------------------------------------------------
void set_split_vfo(){
    // set split & TX vfo 
    boolean o = true;
    String SetS="";
    String Ans = "RPRT -1\n";
 
    if (RigCmd.charAt(0) == '1') {
      SetS="SP1;";
     } else {
       if (RigCmd.charAt(0) == '0') {
         SetS="SP0;";
       } else { o = false;}
     }
     
     if ( o ) {    
      if (RigCmd.indexOf("VFOA",1) > -1 ) { 
        if (SetS == "SP0;") writeHost("FN0;"); else writeHost("FN1;"); //set RX vfo as B if split
        writeHost(SetS); //set split
        Ans = "RPRT 0\n";
        }
       if (RigCmd.indexOf("VFOB",1) > -1 ) {
         if (SetS == "SP0;") writeHost("FN1;"); else writeHost("FN0;"); //set RX vfo as A if split
         writeHost(SetS); //set set split
         Ans = "RPRT 0\n";
        }
     } 
     if (longch == '\n')
       writeCli(Ans);
     else {
      if (longch == '+') longch = '\n';
      writeCli("set_split_vfo: "+RigCmd+String(longch)+Ans);
      }
}         

//------------------------------------------------------- 
