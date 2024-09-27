#include <Arduino.h>


#define PWM_PIN PB1
#define ADC_PIN 1 // PB2

void setup() {
  // Set the PWM pin as output
  DDRB |= _BV(PWM_PIN);

  // Configure Timer/Counter0
  TCCR0A = 0x00;
  TCCR0A |= _BV(WGM00) | _BV(COM0A0)| _BV(COM0B1);          

  TCCR0B = 0x00;
  TCCR0B |= _BV(CS02); // Set the PWM frequency to 122 Hz

  TCNT0 = 0;
  OCR0B = 0;

  // ADC configuration
  ADMUX = _BV(MUX0);          // Select ADC input channel 1 (PB2)
  ADCSRA = _BV(ADEN) |        // Enable ADC
           _BV(ADPS2) |       // Set prescaler to 64 (125kHz with 8MHz clock)
           _BV(ADPS1);
}

void loop() {
  // Start ADC conversion
  ADCSRA |= _BV(ADSC);

  // Wait for conversion to complete
  while (ADCSRA & _BV(ADSC));

  // Read ADC value (10-bit value)
  uint16_t adcValue = ADC;

  // Scale ADC value to match OCR1A range (0 to OCR1C)
  uint8_t pwmValue = adcValue >> 2;

  OCR0B = pwmValue;
}