/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.0
        Device            :  PIC16F18877
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


#include "mcc_generated_files/mcc.h"
#include "essential.h"
#include "lcd.h"
#include <stdio.h>
#include "math.h"

/******************************************************************************/
int32_t preClockWiseDistace = 0;
int32_t preAntiClockWiseDistace = 0;
bit IR=0;
int32_t preClockWiseTime = 0;
int32_t preAntiClockWiseTime = 0;

int32_t clockWiseTime = 0;
int32_t antiClockWiseTime = 0;

int32_t disToBeCovered = 0;

uint8_t holdTime = 0;

uint16_t cnfHoldTime;

uint16_t speedRpm = 0;
uint8_t direction;
uint8_t preDirection;

int32_t totalRunTime = 0;

int stopCount = 0;
U8 SetParamCount;
U16 timeInMs, pretimeInMs;

U8 sec;
U8 min;
U8 doorStatus;
U16 keyCounter;
int32_t sensorPeriodicity,tempSamples,currentSensorSamples = 0;
int32_t battVoltPeriodicity,tempBattVoltSamples,battVoltSamples;
int32_t mainsVoltPeriodicity,tempMainsVoltSamples,mainsVoltSamples,mainVoltRef;

U8 sampleCount = 0;
U8 sampleCountBattVolt = 0;
U8 sampleCountMainsVolt = 0;
U8 adcValueChanged[2] = 0;
int32_t encoderTmr3 = 0;
int32_t encoderTmr5 = 0;
unsigned int integralValue;
unsigned int diffValue;

uint16_t tempSpeedRpm = 0;
/******************************************************************************/

/****
 *  for current sensor when load is more than expected
 */
void checkCurrentSensor(char dir, uint16_t speed)
{
    uint16_t tempEncoderVal = 0;
    
    forceStop = 0;    
    
    if(adcSamplingTimeExpired)
    {      
        tempSamples = ADCC_GetSingleConversion(channel_ANA0);
        if(sampleCount < CURRENT_SENSOR_SAMPLES)
        {
            sampleCount++;
            currentSensorSamples = tempSamples + currentSensorSamples;
            tempSamples = 0;
        }
        else
        {
            sampleCount = 0;                
            currentSensorSamples = tempSamples + currentSensorSamples;
            currentSensorSamples = currentSensorSamples/CURRENT_SENSOR_SAMPLES;

            if(currentSensorSamples > MIN_CURRENT_SENSOR_LIMIT)
            {

                               //0123456789012345
                display((char *)"Current: ",LCD_LINE2_ADDR);                 
                lcd((LCD_LINE2_ADDR+9),LCD_CMD);
                number_5(currentSensorSamples);
                forceStop = 1;
            }
            currentSensorSamples = tempSamples = 0;
        }
        adcSamplingTimeExpired = 0;    
    }
}
/********************************************/
void checkBatteryVoltage()
{

    tempBattVoltSamples = ADCC_GetSingleConversion(channel_ANA1);

    if(sampleCountBattVolt < BATTERY_VOLTAGE_SAMPLES)
    {
        sampleCountBattVolt++;
        battVoltSamples = tempBattVoltSamples + battVoltSamples;
        //tempSamples = 0;
    }
    else
    {
        sampleCountBattVolt = 0;                
        battVoltSamples = tempBattVoltSamples + battVoltSamples;
        battVoltSamples = battVoltSamples/CURRENT_SENSOR_SAMPLES;

        if(battVoltSamples < MIN_BATTERY_VOLTAGE_LIMIT && \
           mainVoltRef > MAINS_SUPPLY_AVAILABLE)
        {

                           //0123456789012345
            display((char *)"B: ",LCD_LINE1_ADDR);                 
            lcd((LCD_LINE1_ADDR+2),LCD_CMD);
            number_5(battVoltSamples);
            Battery_LED_RE2 = 0;
            BATT_CHARG_ON = 1;
        }
        else if(battVoltSamples > MAX_BATTERY_VOLTAGE_LIMIT)
        {
                           //0123456789012345
            display((char *)"B: ",LCD_LINE1_ADDR);                 
            lcd((LCD_LINE1_ADDR+2),LCD_CMD);
            number_5(battVoltSamples);
            Battery_LED_RE2 = 1;
            BATT_CHARG_ON = 0;            
        }
        else
        {
                           //0123456789012345
            display((char *)"B: ",LCD_LINE1_ADDR);                 
            lcd((LCD_LINE1_ADDR+2),LCD_CMD);
            number_5(battVoltSamples);
        }
        battVoltSamples = tempBattVoltSamples = 0;
    }
    adcSamplingBattVoltTimeExpired = 0;    
}

/********************************************/
void checkMainSupplyFailure()
{
	tempMainsVoltSamples = ADCC_GetSingleConversion(channel_ANA4);

	if(sampleCountMainsVolt < MAINS_VOLTAGE_SAMPLES)
	{
		sampleCountMainsVolt++;
		mainsVoltSamples = tempMainsVoltSamples + mainsVoltSamples;
		//tempSamples = 0;
	}
	else
	{
		sampleCountMainsVolt = 0;                
		mainsVoltSamples = tempMainsVoltSamples + mainsVoltSamples;
		mainsVoltSamples = mainsVoltSamples/CURRENT_SENSOR_SAMPLES;

					   //0123456789012345
		display((char *)"M: ",LCD_LINE1_ADDR+8);                 
		lcd((LCD_LINE1_ADDR+10),LCD_CMD);
		number_5(mainsVoltSamples);
		mainVoltRef = mainsVoltSamples;
	}
	adcSamplingMainSupply = 0;    
}

