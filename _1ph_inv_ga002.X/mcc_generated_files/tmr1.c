
/**
  TMR1 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.c

  @Summary
    This is the generated source file for the TMR1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for TMR1. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ64GA002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB             :  MPLAB X v5.45
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

#include <stdio.h>
#include "tmr1.h"
#include "tmr2.h"

#define ARRAY_SIZE  100
#define HALF_TREF  (int)(ARRAY_SIZE/2)
#define MAX_DUTY    TMR2_Period16BitGet()

/**
 Section: File specific functions
*/
void (*TMR1_InterruptHandler)(void) = NULL;
void TMR1_CallBack(void);

void _1_ph_cntrl_OL(void);
void _1_ph_cntrl_CL_PEAK(void);
void _1_ph_cntrl_CL_SREF(void);

/**
  Section: Data Type Definitions
*/

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintenance of the hardware instance.

  @Description
    This defines the object required for the maintenance of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    volatile bool           timerElapsed;
    /*Software Counter value*/
    volatile uint8_t        count;

} TMR_OBJ;

static TMR_OBJ tmr1_obj;

/**
  Section: Driver Interface
*/

void TMR1_Initialize (void)
{
    //TMR1 0; 
    TMR1 = 0x00;
    //Period = 0.0002 s; Frequency = 16000000 Hz; PR1 3199; 
    PR1 = 0xC7F;
    //TCKPS 1:1; TON enabled; TSIDL disabled; TCS FOSC/2; TSYNC disabled; TGATE disabled; 
    T1CON = 0x8000;

    if(TMR1_InterruptHandler == NULL)
    {
        TMR1_SetInterruptHandler(&TMR1_CallBack);
    }

    IFS0bits.T1IF = false;
    IEC0bits.T1IE = true;
	
    tmr1_obj.timerElapsed = false;

}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt (  )
{
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin

//    _1_ph_cntrl_OL();
    _1_ph_cntrl_CL_PEAK();
//    _1_ph_cntrl_CL_SREF();

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes
    if(TMR1_InterruptHandler) 
    { 
           TMR1_InterruptHandler(); 
    }

    //***User Area End

    tmr1_obj.count++;
    tmr1_obj.timerElapsed = true;
    IFS0bits.T1IF = false;
}

void _1_ph_cntrl_OL(void){
    extern float sin1_array[ARRAY_SIZE+1];
    extern float sin2_array[ARRAY_SIZE+1];
    extern float ma;
    extern int i;
    
    int pwm1;
    int pwm2;
    
    pwm1 = (int)(MAX_DUTY*(0.5 + ma*sin1_array[i]));
    pwm2 = (int)(MAX_DUTY*(0.5 + ma*sin2_array[i]));
    
    OC1RS = pwm1;
    OC2RS = pwm2;
    
    i++;
    if(i>ARRAY_SIZE){
        i = 0;
    }
    
}

void _1_ph_cntrl_CL_PEAK(void){
    extern float sin1_array[ARRAY_SIZE+1];
    extern float sin2_array[ARRAY_SIZE+1];
    extern float ma;
    extern int i;
    extern int ss_tr;
    
    extern float Vac;
    extern float Vac_ref;
    extern float Vac_hgh;
    extern float V_REF_AC_MAG;
    
    int pwm1;
    int pwm2;
       
//////////////////////////////////////////////////////////////
//    Vac = ((Vref/2**ADC_RES)*RawAdcVal- V_offset)*SENSOR_GAIN_FACTOR;
//    Vref = 5.0
//    ADC_RES = 10
//    RawAdcVal - Range from 0 to 1023
//    V_offset = 2.5
//    SENSOR_GAIN_FACTOR = 230
//    Vac = ((5.0/1023.0)*(float)ADC1BUF0- 2.5)*230.0;
    Vac = 1.124144673*(float)ADC1BUF0 - 575.0; // Calculate AC Voltage
    if(Vac < 0){Vac = -1*Vac;} //Rectify AC Voltage
    if(Vac > Vac_hgh){Vac_hgh = Vac;} //Check if it is higher than Vac_hgh(Vac_pk)

    Vac_ref = V_REF_AC_MAG; //Assigning the Vac_ref
    
    ss_tr++; //Incrementing ss_tr to keep track of time -- 0.5*Tref
    if(ss_tr > HALF_TREF){ //If 0.5*Tref has passed
        Vac = Vac_hgh; //Equate Vac to Vac_hgh(Vac_pk)
        
        // Increase or decrease ma based on Vac and Vac_ref
        if(Vac < Vac_ref){ma = ma + 0.005;}
        else if(Vac > Vac_ref){ma = ma - 0.005;}
        
        //Limiting the ma
        if(ma > 0.95){ma = 0.95;}
        else if(ma < 0.0){ma = 0.0;}
        
        // Reinitialise Vac_hgh(Vac_pk) and ss_tr
        Vac_hgh = 0.0;
        ss_tr = 0;
    }
//////////////////////////////////////////////////////////////
    
    
    // update pwm1 and pwm2 variables
    pwm1 = (int)(MAX_DUTY*(0.5 + ma*sin1_array[i]));
    pwm2 = (int)(MAX_DUTY*(0.5 + ma*sin2_array[i]));
    
    //update the duty cycle for both half bridges
    OC1RS = pwm1;
    OC2RS = pwm2;
    
    
//////////////////////////  
    // keep track of the array element for the spwm
    i++;
    if(i>ARRAY_SIZE){ // restart the array if overflow occurs
        i = 0;
    }
//////////////////////////    
    
}

