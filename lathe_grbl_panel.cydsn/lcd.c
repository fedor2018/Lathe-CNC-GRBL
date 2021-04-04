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
#include "lcd.h"
#include <stdlib.h>
/*
  01234567890123456789|
0 M: x.xxmm  RPM: xxxx|
0 T: x.xxbpi RPM: xxxx|
0 F: x.xxmm  RPM: xxxx|
1 message             |
2 Z:-zzz.zz  X:-xxx.xx| 
3 -xxx.xx      -yyy.yy|
*/
extern MODE_INFO mode;
extern MOVE_INFO move;

uint8 Feeds[]={3,6,9,12,15,18,21};//mm*100
#define Feed_idx NELEMS(Feeds)
uint16 Thread[]={25,30,35,40,50,60,70,75,80,100,125,150,175,200};//,250,300,400,500,600};//mm*100
#define Thread_idx NELEMS(Thread)
//int Thread_rpm[]={1200,100};
uint8 Thread_tpi[]={80,72,64,60,56,48,44,40,36,32,28,27,26,24,22,20,19,18,16,14,12};//,10,9,8,7,6,5,4};//inch*1
#define Tpi_idx NELEMS(Thread_tpi)
//int Tpi_rpm[]={1200,100};

void clear_row(uint8 r){
    char b[21];
    space_str(b);
    LCD_Position(r, 0u);//row, col
    LCD_PrintString(b);    
}

void lcd_mode(){
    char buf[11];
    space_str(buf);
    LCD_Position(0u, 0u);//row, col
    LCD_PrintString(buf);
    LCD_Position(0u, 2u);//row, col
    switch(mode.Mode){
        case Mode_Thread:
            snprintf(buf,15,"M: %d.%2.2dmm   ", Thread[mode.Thread]/100, Thread[mode.Thread]%100);
            move.ks_div=THREAD_MM_STEP_I(Thread[mode.Thread]);
            move.km_div=THREAD_MM_STEP_D(Thread[mode.Thread]);
            break;
        case Mode_Tpi:
            snprintf(buf,15,"T:  %2dtpi   ", Thread_tpi[mode.Tpi]);
            move.ks_div=THREAD_TPI_STEP_I(Thread_tpi[mode.Tpi]);
            move.km_div=THREAD_TPI_STEP_D(Thread_tpi[mode.Tpi]);
            move.ks_fast=THREAD_TPI_STEP_I(Thread_tpi[Tpi_idx-1]);
//            move.ks_tmp=THREAD_TPI_STEP_I(Thread_tpi[0]);
//            move.ks_inc=mode.Tpi<1?1:(move.ks_tmp-move.ks_div)/mode.Tpi;//
            break;
        case Mode_Feed:
        default:
            snprintf(buf,15,"F: 0.%2.2dmm   ", Feeds[mode.Feed]);
            move.ks_div=FEED_MM_STEP(Feeds[mode.Feed]);
            move.km_div=0;
            move.ks_fast=FEED_MM_STEP(100);//1mm Feeds[Feed_idx-1]);
//            move.ks_inc=mode.Feed<1?1:(move.ks_tmp-move.ks_div)/mode.Feed;//
            break;
    }
    LCD_PrintString(buf);
}

void lcd_rpm(int rpm){
    char b[11];
    space_str(b);
    LCD_Position(1u, 11u);//row, col
    snprintf(b,10,"RPM: %04d", rpm);
    LCD_PrintString(b);
}

void lcd_msg(char * msg){
    char b[21];
    space_str(b);
    memcpy(b, msg, strlen(msg));
    LCD_Position(1u, 0u);//row, col
    LCD_PrintString(b);
}

void lcd_pos(char* zpos, char* xpos){
    char b[21];
    space_str(b);
    snprintf(b,20,"Z:%7s  Z:%7s",zpos, xpos);
    LCD_Position(2u, 0u);//row, col
    LCD_PrintString(b);
}

void lcd_limit(){
    char b[21];
    LCD_Position(3u, 7u);//row, col
    LCD_PrintString(b);
}

void version(char *sversion)
{
  const char data[]=__DATE__;
  const char tempo[]=__TIME__;
  const char nomes[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  char omes[4];
  int ano, mes, dia, hora, min, seg;
//  char sversion[]="130910.001339";
//  double fv;

  if(strlen(data)!=11||strlen(tempo)!=8)
    return;

  sscanf(data, "%s %d %d", omes, &dia, &ano);
  sscanf(tempo, "%d:%d:%d", &hora, &min, &seg);
  mes=(strstr(nomes, omes)-nomes)/3+1;
//  sprintf(sversion,"%04d%02d%02d.%02d%02d%02d", ano, mes, dia, hora, min, seg);
//  fv=atof(sversion);
  snprintf(sversion,20," Ver %s(%04d%02d%02d)", VER, ano, mes, dia);
  return;
}

/* [] END OF FILE */
