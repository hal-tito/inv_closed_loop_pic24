/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ64GA002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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
#include "mcc_generated_files/system.h"
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846

#define ARRAY_SIZE  100

/*
                         Main application
 */

//Variables Initialization
float sin1_array[ARRAY_SIZE+1] = {0.0};     // Half bridge 1 reference sine wave initialization
float sin2_array[ARRAY_SIZE+1] = {0.0};     // Half bridge 2 reference sine wave initialization
float ma = 0.48; // Amplitude modulation index initialization

float Vac = 0.0; //Sensed AC Voltage
float Vac_ref = 0.0; //Reference AC Voltage
float Vac_hgh = 0.0; //Peak AC Voltage

int i = 0; //Element index variable
int ss_tr = 0; //Half wave tracking variable

float refSINtable[ARRAY_SIZE+1] = {0.0};
float V_REF_AC_MAG = 210; //Vrms*sqrt(2)

int main(void)
{
    
    for(i=0; i<=ARRAY_SIZE; i++){
        sin1_array[i] = 0.5*sinf(((float)i/(float)ARRAY_SIZE)*2*M_PI); // Half Bridge 1 reference sine wave
        sin2_array[i] = 0.5*sinf(((float)i/(float)ARRAY_SIZE)*2*M_PI + M_PI); // Half Bridge 2 reference sine wave
        refSINtable[i] = sinf(((float)i/(float)ARRAY_SIZE)*2*M_PI); // Reference sine wave for closed loop control
    }
    i = 0; // Reset index after table generation
    
    // initialize the device
    SYSTEM_Initialize();
    
    while (1)
    {
        // Add your application code
    }

    return 1;
}
/**
 End of File
*/

