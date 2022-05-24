/* ATtiny85 PowerDown Sleep Watchdog Timer Wake
 *
 * Blink an LED at 1-second intervals 
 * as regulated by the Watchdog Timer
 * of an AVR ATtiny85 microcontroller.
 * 
 * The code directly accesses registers 
 * by name in the Tiny85. See the 
 * [ATtiny85 datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf)
 * for more information.
 *
 * Copyright (C) 2022  David G Sparks

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Contact the author by opening an Issue in this Repository
 *
 */


void setup() {
  // Turn off embeded peripherals we don't need
  // disable ADC
  ADCSRA &= ~(1 << ADEN);
  // disable bandgap voltage
  ACSR &= ~(1 << ACBG);
  // power-down the ADC
  PRR &= ~(1 << PRADC);
  // set all pins to input, which I think is the default anyway
  DDRB = 0; 
  // connect an LED to pin 3 (i.e., PB3)
  // set pullup (HIGH) on the unconnected I/O pins
  // while also setting pin 3 LOW
  PORTB = 0b00110111;
  // set pin 3 to output after setting it to LOW
  DDRB |= (1 << DDB3);
  // and now we can toggle pin 3,
  // changing it to HIGH from LOW in this instance,
  // which lights up the LED.
  PINB |= (1 << PINB3);
  // Disable the watchdog timer's reset mode,
  // because using the watchdog interrupt instead.
  // The following sequence is time-sensitive
  // Step #1, write logic 1 to WDCE and WDE in a single instruction
  WDTCR |= ( (1 << WDCE) | (1 << WDE) );
  // step #2, write WDTCR as follows
  WDTCR = (1 << WDIF) // clear WD interrupt flag, why not?
        | (0 << WDIE) // don't set WD interrupt here
        // prescaler bits WDP[3:0] to be 0110 for 1-second interval
        | (0 << WDP3) // prescaler bit 3 = 0
        | (0 << WDCE) // WDCE needs to be clear for this step
        | (0 << WDE) // disable the Watchdog's reset behavior
        | (1 << WDP2) // prescaler bit 2 = 1
        | (1 << WDP1) // prescaler bit 1 = 1
        | (0 << WDP0); // prescaler bit 0 = 0
  //set the sleep mode bits SM[1:0] to 10 = power-down
  MCUCR =  (1 << SM1) | (MCUCR & ~(1 << SM0));
}

void loop() {
  // enable the watchdog interrupt
  WDTCR |= (1 << WDIE);
  // Putting the device to sleep involves two steps
  // Step 1: set the Sleep Enable bit to 1
  MCUCR |= (1 << SE);
  // Step 2: execute the SLEEP instruction
  asm volatile (
    "sleep \r\n"
    :
    :
  );
}

// When the Watchdog Timer fires its interrupt
// the CPU jumps immediately
// to the WDT_vect interrupt service routine

ISR (WDT_vect) {
  
  // clear the Sleep Enable bit immediately after waking up
  MCUCR &= ~(1 << SE);

  // toggle the LED on pin 3
  PINB |= (1 << PINB3);
}

// After the WDT_vect ISR concludes, 
// the CPU resumes execution at the
// instruction immediately following 
// the Sleep instruction. In this case,
// that is the top of the loop() process.



