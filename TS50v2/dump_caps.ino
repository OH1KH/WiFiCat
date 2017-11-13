
//-------------------------------------------------------
void dump_caps() {
  //this is a fake for clients like wsjt-x and fldigi who wants to see what the rig is capable of

String caps = "\
Caps dump for model: 201\n\
Model name:	TS-50S\n\
Mfg name:	Kenwood\n\
Backend version:	0.9\n\
Backend copyright:	LGPL\n\
Backend status:	Untested\n\
Rig type:	Mobile\n\
PTT type:	Rig capable\n\
DCD type:	Rig capable\n\
Port type:	RS-232\n\
Serial speed: 4800..4800 bauds, 8N2 \n\
Write delay: 20ms, timeout 2500ms, 10 retry\n\
Post Write delay: 30ms\n\
Has targetable VFO: Y\n\
Has transceive: Y\n\
Announce: 0x0\n\
Max RIT: -1.100kHz/+1.100kHz\n\
Max XIT: -0.0kHz/+0.0kHz\n\
Max IF-SHIFT: -0.0kHz/+0.0kHz\n\
Preamp: None\n\
Attenuator: 18dB\n\
CTCSS: None\n\
DCS: None\n\
Get functions: FAGC NB COMP VOX TONE TSQL NR LOCK BC \n\
Set functions: FAGC NB COMP VOX TONE TSQL NR LOCK BC \n\
Get level: ATT(0..0/0) AF(0..0/0) RF(0..0/0) SQL(0..0/0) RFPOWER(0..0/0) MICGAIN(0..0/0) AGC(0..0/0) STRENGTH(0..0/0) \n\
Set level: ATT(0..0/0) AF(0..0/0) RF(0..0/0) SQL(0..0/0) RFPOWER(0..0/0) MICGAIN(0..0/0) AGC(0..0/0) \n\
Extra levels:\n\
Get parameters: \n\
Set parameters: \n\
Extra parameters:\n\
Mode list: AM CW USB LSB FM \n\
VFO list: VFOA VFOB \n\
VFO Ops: UP DOWN \n\
Scan Ops: \n\
Number of banks:	0\n\
Memory name desc size:	0\n\
Memories:\n\
	0..89:   	MEM\n\
	  Mem caps: \n\
	90..99:   	EDGE\n\
	  Mem caps: \n\
TX ranges status, region 1:	OK (0)\n\
RX ranges status, region 1:	OK (0)\n\
TX ranges status, region 2:	OK (0)\n\
RX ranges status, region 2:	OK (0)\n\
Tuning steps:\n\
	50 Hz:   	AM CW USB LSB FM \n\
	100 Hz:   	AM CW USB LSB FM \n\
	1 kHz:   	AM CW USB LSB FM \n\
	5 kHz:   	AM CW USB LSB FM \n\
	9 kHz:   	AM CW USB LSB FM \n\
	10 kHz:   	AM CW USB LSB FM \n\
	12.5 kHz:   	AM CW USB LSB FM \n\
	20 kHz:   	AM CW USB LSB FM \n\
	25 kHz:   	AM CW USB LSB FM \n\
	100 kHz:   	AM CW USB LSB FM \n\
	1 MHz:   	AM CW USB LSB FM \n\
	ANY:   	AM CW USB LSB FM \n\
Tuning steps status:	OK (0)\n\
Filters:\n\
	2.2 kHz:   	CW USB LSB \n\
	5 kHz:   	AM \n\
	12 kHz:   	FM \n\
Bandwidths:\n\
	AM	Normal: 5 kHz,	Narrow: 0 Hz,	Wide: 0 Hz\n\
	CW	Normal: 2.2 kHz,	Narrow: 0 Hz,	Wide: 0 Hz\n\
	USB	Normal: 2.2 kHz,	Narrow: 0 Hz,	Wide: 0 Hz\n\
	LSB	Normal: 2.2 kHz,	Narrow: 0 Hz,	Wide: 0 Hz\n\
	FM	Normal: 12 kHz,	Narrow: 0 Hz,	Wide: 0 Hz\n";

writeCli(caps);

caps = "\
Has priv data:	Y\n\
Has Init:	Y\n\
Has Cleanup:	Y\n\
Has Open:	N\n\
Has Close:	N\n\
Can set Conf:	N\n\
Can get Conf:	N\n\
Can set Frequency:	Y\n\
Can get Frequency:	Y\n\
Can set Mode:	Y\n\
Can get Mode:	Y\n\
Can set VFO:	Y\n\
Can get VFO:	Y\n\
Can set PTT:	Y\n\
Can get PTT:	Y\n\
Can get DCD:	Y\n\
Can set Repeater Duplex:	N\n\
Can get Repeater Duplex:	N\n\
Can set Repeater Offset:	N\n\
Can get Repeater Offset:	N\n\
Can set Split Freq:	E\n\
Can get Split Freq:	E\n\
Can set Split Mode:	E\n\
Can get Split Mode:	E\n\
Can set Split VFO:	Y\n\
Can get Split VFO:	Y\n\
Can set Tuning Step:	N\n\
Can get Tuning Step:	N\n\
Can set RIT:	Y\n\
Can get RIT:	Y\n\
Can set XIT:	Y\n\
Can get XIT:	Y\n\
Can set CTCSS:	Y\n\
Can get CTCSS:	Y\n\
Can set DCS:	N\n\
Can get DCS:	N\n\
Can set CTCSS Squelch:	N\n\
Can get CTCSS Squelch:	N\n\
Can set DCS Squelch:	N\n\
Can get DCS Squelch:	N\n\
Can set Power Stat:	Y\n\
Can get Power Stat:	Y\n\
Can Reset:	Y\n\
Can get Ant:	N\n\
Can set Ant:	N\n\
Can set Transceive:	Y\n\
Can get Transceive:	Y\n\
Can set Func:	Y\n\
Can get Func:	Y\n\
Can set Level:	Y\n\
Can get Level:	Y\n\
Can set Param:	N\n\
Can get Param:	N\n\
Can send DTMF:	N\n\
Can recv DTMF:	N\n\
Can send Morse:	N\n\
Can decode Events:	N\n\
Can set Bank:	N\n\
Can set Mem:	Y\n\
Can get Mem:	Y\n\
Can set Channel:	N\n\
Can get Channel:	N\n\
Can ctl Mem/VFO:	Y\n\
Can Scan:	N\n\
Can get Info:	N\n\
Can get power2mW:	N\n\
Can get mW2power:	N\n\
\n\
Overall backend warnings: 0\n\
RPRT 0\n";

writeCli(caps);
}

//-------------------------------------------------------
