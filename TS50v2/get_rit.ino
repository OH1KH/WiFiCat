//-------------------------------------------------------
void get_rit() {
   // get rit
     theIF();
     if (validIF) {    
     if (longch == '\n')
       writeCli(rit+"\n");
     else {
      if (longch == '+') longch = '\n';
      writeCli("get_rit:"+String(longch)+"Rit: "+rit+String(longch)+"RPRT 0\n");
      }
     } else {  if (longch == '\n') writeCli("RPRT -1\n");
         else       
           writeCli("get_rit:"+String(longch)+"Rit: "+String(longch)+"RPRT -1\n");
       }
     }
//-------------------------------------------------------
//can't set_rit in this model
