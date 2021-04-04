# Lathe-CNC-GRBL

```
  01234567890123456789|
0 M: x.xxmm RPM: xxxx |
0 T: x.xxbpi RPM: xxxx|
0 F: x.xxmm RPM: xxxx |
1 ????     |
2 Z:-zzz.zz X:-xxx.xx | 
3 -xxx.xx SSSS -yyy.yy|

SSSS:
```
Idle
Run
Hold
Door
Home
Alarm
Check
Jog
Sleep
Tool

```

```
$10=0 #Wpos only
?
<Idle|WPos:-20.000,0.000,0.000|Bf:15,128|FS:0,0|WCO:20.000,0.000,0.000>
<Idle|WPos:-20.000,0.000,0.000|Bf:15,128|FS:0,0|Ov:100,100,100>
<Idle|MPos:0.000,0.000,0.000|Bf:35,1024|FS:0,0,0|Pn:Y|WCO:0.000,0.000,0.000|Ov:100,100,100|A:M>
$J=G21G91X-0.1F1000
$J=G21G91X0.1F1000
G4P0 - stop streaming
$X - Kill alarm lock
G95 - mm/r

```


