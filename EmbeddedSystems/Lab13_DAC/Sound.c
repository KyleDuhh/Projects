// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// This routine calls the 4-bit DAC

#include "Sound.h"
#include "DAC.h"
#include "..//tm4c123gh6pm.h"
const unsigned short SineWave[32] = {
8,9,11,12,13,14,14,15,15,15,14,
14,13,12,11,9,8,7,5,4,3,2,
2,1,1,1,2,2,3,4,5,7
};
unsigned char Index;           // Index varies from 0 to 31
// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void){
  DAC_Init();
	Index = 0;
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF; // reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;
// priority 1
  NVIC_ST_CTRL_R = 0x0007; // enable,core clock, and interrupts
}
// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long period){
// this routine sets the RELOAD and starts SysTick
	NVIC_ST_CTRL_R = 0x0007;
	NVIC_ST_RELOAD_R = period-1;
}
// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
 // this routine stops the sound output
	NVIC_ST_RELOAD_R = 0;
	NVIC_ST_CTRL_R = 0x0000;
	DAC_Out(0x00);
}
// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
		Index = (Index+1)&0x1F; 
		DAC_Out(SineWave[Index]);    // output one value each interrupt
}
