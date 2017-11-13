
//-------------------------------------------------------
void get_mode() {
    // get mode 
    theIF();
    if (validIF) {    
     if (longch == '\n')
       writeCli(mode_is+"\n"+passband+"\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("get_mode:"+String(longch)+"Mode: "+mode_is+String(longch)+"Passband: "+passband+String(longch)+"RPRT 0\n");
      }
     } else {  if (longch == '\n') writeCli("RPRT -1\n");
         else       
         writeCli("get_mode:"+String(longch)+"Mode: "+String(longch)+"Passband: "+String(longch)+"RPRT -1\n");
       }
}    

//-------------------------------------------------------     
 void set_mode() { //we do not use passband as no filters in rig (CWn)+M
   // set mode
    if (ModeSet()) { 
      if (longch == '\n')
       writeCli("RPRT 0\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("set_mode: "+mode_is+" "+passband+String(longch)+"RPRT 0\n");
      }
     } else {  if (longch == '\n') writeCli("RPRT -1\n");
         else       
         writeCli("set_mode: "+String(longch)+String(longch)+"RPRT -1\n");
       } 
    } 
//-------------------------------------------------------
boolean ModeSet() {
    ok = false;
    if (RigCmd.indexOf("LSB",0) > -1 ) {
      writeHost("MD1;");
      mode_is = "LSB";
      ok = true;
      }
    if (RigCmd.indexOf("USB",0) > -1 ) {
      writeHost("MD2;");
      mode_is = "USB";
      ok = true;
     }
    if (RigCmd.indexOf("CW",0) > -1 ) {
      writeHost("MD3;");
      mode_is = "CW";
      ok = true;
     }
    if (RigCmd.indexOf("FM",0) > -1 ) {
      writeHost("MD4;");
      mode_is = "FM";
      ok = true;
     }
   if (RigCmd.indexOf("AM",0) > -1 ) {
      writeHost("MD5;");
      mode_is = "AM";
      ok = true;
     }
  return ok; 
}
