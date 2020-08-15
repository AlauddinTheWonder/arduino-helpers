#include <util/delay.h>

// https://wp.josh.com/2014/11/06/battery-fuel-guage-with-zero-parts-and-zero-pins-on-avr/
// Returns the current Vcc voltage as a fixed point number with 1 implied decimal places, i.e.
// 50 = 5 volts, 25 = 2.5 volts,  19 = 1.9 volts
//
// On each reading we: enable the ADC, take the measurement, and then disable the ADC for power savings.
// This takes >1ms becuase the internal reference voltage must stabilize each time the ADC is enabled.
// For faster readings, you could initialize once, and then take multiple fast readings, just make sure to
// disable the ADC before going to sleep so you don't waste power.

uint8_t readVcc(void) {

  // Select ADC inputs
  // bit    76543210
  // REFS = 00       = Vcc used as Vref
  // MUX  =   100001 = Single ended, 1.1V (Internal Ref) as Vin

  ADMUX = 0b00100001;

  /*
    By default, the successive approximation circuitry requires an input clock frequency between 50
    kHz and 200 kHz to get maximum resolution.
  */

  // Enable ADC, set prescaller to /8 which will give a ADC clock of 1mHz/8 = 125kHz

  ADCSRA = _BV(ADEN) | _BV(ADPS1) | _BV(ADPS0);

  /*
    After switching to internal voltage reference the ADC requires a settling time of 1ms before
    measurements are stable. Conversions starting before this may not be reliable. The ADC must
    be enabled during the settling time.
  */

  _delay_ms(1);

  /*
    The first conversion after switching voltage source may be inaccurate, and the user is advised to discard this result.
  */

  ADCSRA |= _BV(ADSC);        // Start a conversion

  while ( ADCSRA & _BV( ADSC) );   // Wait for 1st conversion to be ready... and ignore the result

  /*
    After the conversion is complete (ADIF is high), the conversion result can be found in the ADC
    Result Registers (ADCL, ADCH).

    When an ADC conversion is complete, the result is found in these two registers.
    When ADCL is read, the ADC Data Register is not updated until ADCH is read.
  */

  // Note we could have used ADLAR left adjust mode and then only needed to read a single byte here

  uint8_t low  = ADCL;
  uint8_t high = ADCH;

  uint16_t adc = (high << 8) | low;   // 0<= result <=1023

  // Compute a fixed point with 1 decimal place (i.e. 5v= 50)
  //
  // Vcc   =  (1.1v * 1024) / ADC
  // Vcc10 = ((1.1v * 1024) / ADC ) * 10        ->convert to 1 decimal fixed point
  // Vcc10 = ((11   * 1024) / ADC )       ->simplify to all 16-bit integer math

  uint8_t vccx10 = (uint8_t) ( (11 * 1024) / adc);

  /*
    Note that the ADC will not automatically be turned off when entering other sleep modes than Idle
    mode and ADC Noise Reduction mode. The user is advised to write zero to ADEN before entering such
    sleep modes to avoid excessive power consumption.
  */

  ADCSRA &= ~_BV( ADEN );     // Disable ADC to save power

  return ( vccx10 );

}


float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//read internal voltage
long readVcc()
{
  long result;
  
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  
  delay(2); // Wait for Vref to settle
  
  ADCSRA |= _BV(ADSC); // Convert
  
  while (bit_is_set(ADCSRA, ADSC));
  
  result = ADCL;
  result |= ADCH << 8;
  
  result = 1126400L / result; // Back-calculate AVcc in mV
  
  return result;
}