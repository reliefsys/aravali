/**
  CWG2 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    cwg2.h

  @Summary
    This is the generated header file for the CWG2 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides APIs for driver for CWG2.
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

#ifndef _CWG2_H
#define _CWG2_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>



#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: CWG2 APIs
*/

/**
  @Summary
    Initializes the CWG2

  @Description
    This routine configures the CWG2 specific control registers

  @Preconditions
    None

  @Returns
    None

  @Param
    None

  @Comment
    

  @Example
    <code>
    CWG2_Initialize();
    </code>
*/
void CWG2_Initialize(void);

/**
  @Summary
    This function would write the rising dead band count into CWG2DBR registers

  @Description
    This function can be used to modify rising dead band count of CWG on the fly

    When module is disabled, the CWG2DBR register is loaded immediately when
    CWG2DBR is written. When module is enabled, then software must set the
    CWG2CON0<LD> bit, and the buffer will be loaded at the next falling edge of
    the CWG input signal.

  @Preconditions
    CWG2_Initialize() function should have been called before calling this function

  @Returns
    None

  @Param
    value - 6 bit digital rising dead band count to CWG.

  @Example
    <code>
    uint8_t myDutyValue;

    CWG2_Initialize();

    // do something

    CWG1_LoadRiseDeadbandCount(myDutyValue);

    if(CWG1_IsModuleEnabled())
    { // yes
        CWG1_LoadBufferEnable();
    }
    </code>
*/
void CWG2_LoadRiseDeadbandCount(uint8_t dutyValue);

/**
  @Summary
    This function would write the falling dead band count into CWG2DBF registers

  @Description
    This function can be used to modify falling dead band count of CWG on the fly

    When module is disabled, the CWG2DBF register is loaded immediately when
    CWG2DBF is written. When module is enabled, then software must set the
    CWG2CON0<LD> bit, and the buffer will be loaded at the rising edge of CWG
    input signal after the next falling edge (that is, after the falling-edge update.

  @Preconditions
    CWG2_Initialize() function should have been called before calling this function

  @Returns
    None

  @Param
    value - 6 bit digital falling dead band count to CWG.

  @Example
    <code>
    uint8_t myDutyValue;

    CWG2_Initialize();

    // do something

    CWG1_LoadFallDeadbandCount(myDutyValue);

    if(CWG1_IsModuleEnabled())
    { // yes
        CWG1_LoadBufferEnable();
    }
    </code>
*/
void CWG2_LoadFallDeadbandCount(uint8_t dutyValue);



/**
  @Summary
    This function sets the CWG2CON0<LD> bit

  @Description
    This function enables the CWG2 module to load the buffers on the next
	rising/falling event of the input signal.
	
	The CWG2CON0<LD> bit can only be set after EN = 1 and cannot be set in the same 
	instruction that EN is set.

  @Preconditions
    CWG2_Initialize() function should have been called and the EN bit
	has to be set before calling this function

  @Returns
    None

  @Param
    None

  @Example
    <code>
    Refer CWG2_LoadFallDeadbandCount() example.
    </code>
*/
void CWG2_LoadBufferEnable(void);

/**
  @Summary
    Returns CWG2 module enable status

  @Description
    Returns the status of whether the CWG2 module is enabled or not

  @Preconditions
    None

  @Returns
    Returns CWG2 module enable status

  @Param
    None

  @Example
    <code>
    Refer CWG2_LoadFallDeadbandCount() example.
    </code>
*/
bool CWG2_IsModuleEnabled(void);

/**
  @Summary
    Software generated Shutdown event

  @Description
    This function forces the CWG into Shutdown state.

  @Preconditions
    CWG2_Initialize() function should have been called before calling this function

  @Returns
    None

  @Param
    None

  @Example
    <code>
    CWG2_Initialize();
    
    // do something
    
    CWG2_AutoShutdownEventSet();
    </code>
*/
void CWG2_AutoShutdownEventSet();

/**
  @Summary
    This function makes the CWG to resume its operations from the software
    generated shutdown state.

  @Description
    When auto-restart is disabled, the shutdown state will persist as long as the
    CWG2AS0<SHUTDOWN> bit is set and hence to resume operations, this function should be used.

    However when auto-restart is enabled, the CWG2AS0<SHUTDOWN> bit will be cleared automatically
    and resume operation on the next rising edge event. In that case, there is no
    need to call this function.

  @Preconditions
    CWG2_Initialize() and CWG2_AutoShutdownEventSet() functions should have been called before calling this function

  @Returns
    None

  @Param
    None

  @Example
    <code>
    CWG2_Initialize();
    
    // Do something
    
    CWG2_AutoShutdownEventSet();

    // Do something
    
    CWG2_AutoShutdownEventClear();
    </code>
*/
void CWG2_AutoShutdownEventClear();

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  // _CWG2_H
/**
 End of File
*/