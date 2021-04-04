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
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include <stdlib.h>

uint16 joy=0;
extern int32 volatile Motor_Z_Pos;
extern uint16 volatile rpm;


MODE_INFO mode;
FLAG_INFO flag;
MOVE_INFO move;

void key_init(){
    mode.Mode=Mode_Feed;
    mode.Feed=2;
    mode.Thread=9;
    mode.Tpi=0;
    flag.joy=Key_None;
    flag.rpm=0;
    lcd_row1();

    ADC_Start();
    ISR_ADC_StartEx(ISR_ADC_Handler);
    ADC_StartConvert();
    ISR_KEY_ClearPending();
    ISR_KEY_StartEx(ISR_KEY_Handler); 
}

CY_ISR(ISR_KEY_Handler){
    flag.fast=Pin_Fast_Read()?0:1;
}

CY_ISR(ISR_ADC_Handler){/* 0-156-261-350-450-550-650-750-850-950-1024 */
    static int16 old=0;
    int16 j=ADC_GetResult16(0);
    if(j==old){
        joy=j/200;//12bit->0-10
    }
    old=j;
}

void joy_scan(){
    if(joy!=flag.joy){
        if(joy<Key_None&&flag.joy<Key_None){//unknown
            flag.joy=joy;
            return;
        }else{
            if(flag.joy==Key_None){//none->move
                flag.fast=0;
                switch(joy){
                    case Joy_Left:
                        Pin_Dir_Write(DIR_LEFT);
                        move_en();
                        break;
                    case Joy_Right:
                        Pin_Dir_Write(DIR_RIGHT);
                        move_en();
                        break;
                    case Key_Down:
                        MenuKeyDownPressed();
                        break;
                    case Key_Up:
                        MenuKeyUpPressed();
                        break;
                    case Key_Select:
                        MenuKeySelectPressed();
                        break;
                    case Limit_Left_Button:
                        LimitLeft();
                        break;
                    case Limit_Right_Button:
                        LimitRight();
                        break;
                }
            }else{//move->end
                 switch(joy){
                    case Key_None:
                        switch(move.state){
                            case st_accel:
                            case st_fast:
                            case st_run:
                                //div_print('n',st_decel);
                                move.state=st_decel;
                                break;
                            case st_end:
                            case st_wait:
                                //DIV_ClearInterrupt(DIV_INTR_MASK_TC|DIV_INTR_MASK_CC_MATCH);
                                //DIV_Stop();
                                move.state=st_stop;
                                STEP_OFF;
                                break;
                            case st_decel:
                            default:
                                break;
                        }
                        break;
                }
           }
        }
        flag.joy=joy;
        lcd_mode();
        beep(20);
    }
}

void MenuKeySelectPressed(){
    mode.Mode++;
    if(mode.Mode>=Mode_None){
        mode.Mode=0;
	}
}

void MenuKeyUpPressed(){
  switch (mode.Mode){
    case Mode_Thread:
      if (mode.Thread < Thread_idx-1){
        mode.Thread++;
      }
      break;
    case Mode_Tpi:
      if (mode.Tpi < Tpi_idx-1){
        mode.Tpi++;
      }
      break;
    case Mode_Feed:
      if (mode.Feed < Feed_idx-1){
        mode.Feed++;
      }
      break;
  }
}

void MenuKeyDownPressed(){
  switch (mode.Mode){
    case Mode_Thread:
      if (mode.Thread > 0){
        mode.Thread--;
      }
      break;
    case Mode_Tpi:
      if (mode.Tpi > 0){
        mode.Tpi--;
      }
      break;
    case Mode_Feed:
      if (mode.Feed > 0){
        mode.Feed--;
      }
      break;
  }
//	setParam();
}

void LimitSwap(){
    return;
    if(move.lim_left>move.lim_right){
        long tmp=move.lim_left;
        move.lim_left=move.lim_right;
        move.lim_left=tmp;
    }
}

void LimitLeft(){
    char b[16];
    
	if(move.state==st_stop){
        if (LEFT_IS_ON){//led on?
            LEFT_OFF;
            snprintf(b, 8,"%8s"," ");
        }else{
            if(RIGHT_IS_ON){//led on
                move.lim_left=Motor_Z_Pos;
            }else{
                move.lim_left=Motor_Z_Pos=0;
            }
            LEFT_ON;
            LimitSwap();
            fmt_pos(b);
        }
       	beep(10);
        LCD_Position(3u, 0u);//row, col
        LCD_PrintString(b);
	}
}

void LimitRight(){
    char b[16];
    
	if(move.state==st_stop){
        if (RIGHT_IS_ON){//led on?
            RIGHT_OFF;
            snprintf(b, 7,"%7s", " ");
        }else{
            if(LEFT_IS_ON){//led on
                move.lim_right=Motor_Z_Pos;
            }else{
                move.lim_right=Motor_Z_Pos=0;
            }
            RIGHT_ON;
            LimitSwap();
            fmt_pos(b);
        }
       	beep(10);
        LCD_Position(3u, 13u);//row, col
        LCD_PrintString(b);
	}
}

/* [] END OF FILE */
