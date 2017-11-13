
//-------------------------------------------------------
void dump_state() {
  //this is a fake for clients like wsjt-x and fldigi who wants to see what the rig is capable of
 
String state =  "\
0\n\
201\n\
1\n\
100000.000000 30000000.000000 0x1ff -1 -1 0x10000003 0x3\n\
0 0 0 0 0 0 0\n\
100000.000000 30000000.000000 0x8006f -1 -1 0x4000000 0x3\n\
0 0 0 0 0 0 0\n\
0x1ff 1\n\
0x1ff 0\n\
0 0\n\
0x1e 2400\n\
0x2 500\n\
0x1 8000\n\
0x1 2400\n\
0 0\n\
9990\n\
9990\n\
10000\n\
0\n\
0\n\
0\n\
0xffffffffffffffff\n\
0xffffffffffffffff\n\
0xfffffffff7ffffff\n\
0xffffffff83ffffff\n\
0xffffffffffffffff\n\
0xffffffffffffffbf\n";
writeCli(state);
}

//-------------------------------------------------------
