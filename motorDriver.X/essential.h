
/* 
 * File:   essential.h
 * Author: Mani
 *
 * Created on October 16, 2016, 3:14 AM
 */

#ifndef ESSENTIAL_H
#define	ESSENTIAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcc_generated_files/mcc.h"
    
typedef unsigned char U8;
typedef unsigned long long U32;
typedef unsigned int U16;

#define TRUE            (1)
#define FALSE           (0)

#define COMPANY_NAME    "     ALTOS      "
#define COMPANY_NAME1   "ENGINEERS P. LTD"
#define ENCODER_MAX_VALUE         (9999)

/******************************************************************************/
#define EEPROM_PARAM_NO                     (0x0F)
#define EEPROM_WRITE_ADDR                   (EEPROM_BASE_ADDR + EEPROM_PARAM_NO)
#define WRITE_TO_EEPROM(x,y)  eeprom_write(x, y);
/*****************************************************************************/
#define PPR_CONST                   (10)
#define CURR_CALCULATED_OFFSET      (4)
#define TEMP_CALCULATED_OFFSET      (8)

#define DISPLAY_DELAY               (150)

#define TEMP_SENSOR_SAMPLES         (5)
#define CURRENT_SENSOR_SAMPLES      (TEMP_SENSOR_SAMPLES)
#define BATTERY_VOLTAGE_SAMPLES     (TEMP_SENSOR_SAMPLES)
#define MAINS_VOLTAGE_SAMPLES       (TEMP_SENSOR_SAMPLES)

#define ADC_CHECK_PERIODICITY        (50)  //ms
#define ADC_TEMP_CURRENT_PERIODICITY (ADC_CHECK_PERIODICITY)

#define ADC_BATT_VOLT_CHECK_PERIODICITY (500)

#define ADC_MAINS_VOLT_CHECK_PERIODICITY (500)

#if 0 // setting for small groove with motor
#define  DEFAULT_CONFIGURED_MOTOR_SPEED             (350)
#define  DEFAULT_CONFIGURED_CLOCK_WISE_TIME         (345)
#define  DEFAULT_CONFIGURED_ANTI_CLOCK_WISE_TIME    (335)
#define  DEFAULT_CONFIGURED_HOLD_TIME               (5)
#endif 

 // setting for small groove with motor
#define  DEFAULT_CONFIGURED_MOTOR_SPEED             (48)

#define  DEFAULT_CONFIGURED_CLOCK_WISE_TIME         (1270)
#define  DEFAULT_CONFIGURED_ANTI_CLOCK_WISE_TIME    (1290)

#define  DEFAULT_CONFIGURED_HOLD_TIME               (10)

#define THRESHOLD_OVER                               (2500) //in ms
#define MILISEC_TO_SEC                              (1000)
#define SEC_TO_MIN                                  (60)
#define HOLD_TIME_MULTIPLIER                        (SEC_TO_MIN*MILISEC_TO_SEC)
#define DOOR_CLOSE_TIME_OFFSET                      (24)

#define EEPROM_SAVE_DELAY                (3)
#define PREAMBLE                         (0x11)
#define EEPROM_START_ADDR               (0x05)
#define EEPROM_ADDR_OFFSET              (0x01)
#define SAVED_BYTES                     (0x02)

#define RPM_TIME_ADDR   /*0x06,0x07*/   (EEPROM_START_ADDR + EEPROM_ADDR_OFFSET) // 0x06
#define CT_TIME_ADDR   /*0x08,0x09*/    (0x08)//(EEPROM_START_ADDR + EEPROM_ADDR_OFFSET + SAVED_BYTES)  // 2bytes
#define AT_TIME_ADDR   /*0x0A,0x0B */   (0x0A)//(EEPROM_START_ADDR + EEPROM_ADDR_OFFSET + SAVED_BYTES + SAVED_BYTES)  // 2bytes
#define HOLD_TIME_ADDR /*0x0C*/         (0x0C)//(EEPROM_START_ADDR + EEPROM_ADDR_OFFSET + SAVED_BYTES + SAVED_BYTES + EEPROM_ADDR_OFFSET)
#define DIR_ADDR       /*0x0D*/         (0x0D)//(EEPROM_START_ADDR + EEPROM_ADDR_OFFSET + SAVED_BYTES + SAVED_BYTES + SAVED_BYTES)

#define DOOR_OPENED_OR_CLOSED           0x29
#define RUN_TIME_DIRECTON_ADDR          0x30
#define RUN_TIME_DISTANCE_ADDR          0x31
#define CLK_DISTANCE_TO_BE_COVERED_ADDR     0x32

#define ACLK_DISTANCE_TO_BE_COVERED_ADDR     0x34

#define MIN_BATTERY_VOLTAGE_LIMIT                   (200)
#define MAX_BATTERY_VOLTAGE_LIMIT                   (250)

#define MAINS_SUPPLY_AVAILABLE                      (1000)
#define MIN_CURRENT_SENSOR_LIMIT                   (800)
#define MAX_CURRENT_SENSOR_LIMIT                   (1500)
#define RISE_FALL_TIME                             (12)
#define KEY_DISP_DELAY_VALUE                      (220)

#define CURR_MONITOR    (RA0)  //

#define BATT_MONITOR    (RA1)  //

#define IR_SW           (RB0)

#define IR_SW1          (RA2)
#define IR_SW2          (RA3)

