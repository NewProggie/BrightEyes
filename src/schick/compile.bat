@ECHO OFF
@REM -G Optimize speed, -G- optimize size (default)
@REM -O1 Smallest Code -O -Ob -Oe -Os -k- -Z
@REM -O2 Fastest Code -O -Ob -Oe -Og -Oi -Ol -Om -Op -Ot -Ov -k- -Z
@REM -j5 = #Errors
@REM
@REM --the hard cases
@REM -Os, -Od, -k- geht auch
@REM option -P can be enabled explicit	(force C++ compilation)
@REM option -k can be enabled explicit	(standart stack frame), but -k- also works
@REM option -a- can be enabled explicit	(disable word alignment)
@REM option -mlarge or -ml! are allowed	(memory model large with or without DSSTACK)
@REM option -K must be disabled 	(default char is unsigned), but this max be changed in the code
@REM option -1 can be enabled explicit	(80186 instructions), -2 also works, but never disable it with -1-
@REM option -Fm is not enabled		(shortcut for -Fc, -Ff and -Fs)
@REM option -Fc can be enabled explicit	(generate communal variables)
@REM option -Ff must be disabled	(generate far variables)
@REM option -Fs must be disabled	(assume ss == ds), but only 4 files are affected
@REM option -O is not enabled
@REM option -Od can be enabled		(disable optimization)
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG106.CPP
@REM
@REM --the compiling cases
@REM
@REM BCC.EXE -mlarge -O- -c -1 -Y -j5 SEG009.CPP
@REM
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG048.CPP
@REM
@REM -- here start the complete
@REM BCC.EXE -mlarge -O- -c -1 -Y SEG001.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Y -IAIL SEG002.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Y SEG003.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Y SEG004.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Y SEG005.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Y SEG006.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Y SEG007.CPP
@REM TASM.EXE /os /z SEG008.asm SEG008.OBJ
@REM TASM.EXE /os /z SEG009.asm SEG009.OBJ
@REM TASM.EXE /os /z SEG010.asm SEG010.OBJ
@REM TASM.EXE /m /w+ /ml /iAIL AIL\AIL.ASM SEG011.OBJ
@REM TASM.EXE /os /z SEG013.asm SEG013.OBJ
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG024.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG025.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG026.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG027.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG028.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG029.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG030.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG031.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG032.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG033.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG034.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG035.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG036.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG037.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG038.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG039.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG040.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG041.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG042.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG043.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG044.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG045.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG046.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG047.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG048.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG049.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG050.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG051.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG052.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG053.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG054.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG055.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG056.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG057.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG058.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG059.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG060.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG061.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG062.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG063.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG064.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG065.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG066.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG067.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG068.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG069.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG070.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG071.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG072.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG073.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG074.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG075.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG076.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG077.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG078.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG079.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG080.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG081.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG082.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG083.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG084.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG085.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG086.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG087.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG088.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG089.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG090.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG091.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG092.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG093.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG094.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG095.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG096.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG097.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG098.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG099.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG100.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG101.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG102.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG103.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG104.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG105.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG107.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG108.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG109.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG110.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG111.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG112.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG113.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG114.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG115.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG116.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG117.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG118.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG119.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG120.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG121.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo SEG122.CPP
@REM BCC.EXE -mlarge -O- -c -1 -Yo DATSEG.CPP
@REM -- here start the empty

@REM examine the ouput and press anykey
PAUSE
