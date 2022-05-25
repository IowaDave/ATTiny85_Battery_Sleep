# ATTiny85_Battery_Sleep
**Blink an LED for two weeks on two AAA batteries**

Arduino development boards provide a popular introduction to working with microcontrollers. However, one sometimes hears people grumble about short battery life. 

There is a way to fix that problem, gaining remarkable increase in battery life.

**Here's what eats batteries**:<br>
The boards surround their central microcontroller with other hardware. For example, consider the familiar Uno. It is built around a "low-power" ATmega328P controller. But wait. Other devices inhabit the Arduino board, including:

* another microcontroller in the role of a USB-to-TTL programmer,
* an external crystal oscillator,
* a voltage regulator,
* and several LEDs, including one that burns continuously.

All of those additional devices consume current. Moreover, most newcomers stop short of taking full advantage of power-saving features available in the ATmega chip, such as its various "sleep" modes. Usually, people just let everything run full-bore all the time. 

**A Solution in Two Parts**:
Part 1 is to separate the microcontroller from the Arduino development board and run the controller by itself. Presto! all those power-devouring peripherals are gone. In this project I demonstrate using an ATtiny85 controller, a smaller cousin of the ATmega328P.

Part 2 involves minimizing power consumption of the controller itself. How long can two AAA batteries keep a microcontroller running? The answer is: a surprisingly long time.

This project demonstrates the deepest sleep level, "power down" sleep, combined with using the Watchdog Timer Interrupt to wake the chip at approximately one-second intervals. An LED is toggled at each interrupt to visualize the operation. The controller uses about 1/800th of the power while sleeping, compared to the amount it uses while active. Even better, in this demonstration it sleeps more than 99.99 percent of the time.

The average power consumption of the chip was reduced to 10.1 *micro*Amps. The LED drew about 9 *milli*Amps when it was "on", more than 891 times more power than the chip used, and the LED was active 50% of the time. Even so, a single pair of AAA batteries kept the demonstration running more than 2 weeks, with ample power to spare when I unplugged the batteries.

## Equipment List
* ATtiny85 microcontroller
* Breadboard
* LED and 220-ohm resistor
* 2 AAA batteries
* Battery case for the batteries
* ISP programmer, e.g., an [Arduino as ISP](https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP)
* Jumper wires

## Programming the ATtiny85
### Connections
Refer to the 8-pin PDIP package in the following diagram, extracted from the ATtiny25/45/85 data sheet.

