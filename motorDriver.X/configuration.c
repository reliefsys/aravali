#include "lcd.h"
#include "essential.h"
#if 0
uint16_t readConfiguration( char configTYpe)
{
    uint8_t temp[2];
    uint16_t retValue = 0;
    
    switch(configTYpe)
    {
        case SET_RPM:            
            
            temp[0] =  eeprom_read(RPM_TIME_ADDR); // DATAEE_ReadByte(RPM_TIME_ADDR);            
            __delay_ms(EEPROM_SAVE_DELAY);
            temp[1] =  eeprom_read(RPM_TIME_ADDR+1); // DATAEE_ReadByte(RPM_TIME_ADDR+1);            
            __delay_ms(EEPROM_SAVE_DELAY);            
            
            retValue = (uint16_t)((temp[1] << 8) || temp[0]);
            
            break;            
        case SET_CLOCKWISE_TIME:
                        
            temp[0] =  eeprom_read(CT_TIME_ADDR); // DATAEE_ReadByte(CT_TIME_ADDR);            
            __delay_ms(EEPROM_SAVE_DELAY);
            temp[1] =  eeprom_read(CT_TIME_ADDR+1); // DATAEE_ReadByte(CT_TIME_ADDR+1);            
            __delay_ms(EEPROM_SAVE_DELAY);            
            
            retValue = (uint16_t)((temp[1] << 8) || temp[0]);
            
            break;            
        case SET_ANTI_CLOCKWISE_TIME:            

            temp[0] =  eeprom_read(AT_TIME_ADDR); // DATAEE_ReadByte(AT_TIME_ADDR);            
            __delay_ms(EEPROM_SAVE_DELAY);
            temp[1] =  eeprom_read(AT_TIME_ADDR+1); // DATAEE_ReadByte(AT_TIME_ADDR+1);            
            __delay_ms(EEPROM_SAVE_DELAY);            
            
            retValue = (uint16_t)((temp[1] << 8) || temp[0]);
            
            break;            
        case SET_HOLD_TIME:

            temp[0] =  eeprom_read(HOLD_TIME_ADDR); // DATAEE_ReadByte(HOLD_TIME_ADDR);            
            __delay_ms(EEPROM_SAVE_DELAY);
            
            retValue =  temp[0];
            
            break;            
        case SET_DIRECTION:
            temp[0] =  eeprom_read(DIR_ADDR); // DATAEE_ReadByte(DIR_ADDR);            
            __delay_ms(EEPROM_SAVE_DELAY);
            
            retValue = temp[0];
            
            break;            
        default:            
            retValue = 0xFFFF;
            break;
    }
    
    return retValue;
}
#endif 
void saveConfiguration( char configTYpe, uint16_t value)
{
    uint8_t temp;

    switch(configTYpe)
    {
        case SET_RPM:            
            temp = (uint8_t) (value);// && 0x00FF);
            eeprom_write(RPM_TIME_ADDR, temp);
            __delay_ms(EEPROM_SAVE_DELAY);
            
            temp = (uint8_t)((value >> 8));
            eeprom_write(RPM_TIME_ADDR+1, temp);
     
            __delay_ms(EEPROM_SAVE_DELAY);
            
            break;            
        case SET_CLOCKWISE_TIME:
            
            temp = (uint8_t) (value);// && 0x00FF);
            eeprom_write(CT_TIME_ADDR, temp);

            __delay_ms(EEPROM_SAVE_DELAY);
            
             temp = (uint8_t)((value >>8));
             eeprom_write(CT_TIME_ADDR+1, temp);
             __delay_ms(EEPROM_SAVE_DELAY);
            
            break;            
        case SET_ANTI_CLOCKWISE_TIME:
            temp = (uint8_t) (value); //&& 0x00FF);
            
            eeprom_write(AT_TIME_ADDR, temp);

            __delay_ms(EEPROM_SAVE_DELAY);
            
            temp = (uint8_t)((value >>8));

            eeprom_write(AT_TIME_ADDR+1, temp);

            __delay_ms(EEPROM_SAVE_DELAY);
            
            break;            
        case SET_HOLD_TIME:
            temp = (uint8_t) (value);// && 0x00FF);            

            eeprom_write(HOLD_TIME_ADDR, temp);
            __delay_ms(EEPROM_SAVE_DELAY);            
            
            break;            
        case SET_DIRECTION:

            eeprom_write(DIR_ADDR, (uint8_t)value);
            __delay_ms(EEPROM_SAVE_DELAY);         
            
            break;            
        default:            
            break;
    }

}