/********************************************/
void stopMotor(char dir, uint16_t speed)
{
    if(dir != CLOCKWISE)
    {
    // stop the motor
        for(int i = speed; i > 0; i-- ) {
            PWM3_LoadDutyValue(0);
            if(i > 1)
                PWM4_LoadDutyValue(i);             
            __delay_ms(RISE_FALL_TIME);
        }
    }
    else
    {
        // stop the motor
        for(int i = speed; i > 0; i-- ) {
            PWM4_LoadDutyValue(0);
            if(i > 1)
                PWM3_LoadDutyValue(i);             
            __delay_ms(RISE_FALL_TIME);
        }
    
    }
    PWM3_LoadDutyValue(0);
    PWM4_LoadDutyValue(0);

    MOTOR_RELAY = 0;
}
/********************************************/
void startMotor(char dir, uint16_t speed)
{
    MOTOR_RELAY = 1; 
    ADCON0bits.ADON = 1;

    if(dir != CLOCKWISE)
    {
    // start the motor
        for(int i = 0; i < speed; i++ ) {
            PWM3_LoadDutyValue(0);
            if(i < speed)
                PWM4_LoadDutyValue(i);             
            __delay_ms(RISE_FALL_TIME);
        }
        PWM4_LoadDutyValue(speed);             
    }
    else
    {
        // start the motor
        for(int i = 0; i < speed; i++ ) {
            PWM4_LoadDutyValue(0);
            if(i < speed)
                PWM3_LoadDutyValue(i);             
            __delay_ms(RISE_FALL_TIME);
        }
        PWM3_LoadDutyValue(speed);    
    }
 
}
/***
 * for clockwise rotation
 */
void ouputOnClockwise()
{

    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    RD2PPS = 0x0C;   //RD2->CCP4:CCP4;
    RD3PPS = 0x1D;   //RD3->CWG2:CWG2B;
    RD0PPS = 0x0B;   //RD0->CCP3:CCP3;
    RD1PPS = 0x06;   //RD1->CWG1:CWG1B;
    RC6PPS = 0x10;   //RC6->EUSART:TX;
    INTPPSbits.INTPPS = 0x00;   //RB0->EXT_INT:INT;

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS                        
                                                                     
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();         
    sensorPeriodicity = 0;
    
    speedReduced = 0;
    speedReduced1 = 0;
                   //0123456789012345
    display((char *)"D       C       ",LCD_LINE1_ADDR);   
//                
    display((char *)"OPEN:           ",LCD_LINE2_ADDR);   
//    
 
    timeInMs = 0;
    
    TMR2_StartTimer();                  
    TMR0_StartTimer();      
    
    preClockWiseDistace = preClockWiseTime;
    preAntiClockWiseDistace = preAntiClockWiseTime;
 
    disToBeCovered =  ((preClockWiseDistace * START_SPEED_REDUCING)/100);
    disToBeCovered = disToBeCovered * 10;
    
    lcd((LCD_LINE1_ADDR+1),LCD_CMD);
    number_5(disToBeCovered); 
    
    STARTED = 1;
    
    TMR3_WriteTimer(0);
    TMR5_WriteTimer(0);
    tempSpeedRpm = speedRpm;

    startMotor(CLOCKWISE,tempSpeedRpm);
    
    lcd((LCD_LINE1_ADDR + 7),LCD_CMD);
    number_5(encoderTmr3); 
    
    stopFlag = 0;
    
    while(STARTED)
    {	       
        //checkMainSupplyFailure();
        checkCurrentSensor(CLOCKWISE,tempSpeedRpm);
        if((encoderTmr5 >= disToBeCovered)) {        
           STARTED = 0;  
           break;
        }           
    }
      
    stopMotor(CLOCKWISE,tempSpeedRpm);
    
    lcd((LCD_LINE2_ADDR+6),LCD_CMD);
    number_5(encoderTmr5); 
    
    lcd((LCD_LINE2_ADDR+12),LCD_CMD);
    number_5(timeInMs);   

    STARTED = 0;
    stopFlag = 0;

    TMR0_StopTimer();
    TMR2_StopTimer();                

    PWM3_LoadDutyValue(0);                
    PWM4_LoadDutyValue(0);     

    doorOpened = 1;
    
    eeprom_write(DOOR_OPENED_OR_CLOSED , doorOpened);            
    __delay_ms(EEPROM_SAVE_DELAY);    

    //preAntiClockWiseTime
    
    unsigned char temp = preAntiClockWiseTime;        
    eeprom_write(ACLK_DISTANCE_TO_BE_COVERED_ADDR, temp);

    __delay_ms(EEPROM_SAVE_DELAY);

    temp = (uint8_t)((preAntiClockWiseTime >>8));

    eeprom_write(ACLK_DISTANCE_TO_BE_COVERED_ADDR+1, temp);

    __delay_ms(EEPROM_SAVE_DELAY);
        

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();

    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    RD2PPS = 0x00;   //RD2->CCP4:CCP4;
    RD3PPS = 0x00;   //RD3->CWG2:CWG2B;
    RD0PPS = 0x00;   //RD0->CCP3:CCP3;
    RD1PPS = 0x00;   //RD1->CWG1:CWG1B;
    RC6PPS = 0x00;   //RC6->EUSART:TX;
    INTPPSbits.INTPPS = 0x00;   //RB0->EXT_INT:INT;

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS                        

    __delay_ms(500);
    
    BUZZER = 1;
    __delay_ms(500);
    BUZZER = 0;
    
    default_menu();
}

