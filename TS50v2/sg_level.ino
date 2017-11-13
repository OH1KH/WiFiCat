
//-------------------------------------------------------    
void set_level() {
    String rep ="RPRT -1\n";
    int Cpos = RigCmd.indexOf("KEYSPD",0);
    if (Cpos >= 0) {
      for (int i=0;i < ( Cpos + 6 );i++) RigCmd.setCharAt(i,32); //set [i] to space:
       RigCmd.trim();
        int Csp = RigCmd.toInt();
        if (Csp != 0) { //conv ok
         if (Csp < 5) Csp = 5;
         if (Csp > 50) Csp = 50;
         cwSpeed = Csp;
         rep = "RPRT 0\n";
        } 
      }
    if (longch == '\n')
     writeCli(rep);
     else {
      if (longch == '+') longch = '\n';
      writeCli("set_level:"+String(longch)+"KEYSPD"+String(longch)+RigCmd+String(longch)+rep);
     }  
    }
//-------------------------------------------------------    
void get_level(){
      if (longch == '\n')
     writeCli(String(cwSpeed)+"\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("get_level:"+String(longch)+"KEYSPD"+String(longch)+String(cwSpeed)+String(longch)+"RPRT 0\n");
     }  
}    
//-------------------------------------------------------  