void initMemRead()
{
    uint8_t incR = 0;
    uint8_t temp[2], var[2];

    incR = EEPROM_START_ADDR;

//    incR = EEPROM_START_ADDR;
    var[0] = eeprom_read(incR);
    __delay_ms(EEPROM_SAVE_DELAY);
    
    if(var[0] != 0x11)
    {
        eeprom_write(EEPROM_START_ADDR, (uint8_t)0x11);
        __delay_ms(EEPROM_SAVE_DELAY);

        speedRpm = DEFAULT_CONFIGURED_MOTOR_SPEED;
        temp[0] = (uint8_t) (speedRpm);// && 0x00FF);
        temp[1] = (uint8_t) (speedRpm >> 8);// && 0x00FF);

        speedRpm = 0;

        incR++;
        eeprom_write(incR , temp[0]);            
        __delay_ms(EEPROM_SAVE_DELAY);
        incR++;
        eeprom_write(incR, temp[1]);            
        __delay_ms(EEPROM_SAVE_DELAY);
        // clocWiseTime 
        clockWiseTime = DEFAULT_CONFIGURED_CLOCK_WISE_TIME;
        temp[0] = (uint8_t) (clockWiseTime);// && 0x00FF);
        temp[1] = (uint8_t) (clockWiseTime >> 8);// && 0x00FF);

        clockWiseTime = 0;

        incR++;
        eeprom_write(incR , temp[0]);            
        __delay_ms(EEPROM_SAVE_DELAY);
        incR++;
        eeprom_write(incR, temp[1]);            
        __delay_ms(EEPROM_SAVE_DELAY);

        // antiClockWiseTime 
        antiClockWiseTime = DEFAULT_CONFIGURED_ANTI_CLOCK_WISE_TIME;
        temp[0] = (uint8_t) (antiClockWiseTime);// && 0x00FF);
        temp[1] = (uint8_t) (antiClockWiseTime >> 8);// && 0x00FF);

        antiClockWiseTime = 0;

        incR++;
        eeprom_write(incR , temp[0]);            
        __delay_ms(EEPROM_SAVE_DELAY);
        incR++;
        eeprom_write(incR, temp[1]);            
        __delay_ms(EEPROM_SAVE_DELAY);

        // holdTime 
        holdTime = DEFAULT_CONFIGURED_HOLD_TIME;     
        cnfHoldTime = holdTime * HOLD_TIME_MULTIPLIER;
        temp[0] = (uint8_t) (holdTime);  
        incR++;
        eeprom_write(incR , temp[0]);            
        __delay_ms(EEPROM_SAVE_DELAY);    

        // Direction 
        direction = STOP;
        temp[0] = (uint8_t) (direction);  
        incR++;
        eeprom_write(incR , temp[0]);            
        __delay_ms(EEPROM_SAVE_DELAY);  
        
        doorStatus = 0;
        // assumed door is by default closed - 0 means close, 1 means open
        eeprom_write(DOOR_OPENED_OR_CLOSED , doorStatus);            
        __delay_ms(EEPROM_SAVE_DELAY); 

        // antiClockWiseTime 
        antiClockWiseTime = DEFAULT_CONFIGURED_ANTI_CLOCK_WISE_TIME;
        temp[0] = (uint8_t) (antiClockWiseTime);// && 0x00FF);
        temp[1] = (uint8_t) (antiClockWiseTime >> 8);// && 0x00FF);

        antiClockWiseTime = 0;

        incR = ACLK_DISTANCE_TO_BE_COVERED_ADDR;
        
        eeprom_write(incR , temp[0]);            
        __delay_ms(EEPROM_SAVE_DELAY);
        incR++;
        eeprom_write(incR, temp[1]);            
        __delay_ms(EEPROM_SAVE_DELAY);
        
    }   
    //
    // read the parameters 
    //
    
    incR = EEPROM_START_ADDR;       // 0x05
    var[0] = eeprom_read(incR);     // 0x11
    __delay_ms(EEPROM_SAVE_DELAY);

    incR++;                             //0x06
    
    var[0] = eeprom_read(incR);      // lower byte of RPM
    __delay_ms(EEPROM_SAVE_DELAY);
    
    incR++;                         // 0x07
    
    var[1] = eeprom_read(incR);     // high byte of RPM
    __delay_ms(EEPROM_SAVE_DELAY);
    
    speedRpm = (uint16_t)((var[1] << 8) | var[0]);   //
    
    lcd((LCD_LINE2_ADDR + LCD_SPEED_RPM_COLUMN),LCD_CMD);
    number_3(speedRpm);          
    
    //clockWiseTime
    incR++;                         //0x08
    
    var[0] = eeprom_read(incR);     //lower byte of Clockwise time
    __delay_ms(EEPROM_SAVE_DELAY);
    
    incR++;                        // 0x09
    
    var[1] = eeprom_read(incR);    //high byte of Clockwise time 
    __delay_ms(EEPROM_SAVE_DELAY);
    
    clockWiseTime = (uint16_t)((var[1] << 8) | var[0]); 
    
    lcd((LCD_LINE2_ADDR + LCD_CLOCK_WISE_COLUMN),LCD_CMD);
    number_3(clockWiseTime);          

    //antiClockWiseTime
    incR++;                         //0x0A
    
    var[0] = eeprom_read(incR);     // lower byte of AntiClockwise time
    __delay_ms(EEPROM_SAVE_DELAY);
    
    incR++;                        // 0x0B
    
    var[1] = eeprom_read(incR);    // higher byte of AntiClockWise time
    __delay_ms(EEPROM_SAVE_DELAY);
    
    antiClockWiseTime = (uint16_t)((var[1] << 8) | var[0]); 
    
    lcd((LCD_LINE2_ADDR + LCD_ANTI_CLOCK_WISE_COLUMN),LCD_CMD);
    number_3(antiClockWiseTime);          

    //holdTime
    incR++;                     // 0x0C
    
    holdTime = eeprom_read(incR);   // hold byte 
    __delay_ms(EEPROM_SAVE_DELAY);
  
    lcd((LCD_LINE2_ADDR + LCD_HOLD_TIME_COLUMN),LCD_CMD);
    number_2(holdTime);          

    //direction
    incR++;                 // 0x0D
    
    direction = eeprom_read(incR);  // direction byte
    __delay_ms(EEPROM_SAVE_DELAY);
  
//    lcd((LCD_LINE2_ADDR + LCD_DIRECTION_COLUMN),LCD_CMD);
//    number_3(direction);
    switch(direction)
    {
        case CLOCKWISE:
        display((char *)"C",LCD_LINE2_ADDR + LCD_DIRECTION_COLUMN);                            
        break;
        case ANTICLOCKWISE:
        display((char *)"A",LCD_LINE2_ADDR + LCD_DIRECTION_COLUMN);                            
        break;
        case STOP:
        default:            
        display((char *)"S",LCD_LINE2_ADDR + LCD_DIRECTION_COLUMN);                            
        break;
    }
    // fixed address for storing the door status in EEPROM
    doorStatus = eeprom_read(DOOR_OPENED_OR_CLOSED);  // direction byte
    __delay_ms(EEPROM_SAVE_DELAY);
   
    if(doorStatus)
    {
        incR = ACLK_DISTANCE_TO_BE_COVERED_ADDR;
        var[0] = eeprom_read(incR);     // lower byte of AntiClockwise time
        __delay_ms(EEPROM_SAVE_DELAY);

        incR++;                        // 0x0B

        var[1] = eeprom_read(incR);    // higher byte of AntiClockWise time
        __delay_ms(EEPROM_SAVE_DELAY);

        antiClockWiseTime = (uint16_t)((var[1] << 8) | var[0]); 
    }
}

/**
 * 
 * 
 */
void default_init()
{
    direction = CLOCKWISE;
    clockWiseTime = 0;
    antiClockWiseTime = 0;
    holdTime = 0;
    speedRpm = 0;
}