/***
 *  for Anti clock wise rotation
 */
void ouputOnAntiClock()
{   
    uint16_t tempEncoderVal = 0;
    //char key = 0;
    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    RD2PPS = 0x0C;   //RD2->CCP4:CCP4;
    RD3PPS = 0x1D;   //RD3->CWG2:CWG2B;
    RD0PPS = 0x0B;   //RD0->CCP3:CCP3;
    RD1PPS = 0x06;   //RD1->CWG1:CWG1B;
    RC6PPS = 0x10;   //RC6->EUSART:TX;
    INTPPSbits.INTPPS = 0x00;   //RB0->EXT_INT:INT;

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS                        


    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    sensorPeriodicity = 0;

    TMR2_StartTimer();                  
    TMR0_StartTimer();  
    
    timeInMs = 0;

    TMR3_WriteTimer(0);
    TMR5_WriteTimer(0);  
        
                   //0123456789012345
    display((char *)"D       C       ",LCD_LINE1_ADDR);   
               
    display((char *)"CLOS:           ",LCD_LINE2_ADDR);   
      
    preClockWiseDistace = preClockWiseTime;
    preAntiClockWiseDistace = preAntiClockWiseTime;  
    
    disToBeCovered = ((preAntiClockWiseDistace * (START_SPEED_REDUCING))/100);
    
    disToBeCovered = disToBeCovered * 10;
  //  disToBeCovered = disToBeCovered * 2;   
    lcd((LCD_LINE1_ADDR+1),LCD_CMD);
    number_5(disToBeCovered); 
    
    STARTED = 1;

    tempSpeedRpm = speedRpm;
    
    startMotor(ANTICLOCKWISE,tempSpeedRpm);
    
    lcd((LCD_LINE1_ADDR+7),LCD_CMD);
    number_5(encoderTmr5); 
    
    doorOpenedWhileClosing = 0;
    irSensorDetected = 0; // if already obstacle detected then ignore it

    forceStop = 0;
 /////////////////////////////////        
    while(STARTED)
    {  
        
        checkCurrentSensor(ANTICLOCKWISE,tempSpeedRpm);
        
        if(encoderTmr5 >= disToBeCovered)
        {
            STARTED = 0;
            break;
        }  
        //MAGIC_KEY
        if(((FOOT_SWITCH == 0) || (MAGIC_SW == 0) || (START == 1) \
                  || (IR_SW == 1) || (forceStop == 1)) && \
                (doNotOperateWhileOpeningDoor == 0))
        {
            __delay_ms(50);
            if(((FOOT_SWITCH == 0) || (MAGIC_SW == 0) || (START == 1) \
                     || (IR_SW == 1) || (forceStop ==1) ) && \
                    (doNotOperateWhileOpeningDoor == 0))
            {
                //openedByFoot = 1;
                irSensorDetected = 1;

                // stop the motor                
                stopMotor(ANTICLOCKWISE,tempSpeedRpm);

                lcd((LCD_LINE1_ADDR+7),LCD_CMD);
                number_5(encoderTmr5);

//                disToBeCovered = encoderTmr3;// - START_STOP_OFFSET;//((encoderTmr3 * (START_SPEED_REDUCING + stopCount))/100) ;
                disToBeCovered = encoderTmr5 - START_STOP_OFFSET;//((encoderTmr3 * (START_SPEED_REDUCING + stopCount))/100) ;
                
                
                if(disToBeCovered < 0)
                    disToBeCovered = START_STOP_OFFSET;

                lcd((LCD_LINE1_ADDR+1),LCD_CMD);
                number_5(disToBeCovered); 
                BUZZER = 1;
                __delay_ms(500);
                
                __delay_ms(500);
                __delay_ms(500);

                BUZZER = 0;
                
                stopFlag = 0;
                forceStop = 0;

                tempEncoderVal = encoderTmr5;

                //restart the encoder 
                encoderTmr5 = 0;
                
                doNotOperateWhileOpeningDoor = 1;
             
                TMR3_WriteTimer(0);
                TMR5_WriteTimer(0);

                // restart the motor in clockwise direction
                startMotor(CLOCKWISE,tempSpeedRpm); 
                    IR = 1; doorOpened=1;
            }
        }     
    }
     
    if(irSensorDetected)
    {
        stopMotor(CLOCKWISE,tempSpeedRpm);
        doorOpened = 1;
    }
    else
    {
        // for stopping
        stopMotor(ANTICLOCKWISE,tempSpeedRpm);

        doorOpened = 0;
        if(openedByFoot)
            openedByFoot = 0;
        stopCount = 0;        
    }
    // door open or close status - write to memory
    eeprom_write(DOOR_OPENED_OR_CLOSED , doorOpened);            
    __delay_ms(EEPROM_SAVE_DELAY);    
    
    doNotOperateWhileOpeningDoor = 0;
    lcd((LCD_LINE2_ADDR+6),LCD_CMD);
    number_5(encoderTmr5); 
    
    lcd((LCD_LINE2_ADDR+12),LCD_CMD);
    number_5(timeInMs);

    stopFlag = 0;
    irSensorDetected = 0;

    MOTOR_STOP = 0; // H-bridge stop
    
    TMR0_StopTimer();
    TMR2_StopTimer();               
    
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();

    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    RD2PPS = 0x00;   //RD2->CCP4:CCP4;
    RD3PPS = 0x00;   //RD3->CWG2:CWG2B;
    RD0PPS = 0x00;   //RD0->CCP3:CCP3;
    RD1PPS = 0x00;   //RD1->CWG1:CWG1B;
    RC6PPS = 0x00;   //RC6->EUSART:TX;
    INTPPSbits.INTPPS = 0x00;   //RB0->EXT_INT:INT;

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS                        

    __delay_ms(500);
    BUZZER = 1;
    __delay_ms(500);
    BUZZER = 0;
    default_menu();    
}

