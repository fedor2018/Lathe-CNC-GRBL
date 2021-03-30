# Lathe-CNC-GRBL

```
  01234567890123456789|
0   M: x.xxmm    |
0   UTS: x.xxbpi   |
0     Feed: x.xxmm    |
1   CCW Ang xxx.x     |
1   ACW RPM  xxxx     |
2     << -zzz.zz >>   | 
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
```

```
$10=0 #Wpos only
?
<Idle|WPos:-20.000,0.000,0.000|Bf:15,128|FS:0,0|WCO:20.000,0.000,0.000>
<Idle|WPos:-20.000,0.000,0.000|Bf:15,128|FS:0,0|Ov:100,100,100>
$J=G21G91X-0.1F1000
$J=G21G91X0.1F1000
G4P0 - stop streaming
$X - Kill alarm lock
G95 - mm/r

```


