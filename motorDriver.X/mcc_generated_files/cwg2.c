/**
  CWG2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    cwg2.c

  @Summary
    This is the generated driver implementation file for the CWG2 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides implementations for driver APIs for CWG2.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15.3
        Device            :  PIC16F18877
        Driver Version    :  1.0
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "cwg2.h"

/**
  Section: CWG2 APIs
*/

void CWG2_Initialize(void)
{
    // Set the CWG2 to the options selected in MPLAB(c) Code Configurator

    // Writing to CWGxCON0, CWGxCON1, CWGxCON2, CWGxDBR & CWGxDBF registers
	// CWG2POLA inverted; CWG2POLC non inverted; CWG2POLB inverted; CWG2POLD non inverted; 
	CWG2CON1 = 0x03;
	// CWG2DBR 9 to 10; 
	CWG2DBR = 0x09;
	// CWG2DBF 9 to 10; 
	CWG2DBF = 0x09;
	// CWG2SHUTDOWN No Auto-shutdown; CWG2REN disabled; CWG2LSBD inactive; CWG2LSAC inactive; 
	CWG2AS0 = 0x00;
	// AS1E disabled; AS0E disabled; AS3E disabled; AS2E disabled; AS5E disabled; AS4E disabled; AS6E disabled; 
	CWG2AS1 = 0x00;
	// CWG2STRD disabled; CWG2STRB disabled; CWG2STRC enabled; CWG2STRA disabled; CWG2OVRD low; CWG2OVRA low; CWG2OVRB low; CWG2OVRC low; 
	CWG2STR = 0x04;
	// CWG2CS HFINTOSC; 
	CWG2CLKCON = 0x01;
	// IS CCP4_OUT; 
	CWG2ISM = 0x04;
    
    // CWG2LD Buffer_not_loaded; CWG2EN enabled; CWG2MODE Reverse Full bridge mode; 
    CWG2CON0 = 0x83;
}

void CWG2_LoadRiseDeadbandCount(uint8_t dutyValue)
{
    // Writing 6 bits of rising dead band count into CWGxDBR register
    CWG2DBR = dutyValue;
}

void CWG2_LoadFallDeadbandCount(uint8_t dutyValue)
{
    // Writing 6 bits of rising dead band count into CWGxDBF register
    CWG2DBF = dutyValue;
}

void CWG2_LoadBufferEnable(void)
{
	//It sets the CWG2CON0<LD> bit
    CWG2CON0bits.CWG2LD = 1;
}

bool CWG2_IsModuleEnabled()
{
	//returns whether the module is enabled or not
    return (CWG2CON0bits.CWG2EN);
}

void CWG2_AutoShutdownEventSet()
{
    // Setting the SHUTDOWN bit of CWGxAS0 register
    CWG2AS0bits.CWG2SHUTDOWN = 1;
}

void CWG2_AutoShutdownEventClear()
{
    // Clearing the SHUTDOWN bit of CWGxAS0 register
    CWG2AS0bits.CWG2SHUTDOWN = 0;
}


/**
 End of File
*/