/**
 *  when door open and close by itself
 * 
 **/
void outputOn(void)
{
    while(STARTED)
    {
        
        switch(direction)
        {
            case CLOCKWISE:
                IR=0;        
                ouputOnClockwise();
            
                //uint16_t tempSpeedRpm = 0;
                // Enable the Global Interrupts
                INTERRUPT_GlobalInterruptEnable();

                // Enable the Peripheral Interrupts
                INTERRUPT_PeripheralInterruptEnable();         

 
                timeInMs = 0;
                
                sec = 0;
                min = 0;
                updateTime = 0;

                TMR0_StartTimer();

                //DISP_ON
                lcd(DISP_ON,LCD_CMD);
                               /*1234567890123456*/
                display((char *)"                ",LCD_LINE1_ADDR);                
                // reset the encoder the measure the door distance again 
                display((char *)"HT:     T:  :   ",LCD_LINE1_ADDR);        
                
                lcd((LCD_LINE1_ADDR+3),LCD_CMD);
                number_3(cnfHoldTime);
                lcd((LCD_LINE1_ADDR + 10),LCD_CMD);
                number_2(min);
                lcd((LCD_LINE1_ADDR + 13),LCD_CMD);
                number_2(sec);
               //  wait for Hold time before closing the door
              //  @TODO check time to keep the door open before start closing
                while((cnfHoldTime - 1) >= sec)
                {
                    if(updateTime)
                    {
                        lcd((LCD_LINE1_ADDR + 10),LCD_CMD);
                        number_2(min);
                        lcd((LCD_LINE1_ADDR + 13),LCD_CMD);
                        number_2(sec);
                    }
                }
                
                sec = 0;
                
                if(IR_SW == 1) 
                {
                    __delay_ms(100);
                    while(IR_SW == 1);
                }
                
                 ouputOnAntiClock();
                    __delay_ms(50);
                   //__delay_ms(500);
                  //  __delay_ms(500);
                   // __delay_ms(500);
                    while((IR==1)&&(doorOpened==1))
                    {
                          // Enable the Global Interrupts
                INTERRUPT_GlobalInterruptEnable();

                // Enable the Peripheral Interrupts
                INTERRUPT_PeripheralInterruptEnable(); 
                      timeInMs = 0;
                
                sec = 0;
                min = 0;
                updateTime = 0;

                TMR0_StartTimer();

                //DISP_ON
                lcd(DISP_ON,LCD_CMD);
                               /*1234567890123456*/
                display((char *)"                ",LCD_LINE1_ADDR);                
                // reset the encoder the measure the door distance again 
                display((char *)"HT:     T:  :   ",LCD_LINE1_ADDR);        
                
                lcd((LCD_LINE1_ADDR+3),LCD_CMD);
                number_3(cnfHoldTime);
                lcd((LCD_LINE1_ADDR + 10),LCD_CMD);
                number_2(min);
                lcd((LCD_LINE1_ADDR + 13),LCD_CMD);
                number_2(sec);
               //  wait for Hold time before closing the door
              //  @TODO check time to keep the door open before start closing
                while((cnfHoldTime - 1) >= sec)
                {
                    if(updateTime)
                    {
                        lcd((LCD_LINE1_ADDR + 10),LCD_CMD);
                        number_2(min);
                        lcd((LCD_LINE1_ADDR + 13),LCD_CMD);
                        number_2(sec);
                    }
                }
                
                  sec = 0;
                  
                if(IR_SW == 1) 
                {
                    __delay_ms(100);
                    while(IR_SW == 1);
                }
                
                   //startMotor(ANTICLOCKWISE,tempSpeedRpm);
                   ouputOnAntiClock();
                      __delay_ms(50);
                   // __delay_ms(500);
                  //  __delay_ms(500);
                  //  __delay_ms(500);
                  
                    }
                      
           
                break;
            case ANTICLOCKWISE:
                //ouputOnAntiClock();
                break;                                
        }           

    }
}

