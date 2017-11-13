

void loop() { 
  MDNS.update();
  serveTCP(); //check connects and disconnects
  
  readHostWriteCli();  // rig communication port
  readCliWriteHost();
  
  if (readCli()) {  //cw communication port
    ParseCommand(); 
    RigCmd="";
  }
  iambic(); // read key pads
}

