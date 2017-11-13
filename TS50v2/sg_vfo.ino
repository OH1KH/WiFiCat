
//------------------------------------------------------- 
void get_vfo() {
     // get vfo
    theIF();
    if (validIF) {    
     if (longch == '\n')
       writeCli(vfo+"\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("get_vfo:"+String(longch)+"VFO: "+vfo+String(longch)+"RPRT 0\n");
      }
     } else {  if (longch == '\n') writeCli("RPRT -1\n");
         else       
         writeCli("get_vfo:"+String(longch)+"VFO: "+String(longch)+"RPRT -1\n");
       }
}
    
//-------------------------------------------------------    
void set_vfo() {
    // set  vfo 
    ok = false;
    String rep ="";
    if (RigCmd.indexOf("currVFO",0) > -1 ) {
      ok = true;
      rep ="currVFO";
     }
     if (RigCmd.indexOf("VFO",0) > -1 ) {
      ok = true;
      rep ="VFO";
     }
    if (RigCmd.indexOf("VFOA",0) > -1 ) {
      writeHost("FN0;");
      ok = true;
      rep ="VFOA";
     }
    if (RigCmd.indexOf("VFOB",0) > -1 ) {
      writeHost("FN1;");
      ok = true;
      rep ="VFOB";
     }
    if (RigCmd.indexOf("MEM",0) > -1 ) {
      writeHost("FN2;");
      ok = true;
      rep ="MEM";
    }
    if (ok) { 
      if (longch == '\n')
       writeCli("RPRT 0\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("set_vfo: "+rep+String(longch)+"RPRT 0\n");
      }
     } else { 
      if (longch == '\n') writeCli("RPRT -1\n");
         else       
          writeCli("set_vfo: "+rep+String(longch)+"RPRT -1\n");
       } 
   }
//-------------------------------------------------------

