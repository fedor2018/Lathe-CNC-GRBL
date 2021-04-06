/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "main.h"
#include "grbl.h"
#include "lcd.h"
#include <stdlib.h>
/*
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
*/
char* find_next(char* msg){
    int i=0;
    while(msg[i]!='|' && msg[i]!='>' && msg[i]!=0){
        i++;
    }
    if(msg[i]==0)
        return (char*)0;//eom
    msg[i]=0;
    return (char*)(msg+i+1);//next
}

void grbl_scan(char* cmd){
    uint32 ch;
    char b[128];
    int i;
    char* p;
    char* n;

    while((ch=UART_UartGetChar())!=0){
        UART_DEBUG_PutChar(ch);
    }
    UART_UartPutCRLF((uint32) cmd);
    CyDelay(5);
    for(i=0;i < sizeof(b);i++){
        ch = UART_UartGetChar();
        b[i]=(char) ch;
        if(0u == ch)break;
        UART_DEBUG_PutChar(ch);
    }
    if(i>=sizeof(b)){
        b[sizeof(b)-1]=0;
    //??empty rx string
    }
    switch(b[0]){
        case '<'://status
            p=(char*)(b+1);
            n=find_next(p);
            lcd_msg(p);
            while(n!=0){
                p=n;
                n=find_next(p);
                if(strcmp((const char*)p,(const char*)"Wpos:")){//x,y,z
                    lcd_pos((char*)(p+5));
                }else if(strcmp(p, "F:")){//feed
                }else if(strcmp(p, "FS:")){//feed,rpm[,rpm?] FS:0,0[,0]
                    n=strchr(p, ',')+1;
                    p=strchr(n,',');
                    if(p)*p=0;//if 3 param
                    lcd_rpm(n);
                }
            }
            break;
        case 'A'://ALARM:X
        case 'e'://error:xx
            LCD_PrintString(b);
            break;
        case 'o'://ok
        default:
            return;
            break;
    }
}

/* [] END OF FILE */