/***
 *  default start display
 */
void default_menu(void)
{
    lcd(LCD_CLEAR_CMD,LCD_CMD);
    
//    display((char *)"RPM CT  AT  HT D",LCD_LINE1_ADDR);                            
        display((char *)"                ",LCD_LINE1_ADDR);                            
    lcd((LCD_LINE2_ADDR),LCD_CMD);
    number_3(speedRpm);          
    lcd((LCD_LINE2_ADDR+4),LCD_CMD);
    number_3(clockWiseTime);          
    lcd((LCD_LINE2_ADDR+8),LCD_CMD);
    number_3(antiClockWiseTime);          
    lcd((LCD_LINE2_ADDR+12),LCD_CMD);
    number_2(holdTime);                      

    if(direction == STOP)
        display((char *)"S",LCD_LINE2_ADDR+15);        
    else if(direction == CLOCKWISE)
        display((char *)"C",LCD_LINE2_ADDR+15);
    else if(direction == ANTICLOCKWISE)
        display((char *)"A",LCD_LINE2_ADDR+15);

                    //0123456789012345
    display((char *)"B: ",LCD_LINE1_ADDR);                 
    lcd((LCD_LINE1_ADDR+2),LCD_CMD);  
    number_5(battVoltSamples);
                   //0123456789012345
    display((char *)"M: ",LCD_LINE1_ADDR);                 
    lcd((LCD_LINE1_ADDR+10),LCD_CMD);
    number_5(mainsVoltSamples);    

}

/******************************************************************************
                         Main application
 ******************************************************************************/
