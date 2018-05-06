/*
 *  Reading humidity sensors using the ADC.
 *
 *     Note: This code is a modification of the one created by Ant Scranney
 *    from Coder-Tronics published in the following article:
 *      http://coder-tronics.com/msp430-adc-tutorial/
 *   
 *    I simply changed some variables and registers to suit my needs
 *    here it is used to take lectures from two humidity sensors YL-69
 *    connected on pins P1.1 and P1.2.
 *
 *    This code was tested on an MSP430G2553 launchpad from TI.
 *
 *                                             Author: Brandon Ceja.
*/

#include <msp430.h>

// Global Variables
unsigned int adc[2] = {0};	// This will hold the humidity sensors values
unsigned int sensorHum1 = 0;
unsigned int sensorHum2 = 0;

//Function Prototypes
void Setup_HW(void);		// Setup watchdog timer, clockc, ADC ports
void Read_Acc(void);	// This function reads the ADC and stores the sensor values

int main(void)
{
    Setup_HW();
    while (1)
    {
	Read_Acc();	// This function reads the ADC and stores the sensors values
    }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

void Setup_HW(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    ADC10CTL1 = INCH_1 + CONSEQ_1;            // A0/A1, single sequence
    ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
    ADC10DTC1 = 0x02;                         // 2 conversions
    ADC10AE0 |= 0x03;                         // Disable digital I/O on P1.0 to P1.2
}

void Read_Acc(void)
{
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    ADC10SA = (unsigned int)adc;	    // Copies data in ADC10SA to unsigned int adc array
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

    sensorHum1 = adc[0];		    // adc array 0 copied to the variable sensorHum1
    sensorHum2 = adc[1];	            // adc array 1 copied to the variable sensorHum2
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
}
