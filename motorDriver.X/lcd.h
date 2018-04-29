/* 
 * File:   lcd.h
 * Author: Mani
 *
 * Created on October 16, 2016, 3:10 AM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include "essential.h"
//#define EXIT_SUCCESS    (2)

#define OFFSET_10       (10)

#define LCD_RS          (RB2)
//#define LCD_RW          (RB3)
#define LCD_E           (RB3)
    
#define LCD_DATA        (PORTB)

#define LCD_DELAY       (1000)
    
#define MAX_LCD_CHAR     (20)
#define ASCII           (48)
#define LCD_CMD         (0)
#define LCD_CMD_DATA    (1)
#define LCD_LINE1_ADDR  (0x80)
#define LCD_LINE2_ADDR  (0xC0)
#ifdef LCD20X4
#define LCD_LINE3_ADDR  (0x94)
#define LCD_LINE4_ADDR  (0xD4)
#else 
#define LCD_LINE3_ADDR  (0x90)
#define LCD_LINE4_ADDR  (0xD0)
#endif 

#define LCD_CLEAR_CMD   (0x01)
#define EOL             ('\0')


#define	DISP_ON         0x00C
#define	DISP_ON_C       0x00E
#define	DISP_ON_B       0x00F
#define	DISP_OFF        0x008
#define	CLR_DISP        0x001
#define	ENTRY_INC       0x006
#define	ENTRY_INC_S     0x007
#define	ENTRY_DEC       0x004
#define	ENTRY_DEC_S     0x005
#define	DD_RAM_ADDR     0x080
#define	DD_RAM_UL       0x0C0
    
#define LCD_CURRENT_SENSE_LINE    (LCD_LINE4_ADDR)
#define LCD_CURRENT_SENSE_COLUMN  (12)

#define LCD_SPEED_RPM_COLUMN     (0)    
#define LCD_CLOCK_WISE_COLUMN    (4)    
#define LCD_ANTI_CLOCK_WISE_COLUMN    (8)    
#define LCD_HOLD_TIME_COLUMN    (12)    
#define LCD_DIRECTION_COLUMN    (15)    
    
#if 0   
#define LCD_TEMP_SENSE_COLUMN    (12)

#define LCD_PROG_COUNT_COLUMN_PMODE (14)
    
#define LCD_REF_WATTAGE_COLUMN_PMODE (3)
    
#define LCD_REF_TEMP_COLUMN_PMODE (3)

#define LCD_REF_MOTOR_SPEED_COLUMN_PMODE (8)

#define LCD_MOTOR_SPEED_COLUMN (10)
 
#define LCD_TEMP_OFFSET_COLUMN    (4)    

#define LCD_PROG_NUMBER_COLUMN    (0x0D)
    
#define LCD_REF_TEMP_COLUMN       (9)
//#define LCD_ACTUAL_TEMP_COLUMN    (9)
    
    
#define BLOWER_REF_LCD_COLUMN     (01)

#define HEATER_REF_LCD_COLUMN     (04)    

#define BAG_REF_LCD_COLUMN        (0x08)
    
#define PROG_REF_LCD_COLUMN       (0x0D)

#define BLOWER_LCD_COLUMN         (01)
    
#define HEATER_LCD_COLUMN         (05)
    
#define BAGCOUNT_LCD_COLUMN       (0x08)    
#endif 
    
void delay(unsigned int a);
void lcdInit();
void lcd(char a,char b );
void display(char a[],char add);
void number_2(U8 num);
void number_3(U16 num);
void number_4(U32 num);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

