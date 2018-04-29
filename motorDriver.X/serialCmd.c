#include "mcc_generated_files/mcc.h"
#include "essential.h"
#include "lcd.h"

unsigned char serialCommand(void)
{
    unsigned char rcvCmd = 0;    
    rcvCmd = getch();

    if(rcvCmd >= '0' && rcvCmd < 'z')
    {
        switch(rcvCmd)
        {
            case START_CMD:
                STARTED = 1;                
                MOTOR_STOP = 0;

                display((char *)"Foot Switch",LCD_LINE1_ADDR);
                //preDirection = direction;
                preDirection = STOP;
                preClockWiseTime = clockWiseTime;
                preAntiClockWiseTime = antiClockWiseTime;
                
                if((doorOpened == 0) && (openedByFoot == 0))
                {
                    
                    display((char *)"Foot Clock",LCD_LINE1_ADDR);
                    openedByFoot = 1;
                    ouputOnClockwise();                         
                }
                // half open condition door is in half open state
                else if( (doorOpened == 1) && (openedByFoot == 0) )
                {                       
                    openedByFoot = 1;
                    
                    display((char *)"Already Open",LCD_LINE1_ADDR);
                    //doorOpened = 1;
                    preClockWiseTime = (clockWiseTime * HALF_DOOR_OPEN_MULTIPLIER)/100;
                    preAntiClockWiseTime = (antiClockWiseTime * HALF_DOOR_OPEN_MULTIPLIER)/100;                 
                    ouputOnClockwise();                       
                } // Door Already open  in full open state                
                else if((doorOpened == 1) && (openedByFoot == 1))
                {                       
                    doorOpened = 1;                                                                                      
                    ouputOnAntiClock();
                    openedByFoot =0;
                    doorOpened = 0;
                    
                }
                __delay_ms(500);
                __delay_ms(500);
                GIE = 1;
                // Enable the Global Interrupts
                INTERRUPT_GlobalInterruptEnable();

                // Enable the Peripheral Interrupts
                INTERRUPT_PeripheralInterruptEnable();
                
                
                break;

            case STOP_CMD:
                STARTED = 1;                
                MOTOR_STOP = 0;

                display((char *)"Foot Switch",LCD_LINE1_ADDR);
                //preDirection = direction;
                preDirection = STOP;
                preClockWiseTime = clockWiseTime;
                preAntiClockWiseTime = antiClockWiseTime;
                
                if((doorOpened == 0) && (openedByFoot == 0))
                {
                    
                    display((char *)"Foot Clock",LCD_LINE1_ADDR);
                    openedByFoot = 1;
                    ouputOnClockwise();                         
                }
                // half open condition door is in half open state
                else if( (doorOpened == 1) && (openedByFoot == 0) )
                {                       
                    openedByFoot = 1;
                    
                    display((char *)"Already Open",LCD_LINE1_ADDR);
                    //doorOpened = 1;
                    preClockWiseTime = (clockWiseTime * HALF_DOOR_OPEN_MULTIPLIER)/100;
                    preAntiClockWiseTime = (antiClockWiseTime * HALF_DOOR_OPEN_MULTIPLIER)/100;                 
                    ouputOnClockwise();                       
                } // Door Already open  in full open state                
                else if((doorOpened == 1) && (openedByFoot == 1))
                {                       
                    doorOpened = 1;                                                                                      
                    ouputOnAntiClock();
                    openedByFoot =0;
                    doorOpened = 0;
                    
                }
                __delay_ms(500);
                __delay_ms(500);
                GIE = 1;
                // Enable the Global Interrupts
                INTERRUPT_GlobalInterruptEnable();

                // Enable the Peripheral Interrupts
                INTERRUPT_PeripheralInterruptEnable();
                
                break;

            case INC_SPEED:
                rcvCmd = INC_SPEED;
#if 1                
                speedRpm++;

                lcd((LCD_LINE2_ADDR),LCD_CMD);
                number_3(speedRpm);                              
#endif 
                saveConfiguration(SET_RPM, speedRpm); 
                
                rcvCmd = 0x00;
                break;

            case DEC_SPEED:
                
                rcvCmd = DEC_SPEED;
#if 1                           
                speedRpm--;
     
                lcd((LCD_LINE2_ADDR),LCD_CMD);
                number_3(speedRpm);                              
#endif 
                saveConfiguration(SET_RPM, speedRpm); 
                
                rcvCmd = 0x00;
                break;

            case CHANGE_DIRECTION:
                rcvCmd = CHANGE_DIRECTION;
#if 1
                if(direction >= STOP)
                    direction = CLOCKWISE;
                else
                    direction++;

                switch(direction)
                {
                    case CLOCKWISE:
                    display((char *)"C",LCD_LINE2_ADDR+15);                            
                    break;
                    case ANTICLOCKWISE:
                    display((char *)"A",LCD_LINE2_ADDR+15);                            
                    break;
                    case STOP:
                    display((char *)"S",LCD_LINE2_ADDR+15);                            
                    break;
                }                    
#endif 
                saveConfiguration(SET_DIRECTION, direction); 
                
                break;

            case INC_HOLD_TIME:
                rcvCmd = INC_HOLD_TIME;
#if 1                
                if(holdTime < 99)
                    holdTime++;
                else                        
                    holdTime = 99;

                cnfHoldTime = holdTime * HOLD_TIME_MULTIPLIER;

                lcd((LCD_LINE2_ADDR+12),LCD_CMD);
                number_2(holdTime);                           
#endif 
                saveConfiguration(SET_HOLD_TIME, holdTime); 
                
                break;

            case DEC_HOLD_TIME:
                
                rcvCmd = DEC_HOLD_TIME;
#if 1                
                if(holdTime > 1)
                    holdTime--;

                cnfHoldTime = holdTime * HOLD_TIME_MULTIPLIER;

                lcd((LCD_LINE2_ADDR+12),LCD_CMD);
                number_2(holdTime);                           
#endif 
                saveConfiguration(SET_HOLD_TIME, holdTime); 
                break;

            case INC_CLOCK_WISE_TIME:
                rcvCmd = INC_CLOCK_WISE_TIME;
#if 1                
                clockWiseTime++;
                lcd((LCD_LINE2_ADDR+4),LCD_CMD);
                number_3(clockWiseTime);   
#endif 
                saveConfiguration(SET_CLOCKWISE_TIME, clockWiseTime); 
                
                break;

            case DEC_CLOCK_WISE_TIME:
                rcvCmd = DEC_CLOCK_WISE_TIME;        
#if 1                
                if(clockWiseTime > 1)
                    clockWiseTime--;

                lcd((LCD_LINE2_ADDR+4),LCD_CMD);
                number_3(clockWiseTime);   
#endif 
                saveConfiguration(SET_CLOCKWISE_TIME, clockWiseTime); 
                break;

            case INC_ANTI_CLOCK_WISE_TIME:
                rcvCmd = INC_ANTI_CLOCK_WISE_TIME;        
#if 1                
                antiClockWiseTime++;
                rcvCmd = INC_ANTI_CLOCK_WISE_TIME; 

                lcd((LCD_LINE2_ADDR+8),LCD_CMD);
                number_3(antiClockWiseTime);                    
#endif 
                saveConfiguration(SET_ANTI_CLOCKWISE_TIME, antiClockWiseTime);                        
                break;

            case DEC_ANTI_CLOCK_WISE_TIME:
                rcvCmd = DEC_ANTI_CLOCK_WISE_TIME;        
#if 1                
                if(antiClockWiseTime > 1)                        
                    antiClockWiseTime--;                 

                lcd((LCD_LINE2_ADDR+8),LCD_CMD);
                number_3(antiClockWiseTime);                    
#endif 
                saveConfiguration(SET_ANTI_CLOCKWISE_TIME, antiClockWiseTime);                        
                break;

            default:
                break;            
        }                    
    }
    else
    {
        rcvCmd = 0x30;        
        //putch(rcvCmd);
    }
    return rcvCmd;
}
