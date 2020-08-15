/* 
 * Alauddin Ansari
 * 2018-11-20
 * ATtiny85 Watchdog settings
 * External interrupt enabled (https://www.youtube.com/watch?v=0aAwKT0YWJU)
 */

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect)
{
  // Executes before resuming loop function
}

// 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
// 6=1sec, 7=2sec, 8=4sec, 9=8sec
void setup_watchdog(int ii)
{
  byte bb;
  
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);

  MCUSR &= ~(1<<WDRF);
  
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

// set system into the sleep state 
// system wakes up when watchdog is timed out
void system_sleep()
{
  cbi(ADCSRA, ADEN);                   // switch Analog to Digital Converter (ADC) OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System sleeps here

  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA, ADEN);                   // switch Analog to Digital Converter (ADC) ON
}

/*
 * External Interrupt setting
 * 
 * PCIE is mapped for PCINT0 to PCINT5
 * PCMSK is mapped with PCIE
 */
void setup_interrupt(int PCINT_PIN)
{
  sbi(GIMSK, PCIE);
  sbi(PCMSK, PCINT_PIN);
}
