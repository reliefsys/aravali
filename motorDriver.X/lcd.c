#include "lcd.h"
#include "essential.h"

/*
 * LCD Init display function
 */
void lcdInit()
{
    lcd(0x02,LCD_CMD);
    lcd(0x28,LCD_CMD);
    lcd(LCD_CLEAR_CMD,LCD_CMD);
    lcd(0x0c,LCD_CMD);
    //delay(100);
    lcd(LCD_LINE1_ADDR,LCD_CMD);
}

/*
 * LCD display function
 */
void lcd(char a,char b )
{
    LCD_RS = b;
    LCD_DATA = LCD_DATA & 0x0f;
    LCD_DATA = LCD_DATA | (a & 0xf0);
    LCD_E = 1;
    __delay_us(LCD_DELAY);
    //delay(LCD_DELAY);
    
 //   __delay_ms(5);
    LCD_E = 0;
    //delay(LCD_DELAY);
    __delay_us(LCD_DELAY);
    LCD_DATA = LCD_DATA & 0x0f;
    LCD_DATA = LCD_DATA | ((a << 4)&0xf0);
    LCD_E = 1;
    __delay_us(LCD_DELAY);
    //delay(LCD_DELAY);
    //__delay_ms(5);
    LCD_E = 0;
    //delay(LCD_DELAY);
    __delay_us(LCD_DELAY);
    //__delay_ms(5);
}

/*
 * LCD string display function
 */
void display(char a[],char add)
{
    int i = 0;
    lcd(add,LCD_CMD);
    while (a[i] != EOL) {
        lcd(a[i],LCD_CMD_DATA);
        i++;
    }
}

/*
 * LCD 2 digit number display
*/
void number_2(U8 num)
{
    if(num > 0x39 && num < 0x030)
        return;
    
    lcd(ASCII+num/OFFSET_10,LCD_CMD_DATA);
    lcd(ASCII+num%OFFSET_10,LCD_CMD_DATA);
}
/*
 * LCD 3 digit number display
*/
void number_3(U16 num)
{
	U16 temp;
	U16 a,b;    
    U16 num1 = 0x030;
    U16 num2 = 0x030; 
    U16 num3 = 0x030;
	
    temp = num;

    a=num%OFFSET_10;
    num/=OFFSET_10;
    b=num%OFFSET_10;
    num/=OFFSET_10;

	if(temp > 99)
	{
		num1 = ASCII+num;
		num2 = ASCII+b;
		num3 = ASCII+a;

	}
	else if(temp > 9)
	{
		//num1 = ' ';//' '//ASCII+num;
        
		num2 = ASCII+b;
		num3 = ASCII+a;
	}
	else
	{

		//num1 = ' ';
		//num2 = ' ';
		num3 = ASCII+a; 
	}
	lcd(num1,LCD_CMD_DATA);
    lcd(num2,LCD_CMD_DATA);
    lcd(num3,LCD_CMD_DATA);

}

/*
 * LCD 4 digit number display
*/
void number_4(U32 num)
{
    char arr[10] = {"0123456789"};
	U16 temp, unit;
	U16 a,b,c; 
    U32 ten, hun,thou;
    
    U32 num1 = 0x030;
    U32 num2 = 0x030; 
    U32 num3 = 0x030;
    U32 num4 = 0x030;
    
    unit = num%10;
    ten = (num/10)%10;
    hun = (num/100)%10;
    thou = (num/1000);
    
    if(num > 999)
        num1 = arr[thou];
    if(num > 99)
        num2 = arr[hun];
    if(num > 9)
        num3 = arr[ten];
    
    num4 = arr[unit];

   // lcd((LCD_LINE3_ADDR+LCD_TEMP_SENSE_COLUMN), LCD_CMD);
	lcd(num1,LCD_CMD_DATA);
  //  lcd((LCD_LINE3_ADDR+LCD_TEMP_SENSE_COLUMN+1), LCD_CMD);
    lcd(num2,LCD_CMD_DATA);
  //  lcd((LCD_LINE3_ADDR+LCD_TEMP_SENSE_COLUMN+2), LCD_CMD);
    lcd(num3,LCD_CMD_DATA);
//    lcd((LCD_LINE3_ADDR+LCD_TEMP_SENSE_COLUMN+3), LCD_CMD);
    lcd(num4,LCD_CMD_DATA);
}

/*
 * LCD 5 digit number display
*/
void number_5(U32 num)
{
    char arr[10] = {"0123456789"};
	U16 temp, unit;
	U16 a,b,c; 
    U32 ten, hun, thou, tenThou;
    
    U32 num1 = 0x030;
    U32 num2 = 0x030; 
    U32 num3 = 0x030;
    U32 num4 = 0x030;
    U32 num5 = 0x030;    

    unit = num%10;
    ten = (num/10)%10;
    hun = (num/100)%10;
    thou = (num/1000) %10;

    tenThou = (num/10000)%10;
    
    if(num > 9999)
        num1 = arr[tenThou];
    if(num > 999)
        num2 = arr[thou];
    if(num > 99)
        num3 = arr[hun];
    if(num > 9)
        num4 = arr[ten];
    
    num5 = arr[unit];

   // lcd((LCD_LINE3_ADDR+LCD_TEMP_SENSE_COLUMN), LCD_CMD);
	lcd(num1,LCD_CMD_DATA);
  //  lcd((LCD_LINE3_ADDR+LCD_TEMP_SENSE_COLUMN+1), LCD_CMD);
    lcd(num2,LCD_CMD_DATA);
  //  lcd((LCD_LINE3_ADDR+LCD_TEMP_SENSE_COLUMN+2), LCD_CMD);
    lcd(num3,LCD_CMD_DATA);
//    lcd((LCD_LINE3_ADDR+LCD_TEMP_SENSE_COLUMN+3), LCD_CMD);

    lcd(num4,LCD_CMD_DATA);

    lcd(num5,LCD_CMD_DATA);
}
