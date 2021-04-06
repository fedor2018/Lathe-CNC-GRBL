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
#include "beep.h"
#include "grbl.h"
#include "key.h"
#include "lcd.h"

extern FLAG_INFO flag;
extern uint16 volatile rpm;

void init(){
    UART_DEBUG_Start();
    UART_Start();
    beep_init();
    LCD_Start();
    LCD_ClearDisplay();
    grbl_scan(GRBL_INIT);
    key_init();
    CyGlobalIntEnable; /* Enable global interrupts. */
    LEFT_ON;
    RIGHT_ON;
    beep(100);
    CyDelay(500);
    LEFT_OFF;
    RIGHT_OFF;
    start_msg();
    CyDelay(500);
}

int main(void)
{
    init();
    LCD_ClearDisplay();
    for(;;){
        if(Pin_SW_Read()==0){
            CyDelay(500);
            if(Pin_SW_Read())CySoftwareReset();
        }
        joy_scan();
        grbl_scan("?");
        Pin_Led_Int_Write(~Pin_Led_Int_Read());
        CyDelay(50);
    }
}
/* [] END OF FILE */