#define SET             (RC4)  // RB1

#define INC             (RC1)
#define DEC             (RC2)

#define START           (RC0) 

#define MAGIC_SW        (RA5)   // magnetic switch active low
//#define MAGIC_SW        (RC0)

#define FOOT_SWITCH     (RE1)  // magnetic switch active low


#define OPENSIDE_MAGNET1 (RE0)
#define OPENSIDE_MAGNET2 (RE1)

#define OPENSIDE_MAGNET4 (RE2)


#define RD0_FORWARD_HIGH  RD0
#define RD1_FORWARD_LOW   RD1
#define RD2_REVERSE_HIGH  RD2
#define RD3_REVERSE_LOW   RD3

#define MOTOR_STOP      (RD4)

#define BATT_CHARG_ON   (RD5)  //active high -- cut off the charging 
#define Battery_LED_RE2   (RE2)

#define BUZZER          (RD6)  

#define MOTOR_RELAY     RD7

#define MAIN_POWER      RA4

#define RS485_DE        RA7
#define RS485_RE        RA6

#define MAGIC_SW_DOOR_OPEN_MULTIPLIER       (83)

#define HALF_DOOR_OPEN_MULTIPLIER        (100 - MAGIC_SW_DOOR_OPEN_MULTIPLIER)
#define START_SPEED_REDUCING             (80)

#define START_STOP_OFFSET       (1106)

/*****************************************************************************/
typedef enum serialCmds {
    START_CMD = 'q',
    STOP_CMD = 'w',
    INC_SPEED = 'a',
    DEC_SPEED = 'b',
    CHANGE_DIRECTION = 'c',
    INC_HOLD_TIME = 'd',                        
    DEC_HOLD_TIME = 'e',                        
    INC_CLOCK_WISE_TIME = 'f',
    DEC_CLOCK_WISE_TIME = 'g',
    INC_ANTI_CLOCK_WISE_TIME = 'h',
    DEC_ANTI_CLOCK_WISE_TIME = 'i',
    NO_CMD = 0x00,        
};

typedef enum keys {
    START_KEY = 1,
    SET_KEY,
    INC_KEY,
    DEC_KEY,
    FOOT_KEY,
    IR_SWITCH,
    MAGIC_KEY
};
typedef enum dir {
    CLOCKWISE = 0,
    ANTICLOCKWISE = 1,
    STOP = 2,
    MAX_DIRECTION
};

typedef enum ParamType 
{
    SET_RPM = 0, 
    SET_CLOCKWISE_TIME = 1, 
    SET_ANTI_CLOCKWISE_TIME = 2,
    SET_HOLD_TIME = 3,            
    SET_DIRECTION = 4,
    MAX_PARAMS        
            
}paramType;

/******************************************************************************/
bit STARTED;
bit doorOpened;
bit DoorClosed;
bit openedByFoot;
bit doorOpenedWhileClosing;
bit upDwnKeyPressed;
bit adcSamplingTimeExpired;
bit adcSamplingBattVoltTimeExpired;
bit adcSamplingMainSupply;
bit speedReduced = 0;
bit speedReduced1 = 0;
bit speedReduced2 = 0;
bit stopByIR  = 0;
bit addIntegValue = 0;
bit subDiffValue = 0;
bit incDecSpeed = 0;
bit onByFootSw = 0;
bit doNotOperateWhileOpeningDoor = 0;
bit irSensorDetected = 0;
bit stopFlag = 0;
bit updateTime = 0;
bit mainPowerFailure = 0;
bit forceStop = 0;
/******************************************************************************/
extern U8 SetParamCount;
extern uint16_t cnfHoldTime;
extern U16 timeInMs;
extern U16 pretimeInMs;
extern U8 CountMs;
extern U8 sec;
extern U8 min;
extern U8 watt;
extern U16 keyCounter;
extern int32_t sensorPeriodicity,battVoltPeriodicity;
extern int32_t mainsVoltPeriodicity,tempMainsVoltSamples,mainsVoltSamples;

extern int32_t totalRunTime;
extern  uint16_t tempSpeedRpm;
extern int32_t clockWiseTime;
extern int32_t antiClockWiseTime;

extern int32_t preClockWiseTime;
extern int32_t preAntiClockWiseTime;
extern int32_t encoderTmr3;
extern int32_t encoderTmr5;
extern int runTimeEncoderValue;
extern uint8_t holdTime;
extern uint16_t speedRpm;
extern uint8_t direction;
extern uint8_t preDirection;
extern int32_t disToBeCovered;
extern U8 doorStatus;
extern U8 sampleCountMainsVolt;
//unsigned int disToBeCovered;

extern void setParamDisplay(char type, U16 value);
extern unsigned char serialCommand(void);
extern unsigned char keypad(void);
extern void stopMotor(char dir, uint16_t speed);
extern void checkCurrentSensor(char dir, uint16_t speed);
extern void saveConfiguration( char configTYpe, uint16_t value);
extern uint16_t readConfiguration( char configTYpe);
extern void initMemRead();
extern void default_init();
extern void default_menu(void);
extern void footSwitchOperation();
extern void ouputOnAntiClock();
extern void ouputOnClockwise();
extern void number_5(U32 num);
extern void checkMainSupplyFailure();
extern void startMotor(char dir, uint16_t speed);
/******************************************************************************/
#ifdef	__cplusplus
}
#endif

#endif	/* ESSENTIAL_H */

