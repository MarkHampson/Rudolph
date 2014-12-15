/* blink.c
    14-Dec-2014

    This project outputs a PWM signal on P1.0 of the
    MSP430G2553 suitable to make a nice glowy LED nose
    for Rudolph the reindeer on a ugly Christmas sweater
*/

//--------------------------------------------------------------------------- 
// Includes
//--------------------------------------------------------------------------- 

#include <msp430.h>
#include "pulseWidthTable.h"

//--------------------------------------------------------------------------- 
// Defines
//--------------------------------------------------------------------------- 

#define PULSE_PERIOD    8192
#define UP              0
#define DOWN            1
#define DELAY           1 
#define MIN_INDEX       10
#define DIST_FROM_MAX   2

//--------------------------------------------------------------------------- 
// Main
//--------------------------------------------------------------------------- 

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    P1DIR |= 0x01;            // P1.0 output
    CCTL0 = CCIE;             // CCR0 interrupt enabled
    CCR0 = PULSE_PERIOD;
    CCTL1 = CCIE;             // CCR1 interrupt enabled
    CCR1 = pulseWidth[0];
    P1OUT = 0x01;

    TACTL = TASSEL_2 + MC_1 + TAIE;  // SMCLK, up mode, timer interrupt 

    __bis_SR_register(LPM0_bits + GIE); // Enter LPM0 w/ interrupt
}

int getNextIndex(void) {
    static int sIndex = 0;      // index into pulseWidth array
    static int periodCount = 0; // track number of PWM periods since last reset
    static const arraySize = sizeof(pulseWidth)/sizeof(pulseWidth[0]);
    static int upDown = UP;     // current state: traversing table up or down

    // Test if the periodCount has expired, possibly change directions,
    // return the index into the table
    if(++periodCount == DELAY) {
        periodCount = 0;    // reset the period count
        if(sIndex == arraySize - DIST_FROM_MAX) upDown = DOWN;
        if(sIndex == MIN_INDEX) upDown = UP;
        switch(upDown) {
            case UP:
                return ++sIndex;
            case DOWN:
                return --sIndex;
        }
    }
    else {
        return sIndex;
    }
}

// Timer hit CCR0 and the count is reset
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A0 (void)
{
    CCR1 = pulseWidth[getNextIndex()];
    P1OUT = 0x01;      // set P1.0 
  
}

// Timer hit CCR1, still counting up to CCR0
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) Timer_A1 (void)
{
    switch(TA0IV)
    {
        case 2:      // CCR1
        {
            P1OUT = 0x00;  //clear P1.0
        }
        break; 

        case 10:     // overflow 
        break;
        default:
            break;
    }
}