void main(void)
{
    int temp = 0;
    char cmpName[] = "ALTOS";
    char cmpName1[] = "ENGINEERS P. LTD";
    
    unsigned char tempStatus,txParam,command = 0;
    // initialize the device
    SYSTEM_Initialize();
   
    /**
    ANSELx registers
    */   
    ANSELC = 0x00;
    ANSELB = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;
    //ANSELA = 0xFC;
    ANSELA = 0x13;

    TRISA = 0x3F;
    TRISB = 0x01;
    TRISC = 0xFF;
    TRISD = 0x00;    
    TRISE = 0x07;    

    PORTB = 0x00;        
    PORTC = 0x00;
    PORTD = 0x00;
    PORTC = 0x00;
        
    default_init();
    
    __delay_ms(20);
    
    TMR2_StopTimer();
    
    PWM3_LoadDutyValue(0);
    PWM4_LoadDutyValue(0);
    
    lcdInit();
    
    lcd(LCD_CLEAR_CMD,LCD_CMD);
                   /*1234567890123456*/
    display((char *)COMPANY_NAME,LCD_LINE1_ADDR);
    display((char *)COMPANY_NAME1,LCD_LINE2_ADDR);    
    
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
   INTERRUPT_PeripheralInterruptEnable();

    //EUSART_Initialize();
    PIE3bits.TXIE = 0;
    PIE3bits.RCIE = 1;    
    // Disable the Global Interrupts
  //  INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
  //  INTERRUPT_PeripheralInterruptDisable();
    
    SET = INC = DEC = START = 0;
        
    MAGIC_SW = 0;
    OPENSIDE_MAGNET1 = 1;
    IR_SW = 0;
    MOTOR_RELAY = 0;
    FOOT_SWITCH = 1;
    
    Battery_LED_RE2 = 0;
    
    BUZZER = 1;
    
    for(int i = 0; i < 10; i++) // 2 second delay
        __delay_ms(50);
    BUZZER = 0;
    
    initMemRead();
    
    default_menu();    
    
    SetParamCount = 0;
    
    integralValue = 0;
    diffValue = 0;
    
 // for testing the encoders below code is commented
    
    timeInMs = 0;    
    
    txParam = 0x30;        

    PIE3bits.RCIE = 1;    
    
    temp = sizeof(cmpName);

    for(char i = 0; i < (temp-1); i++)
    {
        putch(cmpName[i]);
        __delay_ms(1);
    }
    
    putch(' ');
    __delay_ms(1);
    
    temp = sizeof(cmpName1);
    
    for(char i = 0; i < (temp-1); i++)
    {
        putch(cmpName1[i]);
        __delay_ms(1);
    }
    putch(' ');
    __delay_ms(1);

    //putch(temp + 0x30);
   // printf("Logs enabled for testing");

    RC1STAbits.CREN = 1;
    PIE3bits.RCIE = 1;    
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();     

    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    RD2PPS = 0x00;   //RD2->CCP4:CCP4;
    RD3PPS = 0x00;   //RD3->CWG2:CWG2B;
    RD0PPS = 0x00;   //RD0->CCP3:CCP3;
    RD1PPS = 0x00;   //RD1->CWG1:CWG1B;
    RC6PPS = 0x00;   //RC6->EUSART:TX;
    INTPPSbits.INTPPS = 0x00;   //RB0->EXT_INT:INT;

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS                        
    
    GIE = 1;
    STARTED = 0;
    
    command = 0;
    
    cnfHoldTime = holdTime;
    doorOpenedWhileClosing = 0;
    irSensorDetected = 0;
    doorOpened = 0;
    totalRunTime = 0;
    //test  Encoder();
    preClockWiseTime = (clockWiseTime);
    preAntiClockWiseTime = (antiClockWiseTime);

#if 1  // read the door status if already openned then close it- full distace    
    if(doorStatus)
    {
        uint8_t tmp[2];
        preDirection = direction;
        
        ouputOnAntiClock();  

        // antiClockWiseTime 
        antiClockWiseTime = DEFAULT_CONFIGURED_ANTI_CLOCK_WISE_TIME;
        tmp[0] = (uint8_t) (antiClockWiseTime);// && 0x00FF);
        tmp[1] = (uint8_t) (antiClockWiseTime >> 8);// && 0x00FF);
        
        eeprom_write(ACLK_DISTANCE_TO_BE_COVERED_ADDR , tmp[0]);            
        __delay_ms(EEPROM_SAVE_DELAY);
        
        eeprom_write(ACLK_DISTANCE_TO_BE_COVERED_ADDR+1, tmp[1]);            
        __delay_ms(EEPROM_SAVE_DELAY);
        
        default_menu();       
    }
#endif 
    
    TMR0_StartTimer();  
         
    while (1)
    {

    
        if(adcSamplingBattVoltTimeExpired)
            checkBatteryVoltage();
        
        if(adcSamplingMainSupply)
            checkMainSupplyFailure();
#if 1       
        command = keypad();
        serialCommand();  
        
        switch(command)
        {
            case FOOT_KEY:
            //if(!irSensorDetected)
            {
                if(IR_SW)
                {
                    BUZZER = 1;
                    __delay_ms(50);
                    if(IR_SW)
                    {
                         display((char *)"IR Obstacle ",LCD_LINE1_ADDR);
                         __delay_ms(500);
                         irSensorDetected = 1;
                        //return;
                         
                    }
                    BUZZER = 0;
                }
                else
                {
                    irSensorDetected = 0;
                    BUZZER = 0;    
                }
                
                if((STARTED == 0) && (irSensorDetected == 0))
                {
                    STARTED = 1;                
                    MOTOR_STOP = 0;

                    //display((char *)"Foot Switch",LCD_LINE1_ADDR);
                    preDirection = direction;
                    //preDirection = STOP;
             
                    preClockWiseTime = clockWiseTime;
                    preAntiClockWiseTime = antiClockWiseTime;
                    
                    if(direction == STOP)
                    {                        

                        if((doorOpened == 0)&&(openedByFoot == 0))                      //for full open direct by foot switch
                        {
                          //display((char *)"Foot Clock",LCD_LINE1_ADDR);

                            //if(!doorOpenedWhileClosing)
                                ouputOnClockwise();  
                                openedByFoot = 1;
                                doorOpenedWhileClosing = 0;
                        }
                        // half open condition door is in half open state
                        else if( (doorOpened == 1) &&(openedByFoot == 0))             //for full-open after magic-switch
                        {                       
                            openedByFoot = 1;
                            preClockWiseTime = ((clockWiseTime) * HALF_DOOR_OPEN_MULTIPLIER)/100;
                            preAntiClockWiseTime = ((antiClockWiseTime) * HALF_DOOR_OPEN_MULTIPLIER)/100;                 
    //                        if(!doorOpenedWhileClosing)
                            ouputOnClockwise(); 

    //                        doorOpenedWhileClosing = 0;
                        } // Door Already open  in full open state                
                        else if((doorOpened == 1) && (openedByFoot == 1))                 //for door closing.
                        {
                            //doorOpened = 1;                                                                                      
                            ouputOnAntiClock();
                        }        
                    }
                    else   // clockwise and anti clock wise                             
                    {
                        DoorClosed = 1;
                        doorOpened = 0;
                        STARTED = 1;
                        outputOn();
                        openedByFoot = 0;
                        doorOpenedWhileClosing = 0;
                    }

                    __delay_ms(500);
                    __delay_ms(500);
                      GIE = 1;
                      STARTED = 0;
                    //doorOpenedWhileClosing = 0;
                    // Enable the Global Interrupts
                    INTERRUPT_GlobalInterruptEnable();

                    // Enable the Peripheral Interrupts
                    INTERRUPT_PeripheralInterruptEnable();
                }
                else
                    irSensorDetected = 0;
              } 
            break;
            
            case START_KEY:   
            case MAGIC_KEY:
                // to prevent double click for start operation
                //if((!irSensorDetected) /*&& (IR_SW == 0)*/)
                {
                    if(IR_SW)
                    {
                        __delay_ms(50);
                        if(IR_SW)
                        {
                             display((char *)"IR Obstacle ",LCD_LINE1_ADDR);
                             __delay_ms(500);
                            irSensorDetected = 1;
                             //return;
                        }
                    }
                    else{
                        irSensorDetected = 0;
                    
                    }

                    if((STARTED == 0) && (irSensorDetected == 0))
                    {

                        STARTED = 1;        
                        preDirection = direction;
                        // if door already opened by Foot Switch then 
                        // start key will close the door as per default distance covered by Door

                        if(openedByFoot)
                        {
                            preClockWiseTime = (clockWiseTime);
                            preAntiClockWiseTime = (antiClockWiseTime);
                        }
                        else
                        { // if door is opened by Start/Magic switch then distance 
                            //covered with be less than the default
                            // now configured for 72% of total distance
                            preClockWiseTime = ((clockWiseTime * 72)/100 ) ;
                            preAntiClockWiseTime = ((antiClockWiseTime *72) /100);
                        }
                    
                        if(direction == STOP)
                        {                        
                            if((doorOpened == 0) && (doorOpenedWhileClosing == 0))
                            {
                                lcd((LCD_LINE2_ADDR),LCD_CMD);
                                number_3(speedRpm);           
                                ouputOnClockwise(); 

                            }
                            else //if((doorOpened == 1) )
                            {
                                //display((char *)" ANTICLOCK      ",LCD_LINE1_ADDR);
                                lcd((LCD_LINE2_ADDR),LCD_CMD);
                                number_3(speedRpm);                 
                                
                                ouputOnAntiClock();                             
                                //doorOpened = 0;
                                     
                            }
                            __delay_ms(500);
                            __delay_ms(500);
                        }
                        else   // clockwise and anti clock wise 
                        {
                            DoorClosed = 1;
                            doorOpened = 0;
                            STARTED = 1;
                            outputOn();
                            openedByFoot = 0;
                            doorOpenedWhileClosing = 0;
                        }
                        GIE = 1;
                        // Enable the Global Interrupts
                        INTERRUPT_GlobalInterruptEnable();

                        // Enable the Peripheral Interrupts
                        INTERRUPT_PeripheralInterruptEnable();
                    }
                    else
                    {
                        irSensorDetected = 0;
                    
                    }
                }

                break;
                            
            case SET_KEY:
                
                upDwnKeyPressed = 0;
                keyCounter = 0;

                TMR0_StopTimer();
                
                if(SetParamCount >= 4) // 0 - 4
                    SetParamCount = 0;
                else
                    SetParamCount++;
            
                switch(SetParamCount)
                {
                    case SET_RPM:                        
                        saveConfiguration(SET_DIRECTION, direction); 

                        lcd(LCD_LINE2_ADDR,LCD_CMD);            
                        lcd(DISP_ON_B, LCD_CMD);    
                        __delay_ms(DISPLAY_DELAY);                        
                        break;
                        
                    case SET_CLOCKWISE_TIME:
                        
                        saveConfiguration(SET_RPM, speedRpm); 
                        
                        lcd(LCD_LINE2_ADDR +4,LCD_CMD);            
                        lcd(DISP_ON_B, LCD_CMD);                                                
                        __delay_ms(DISPLAY_DELAY);                      
                        break;                    
                        
                    case SET_ANTI_CLOCKWISE_TIME:
                        
                        saveConfiguration(SET_CLOCKWISE_TIME, clockWiseTime); 
                        
                        lcd(LCD_LINE2_ADDR +8,LCD_CMD);            
                        lcd(DISP_ON_B, LCD_CMD);  
                        __delay_ms(DISPLAY_DELAY);                        
                        break;                    
                        
                    case SET_HOLD_TIME:      
                        
                        saveConfiguration(SET_ANTI_CLOCKWISE_TIME, antiClockWiseTime); 
                        
                        lcd(LCD_LINE2_ADDR +12,LCD_CMD);            
                        lcd(DISP_ON_B, LCD_CMD);  
                        __delay_ms(DISPLAY_DELAY);
                        break;                        
                    case SET_DIRECTION:      
                        
                        saveConfiguration(SET_HOLD_TIME, holdTime);                     
                        
                        lcd(LCD_LINE2_ADDR +15,LCD_CMD);            
                        lcd(DISP_ON_B, LCD_CMD);  
                        __delay_ms(DISPLAY_DELAY);
                        break;                        
                        
                    default:
                        break;
                }                
                break;
                
            case INC_KEY:
                switch(SetParamCount)
                {
                    case SET_RPM:
                        if(speedRpm < 661)
                            //speedRpm++;                        
                        speedRpm = speedRpm++;                        
                        
                        lcd((LCD_LINE2_ADDR),LCD_CMD);
                        number_3(speedRpm);          
                        lcd((LCD_LINE2_ADDR),LCD_CMD);
                        lcd(DISP_ON_B, LCD_CMD);  
                        __delay_ms(DISPLAY_DELAY);                        
        
                        if(STARTED)                        
                        {
                            if(direction == CLOCKWISE)
                            {
                                PWM4_LoadDutyValue(0);
                                PWM3_LoadDutyValue(speedRpm);                                
                            }
                            else
                            {
                                PWM3_LoadDutyValue(0);
                                PWM4_LoadDutyValue(speedRpm);
                            }                        
                        }                        
                        saveConfiguration(SET_RPM, speedRpm);                        
                        break;
                    case SET_CLOCKWISE_TIME:

                        if(clockWiseTime < 1500)
                            clockWiseTime++;
                                                
                        lcd((LCD_LINE2_ADDR+3),LCD_CMD);
                        number_4(clockWiseTime);  
                        lcd((LCD_LINE2_ADDR+3),LCD_CMD);
                        lcd(DISP_ON_B, LCD_CMD);  
                        __delay_ms(DISPLAY_DELAY);
                        
                        saveConfiguration(SET_CLOCKWISE_TIME, clockWiseTime);                        
                        
                        break;                        
                    case SET_ANTI_CLOCKWISE_TIME:
 
                        if(antiClockWiseTime < 1500)
                            antiClockWiseTime++;
                                                
                        lcd((LCD_LINE2_ADDR+8),LCD_CMD);
                        number_4(antiClockWiseTime);          

                        lcd((LCD_LINE2_ADDR+8),LCD_CMD);
                        lcd(DISP_ON_B, LCD_CMD);  
                        __delay_ms(DISPLAY_DELAY);
                        
                        saveConfiguration(SET_ANTI_CLOCKWISE_TIME, antiClockWiseTime);                        
                        
                        break;                        
                    case SET_HOLD_TIME:  

                        if(holdTime > 99)
                            holdTime = 99;
                        else
                            holdTime++;
                          
                        lcd((LCD_LINE2_ADDR+12),LCD_CMD);
                        number_2(holdTime);          
                        lcd((LCD_LINE2_ADDR+12),LCD_CMD);
                        lcd(DISP_ON_B, LCD_CMD);  
                        __delay_ms(DISPLAY_DELAY);
                        
                        cnfHoldTime = holdTime;
                        
                        saveConfiguration(SET_HOLD_TIME, holdTime); 
                        
                        break;                    
                    case SET_DIRECTION:      
                            if(direction < MAX_DIRECTION)
                                direction++;
                            else
                                direction = CLOCKWISE;

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
                            lcd(LCD_LINE2_ADDR +15,LCD_CMD);            
                            lcd(DISP_ON_B, LCD_CMD);  
                            __delay_ms(DISPLAY_DELAY);

                            saveConfiguration(SET_DIRECTION, direction); 
                    default:
                        break;                        
                }                                
                break;
                
            case DEC_KEY:
                switch(SetParamCount)
                {
                    case SET_RPM:

                        if(speedRpm)
                            speedRpm--;                        

                        lcd((LCD_LINE2_ADDR),LCD_CMD);
                        number_3(speedRpm);          
                        lcd((LCD_LINE2_ADDR),LCD_CMD);
                         lcd(DISP_ON_B, LCD_CMD);  
                        __delay_ms(DISPLAY_DELAY);

                        if(STARTED)                        
                        {
                            if(direction == CLOCKWISE)
                            {
                                PWM4_LoadDutyValue(0);
                                PWM3_LoadDutyValue(speedRpm);                                
                            }
                            else
                            {
                                PWM3_LoadDutyValue(0);
                                PWM4_LoadDutyValue(speedRpm);
                            }                        
                        }                    
                    saveConfiguration(SET_RPM, speedRpm); 
                    
                    break;
                case SET_CLOCKWISE_TIME:

                    if(clockWiseTime > 1)
                        clockWiseTime--;
                        
                    lcd((LCD_LINE2_ADDR+3),LCD_CMD);
                    number_4(clockWiseTime);   
                    lcd((LCD_LINE2_ADDR+3),LCD_CMD);
                     lcd(DISP_ON_B, LCD_CMD);  
                    __delay_ms(DISPLAY_DELAY);

                    saveConfiguration(SET_CLOCKWISE_TIME, clockWiseTime); 
                    break;                        
                    
                case SET_ANTI_CLOCKWISE_TIME:
                    
                    if(antiClockWiseTime > 1)
                        antiClockWiseTime--;

                    lcd((LCD_LINE2_ADDR+8),LCD_CMD);
                    number_4(antiClockWiseTime);
                    lcd((LCD_LINE2_ADDR+8),LCD_CMD);
                    lcd(DISP_ON_B, LCD_CMD);  
                    __delay_ms(DISPLAY_DELAY);
                    
                    saveConfiguration(SET_ANTI_CLOCKWISE_TIME, antiClockWiseTime); 
                    break;                        
                    
                case SET_HOLD_TIME:  

                    if(holdTime > 1)
                        holdTime--;
                    
                    lcd((LCD_LINE2_ADDR+12),LCD_CMD);
                    number_2(holdTime);       
                    lcd((LCD_LINE2_ADDR+12),LCD_CMD);
                    
                    lcd(DISP_ON_B, LCD_CMD);  
                    __delay_ms(DISPLAY_DELAY);
                    
                    cnfHoldTime = holdTime;
                    
                    saveConfiguration(SET_HOLD_TIME, holdTime);                     
                    break;                        
                    
                case SET_DIRECTION:      
                    upDwnKeyPressed = 0;
                    keyCounter = 0;
                    // Enable the Global Interrupts
                    INTERRUPT_GlobalInterruptDisable();

                    // Enable the Peripheral Interrupts
                    INTERRUPT_PeripheralInterruptDisable();

                    TMR0_StopTimer();
                    
                    if(direction > 1)
                        direction--;
                    else
                        direction = 0;

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
                    lcd(LCD_LINE2_ADDR +15,LCD_CMD);            
                    lcd(DISP_ON_B, LCD_CMD);  
                    __delay_ms(DISPLAY_DELAY);
                    // need to verify if it is getting saved or not before moving to set param
                    saveConfiguration(SET_DIRECTION, direction);                     
                    break;                        

                default:
                    break;                        
                }                
                
                break;
            default:
              command = 0;  
              break;
        }
        command = 0;  
#endif 
    }

}
/**
 End of File
*/