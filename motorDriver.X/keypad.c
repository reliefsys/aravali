#include "mcc_generated_files/mcc.h"
#include "essential.h"
#include "lcd.h"

unsigned char keypad(void)
{
    unsigned char retKey = 0;
    if(START)
    {
        __delay_ms(100);
        //if(START)
        while(START);
        retKey = START_KEY;
    }
    if(SET)
    {
        __delay_ms(50);
        if(SET)
        retKey = SET_KEY;
    }
    if(INC)
    {
        __delay_ms(50);
        if(INC)
        retKey = INC_KEY;
    }
    if(DEC)
    {
        __delay_ms(50);
        if(DEC)
        retKey = DEC_KEY;
    }
    if(!FOOT_SWITCH)
    {
        __delay_ms(100);
        while(!FOOT_SWITCH);
        //if(!FOOT_SWITCH)
            retKey = FOOT_KEY;
    }    
    if(!MAGIC_SW)
    {
        __delay_ms(50);
        if(!MAGIC_SW)
            retKey = MAGIC_KEY;
    }
#if 0
    if(!IR_SW)
    {
        __delay_ms(50);
        while(!IR_SW);
        //if(!IR_SW); //@TODO check the operation for IR
        //retKey = IR_SWITCH;
        //retKey = MAGIC_KEY;
    } 
#endif 
    
    return retKey;
}