![ATtiny25/45/85 Package Diagram](https://github.com/IowaDave/ATTiny85_Battery_Sleep/blob/main/Tiny85_Package_Diagram.pdf)

Using an Uno as the ISP programmer, connect the following:
|UNO pin|ISP Function|Tiny85 pin|
|---|---|---|
|13|SCK|PB2|
|12|CIPO|PB1|
|11|COPI|PB0|
|10|Control|PB5|
|5V|Power|VCC|
|GND|Ground|GND|
### Procedure
#### Configure the Arduino IDE
The IDE does not come with a set of core code built-in for the Tiny AVR chips. Add the neeeded cores by pasting the following URL in the "Additional Boards Manager URLs" field of the Arduino IDE Preferences dialog. Note that the field can contain more than one URL; merely separate the URLs with commas.

[https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json](https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json)

Then, in the Boards Manager dialog, scroll down to "attiny by David A. Mellis" and click the Install button.

In the Tools menu, make the following selections:
* Programmer: "Arduino as ISP" *Note: choose the programmer you actually are using*
* Port: *The port used by your programmer*
* Board: ATtiny Microcontrollers > "ATtiny25/45/85"
* Processor: "ATtiny85"
* Clock: "Internal 8 MHz"

#### Configure the Tiny85
A new Tiny85 comes pre-set to use its internal clock at a frequency of 1 MHz. The chip will feel more "like an Arduino" if you change its clock speed to 8 MHz; this change might even be needed for it to accept firmware uploads from the IDE. The change is easy to make.

Having chosen the "Internal 8 MHz" clock speed in the Tools menu, as listed above, run the Tools > Burn Bootloader command. There is no actual bootloader code for a Tiny chip. Instead, this command simply writes certain values to the fuses. It won't hurt your Tiny. After this change, the Tiny85's clock will run at 8 MHz.

You only need to configure the Tiny this way once.

#### Upload the Code
The code for this project is provided in this repository as an Arduino-style folder named "ATtiny85_PowerDown_Watchdog". My respect for readers assumes they already know how to bring it into the IDE.

The IDE actually treats the ArduinoISP as if it were an FTDI programmer for code uploads into a Tiny85. Which makes it possible to use the regular upload method. Just click the right-pointing arrow, press Ctrl-U, or run the Tools > Upload command.

## Power Up!
After code upload is complete: 
* Remove the jumper wires between the ISP programmer and the Tiny85. 
* Connect the anode of an LED to pin PB3.
* Connect the cathode of the LED to a common ground bus through a 220-Ohm resistor.

Finally, attach the batteries:
* Positive 3-volts to the VCC pin of the Tiny85
* Negative to the common ground bus.

If all goes well, the LED will blink at approximately one-second intervals. It is interesting to note, regarding the voltage data below, that the LED would have been "on" and consuming current approximately half of the time.

## Results
Voltage of the batteries was measured at intervals of elapsed hours:

|Hours|Voltage|
|---|---|
|Fresh|3.192|
| 12|3.046|
| 24|2.986|
| 34|2.960|
| 49|2.932|
| 60|2.912|
| 84|2.886|
| 94|2.878|
|119|2.870|
|133|2.854|
|143|2.840|
|158|2.820|
|167|2.812|
|178|2.796|
|192|2.784|
|204|2.776|
|213|2.768|
|226|2.756|
|Skip a day|
|272|2.716|
|284|2.702|
|2.7 volts! See comment below|
|319|2.684|
|365|2.664|

I stopped taking measurements after 365 hours. Two weeks is 336 hours, which means the apparatus ran continuously for more than 2 weeks. Two observations can be made:

1. Voltage decay had fallen to a rate below 0.0005 volt/hour, compared to a rate of more than 0.002 volt/hour during the second 24-hour period. I believe much this slowing may be due to reduction in current flow through the LED as the voltage decreased. Battery life in hours also increases somewhat as current draw decreases. 
2. Familiar AVRs such as the ATmega328P of the Uno have a "safe" minimum voltage threshhold of 2.7 volts. However, according to the datasheet, safe operation of an ATtiny85 at 8 MHz can be maintained down to 2.40 volts. I conjecture that, having 2.664 - 2.400 = 0.264 volts remaining, holding the rate of decrease to a constant 0.0005 volt/hour, the apparatus might have continued to run a further 0.264/0.0005 = 528 hours more or less. Alas, I did not follow it that far.

I have no doubt that the LED accounts for nearly all of the current consumed during the time described above. The Tiny85 would have taken very little. How much?

According to the Electrical Characteristics table in the data sheet, a Tiny draws 10 uA of current in power-down sleep mode while the Watchdog Timer is running. By contrast, it can pull up to 8 mA while active. What proportion of the time is spent sleeping, and how much awake?

A study of the Assembly code listing produced during compilation by the Arduino IDE shows a total of just a few dozen instructions in the ISR and the loop() process. Most AVR instructions execute in one clock cycle. To be generous, imagine 100 clock cycles of activity during each second. 100/8,000,000 = 0.0000125 second of active time per second. We have:<br>
<pre>
        0.0000125 x 8000 uA = 0.100000 uA active
      + 0.9999875 x 10 uA   = 9.999875 uA sleeping
                             ---------
  Total:                     10.099875 uA </pre>

How to interpret an average current of 10.1 uA? A steady average rate of 10 microAmps seems very small compared to the 1,000 milliAmpHour ( 1,000,000 microAmpHours?) of the two batteries when fresh. I don't feel confident estimating a battery life based on just those two numbers. I do feel safe to say that it would extend a great deal longer than two weeks.
