#ifdef USE_UDP
//-------------------------------------------------------
void handleRoot() { 
    servo.setContentLength(CONTENT_LENGTH_UNKNOWN);
    servo.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    servo.sendContent("<!DOCTYPE HTML>\n<html><head><title>"+String(HOST)+"</title>\n");
    servo.sendContent("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n");
    servo.sendContent("<meta http-equiv='Expires' CONTENT='-1'>\n");
    servo.sendContent("<meta http-equiv='Last-Modified' CONTENT='0'>\n");
    servo.sendContent("<meta http-equiv='Cache-Control' CONTENT='no-cache, must-revalidate'>\n");
    servo.sendContent("<meta http-equiv='Pragma' CONTENT='no-cache'>\n");
    servo.sendContent("<meta name='viewport' content='initial-scale=1.0, maximum-scale=1.0, user-scalable=no'/>\n");
    //servo.sendContent("<meta http-equiv='Refresh' CONTENT='10;/'>\n"); //subpage /stat is repeatedly loaded
    servo.sendContent("<style>body {margin: 0px;padding: 0px;}table,th,td{border:1px solid black;border-collapse:collapse;text-align:center;}th{padding:5px;};\n");
    servo.sendContent("</style></head>\n");
    
    servo.sendContent("<body><center>&nbsp;<br><form action='/action'>");
    servo.sendContent("<table><tr><td bgcolor='f9ea02'><font size='5' color='#000000'><b>");
    servo.sendContent(HOST);
    servo.sendContent("</b></font></td></tr>");
    
    servo.sendContent("<tr><td bgcolor='#f7f7f7'><font size='3' color='#000000'>\n");
    servo.sendContent("CW speed "+String(cwSpeed)+" WPM<br>\n");
    servo.sendContent("UDP active at:"+local_IP.toString()+" port:"+String(udpPort)+"\n");
    servo.sendContent("</font></td></tr>");

     servo.sendContent("<tr><td bgcolor='#f7f7f7'><font size='3' color='#000000'>\n");
    servo.sendContent("<div style='text-align:left'><b>Commands:</b><br>\n");
    servo.sendContent("<ul><li><b>KY-1;</b> Send memory 1 contents</li>\n");
    servo.sendContent("<li><b>KY-2;</b> Send memory 2 contents</li>\n"); 
    servo.sendContent("<li><b>KY+1xyz;</b> Store 'xyz' to memory 1 </li>\n");
    servo.sendContent("<li><b>KY+2xyz;</b> Store 'xyz' to memory 2 </li>\n");
    servo.sendContent("<li><b>KY+1xyz;</b> Store 'xyz' to memory 1 </li>\n");
    servo.sendContent("<li><b>KYabcde;</b> Send CW 'abcde' </li></ul></div>\n");
    servo.sendContent("All other commands passed to TS50 Cat.<br>\n"); 
    servo.sendContent("Use ; as command terminator<br>\n");
    servo.sendContent("</font></td></tr>");

    servo.sendContent("<tr><td bgcolor='#f7f7f7'><font size='3' color='#000000'>\n");
    servo.sendContent("<p>Memory 1:<br> <input type='text' name='mem1' size='50' maxlength='100' value='"+String(msg1txt)+"'></p>\n");
    servo.sendContent("</font></td></tr>");

    servo.sendContent("<tr><td bgcolor='#f7f7f7'><font size='3' color='#000000'>\n");
    servo.sendContent("<p>Memory 2:<br> <input type='text' name='mem2' size='50' maxlength='100' value='"+String(msg2txt)+"'></p>\n");
    servo.sendContent("</font></td></tr>");


    servo.sendContent("<tr><td bgcolor='#f7f7f7'><font size='3' color='#000000'>\n");
    servo.sendContent("Paddle order:<br>\n");
    servo.sendContent("<input type='radio' name='reverse' value='1' ");
    if ( CWdit != ditpad ) {servo.sendContent(" checked ");};
    servo.sendContent("> Reverse \n"); 
    servo.sendContent("<input type='radio' name='reverse' value='0' ");
    if ( CWdit == ditpad ) {servo.sendContent(" checked ");};
    servo.sendContent("> Normal \n"); 
    servo.sendContent("</font></td></tr>");
 
    servo.sendContent("<tr><td bgcolor='#f7f7f7'><font size='4' color='#000000'>\n");
    servo.sendContent("<p><input type='submit' name='mSet' Value='Write settings'></p>\n");
    servo.sendContent("</font></td></tr>");
  
    servo.sendContent("<tr><td bgcolor='#f7f7f7'><font size='1' color='#770ab0'>\n");
    servo.sendContent("<div style='text-align:right'>v."+String(vers)+" OH1KH 2019</div>\n");
    servo.sendContent("</font></td></tr>");
    servo.sendContent("</table></form></center></body></html>\n");
    
    servo.sendContent("");
    servo.client().stop(); // Stop is needed because we sent no content length
}
//-------------------------------------------------------
void handleAction() {

// timeout
  if( servo.hasArg("mem1")) {
  servo.arg("mem1").toCharArray(msg1txt, 100);
  eewcharA(4, msg1txt, 100);
  eesum();
 }
 if( servo.hasArg("mem2")) {
 servo.arg("mem2").toCharArray(msg2txt, 100);
  eewcharA(105,msg2txt, 100);
  eesum();
 }
 if( servo.hasArg("reverse")) {
   if (servo.arg("reverse") == "1") {
      CWdit = dahpad;        //iambic keyer pad pins default
      CWdah = ditpad;
      } else {
      CWdit = ditpad;        //iambic keyer pad pins default
      CWdah = dahpad;
      }
   if((eerbyte(1) != CWdit ) || (eerbyte(2) != CWdah )){ //write if changed
     eewbyte(1,CWdit);
     eewbyte(2,CWdah);
     eesum();
     //morse interrupts
     attachInterrupt(digitalPinToInterrupt(CWdit),dit_int,FALLING);
     attachInterrupt(digitalPinToInterrupt(CWdah),dah_int,FALLING);
     }
  }
 handleRoot();
}
//-------------------------------------------------------
void handleNotFound(){
 handleRoot();
/*  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += servo.uri();
  message += "\nMethod: ";
  message += (servo.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += servo.args();
  message += "\n";
  for (uint8_t i=0; i<servo.args(); i++){
    message += " " + servo.argName(i) + ": " + servo.arg(i) + "\n";
  }
  servo.send(404, "text/plain", message);
*/
}
//-------------------------------------------------------
#endif