void _1_ph_cntrl_CL_SREF(void){
    extern float sin1_array[ARRAY_SIZE+1];
    extern float sin2_array[ARRAY_SIZE+1];
    extern float refSINtable[ARRAY_SIZE+1];
    extern float V_REF_AC_MAG;
    extern float ma;
    extern int i;
    
    extern float Vac;
    extern float Vac_ref;
    
    int pwm1;
    int pwm2;
    
    
//////////////////////////////////////////////////////////////
//    Vac = ((Vref/2**ADC_RES)*RawAdcVal- V_offset)*SENSOR_GAIN_FACTOR;
//    Vref = 5.0
//    ADC_RES = 10
//    RawAdcVal - Range from 0 to 1023
//    V_offset = 2.5
//    SENSOR_GAIN_FACTOR = 230
//    Vac = ((5.0/1023.0)*(float)ADC1BUF0- 2.5)*230.0;
    Vac = 1.124144673*(float)ADC1BUF0 - 575.0;
    if(Vac < 0){Vac = -1*Vac;} //Rectify AC Voltage
    
    Vac_ref = V_REF_AC_MAG*refSINtable[i];  //Obtaining the Vac_ref(constantly changes)
    if(Vac_ref < 0){Vac_ref = -1*Vac_ref;}  //Rectify Vac_ref value
    
    // Increase or decrease ma based on Vac and Vac_ref
    if(Vac < Vac_ref){ma = ma + 0.0008;}
    else if(Vac > Vac_ref){ma = ma - 0.0008;}
    
    //Limiting the ma
    if(ma > 0.95){ma = 0.95;}
    else if(ma < 0.0){ma = 0.0;}
//////////////////////////////////////////////////////////////
    
    // update pwm1 and pwm2 variables
    pwm1 = (int)(MAX_DUTY*(0.5 + ma*sin1_array[i]));
    pwm2 = (int)(MAX_DUTY*(0.5 + ma*sin2_array[i]));
    
    //update the duty cycle for both half bridges
    OC1RS = pwm1;
    OC2RS = pwm2;
    
//////////////////////////
    // keep track of the array element for the spwm
    i++;
    if(i>ARRAY_SIZE){  // restart the array if overflow occurs
        i = 0;
    }
//////////////////////////    
    
}


void TMR1_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Period16BitGet( void )
{
    return( PR1 );
}

void TMR1_Counter16BitSet ( uint16_t value )
{
    /* Update the counter values */
    TMR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Counter16BitGet( void )
{
    return( TMR1 );
}


void __attribute__ ((weak)) TMR1_CallBack(void)
{
    // Add your custom callback code here
}

void  TMR1_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.T1IE = false;
    TMR1_InterruptHandler = InterruptHandler; 
    IEC0bits.T1IE = true;
}

void TMR1_Start( void )
{
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;

    /*Enable the interrupt*/
    IEC0bits.T1IE = true;

    /* Start the Timer */
    T1CONbits.TON = 1;
}

void TMR1_Stop( void )
{
    /* Stop the Timer */
    T1CONbits.TON = false;

    /*Disable the interrupt*/
    IEC0bits.T1IE = false;
}

bool TMR1_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr1_obj.timerElapsed;

    if(status == true)
    {
        tmr1_obj.timerElapsed = false;
    }
    return status;
}

int TMR1_SoftwareCounterGet(void)
{
    return tmr1_obj.count;
}

void TMR1_SoftwareCounterClear(void)
{
    tmr1_obj.count = 0; 
}

/**
 End of File
*/
