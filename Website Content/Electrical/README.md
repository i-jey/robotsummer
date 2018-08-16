Electrical Components, Debugging, Diagrams etc.
=== 

## Motor Drivers

PL2 used H-bridges to allow motors to be driven at high voltages and currents while being controlled through low-voltage signals from the Bluepill mirco-controller. Our h-bridges consist of electrical switches (N-Type MOSFETS) controlled by a high-side gate driver IC chip which recieved PWM outputs from the Bluepill. Opposite pairs of MOSFETs were activated so current could flow through the motor in two directions allowing both forward and backwards travel. Each H-bridge was decoupled through capacitor along input and output voltage lines so that noise propagation was kept to a minimum to not influence other componants in the electrical system.

## IR detection circuits

One of the obstacles of the course was decoding an infra-red signal beacon. This beacon was placed just before the archway, and it's signal would switch randomly between flashing at a frequency of 1kHz and 10kHz. If the robot passed throught the archway when the signal was at 1kHz a platform beyond the archway would start rotating, breaking the tape the robot used to navigate the course. PL2 had two IR detection circuits were used to differentiate the beacons 1kHz and 10kHz respectivly. Both circuits where identical, with the exception of the filter explained below

This analog circuit was a series of singal manipulators to decode the flashing of the beacon into a sinal understood by the Bluepill mirco-controller. The componants of the circuit are as followed:
1. DC-Block
      The DC-Block was used to eliminated the stead 60Hz infra-red emitted from the standard household lights as a part of the competition enviroment.
2. Amplify
      At a distance that was relevant to robot manuverability the singal would come across quite weak (int the milli-volts range) the amplificating was used to strengthen the incoming signal
3. Filter
      Since PL2 was required to differentiate 1kHz and 10kHz, the respective circuits were built to ignore the opposite frequency. 
4. Analog to digital conversion and amplification
      The final convertion is required to turn the detection of the appropriate signal into a digital high that the mirco-controller may interpret. The amplification is required in the event the non-fluctuating voltage is not large enough for the mirco-controller to read a digital high.
      
### Inverter
As a part of the analog circuits a switching-inverter was used to supply VCC- to the operational amplifiers.

## Attempted: Encoders

Orginially it was planned to include encoders in the development of PL2 to aid in the navigation of the course. In retrospect we should have started the development of the Encoder interpretation much sooner to ensure reliable usage. The primary difficulty was recieving a steady, noise-free pulse signal from the encoders. As an encoder rotates two switches are turned on and off. The mirco-controller may interpret distance through the number of times the switches are turned on, and direction through the high/low phase shift between the two switches. Due to mechanical inaccuracies the signal is very noise and, initially, the mirco-controller would read high when it should not have, thus, misinterpretation the signal. 

To fix this we built a RC filter circuit and Schmitt-trigger to filter out high frequency noise, and to employ hysterisis to ensure a clean pulse signal could be read by the mirco-controller. As mentioned before, this development occured too late, for we had already started to used hardcoded motor-on times for precise movments. 

## Attempted: IR Signal Direct Read

Due to the extremely fast clock cycle of the Bluepill mirco-controller (72MHz) it is very possible to directly interpret signals at a frequency in the kHz range as required for the compeition. The circuit was simply an amplifier and comparator to ensure the signal was properly read, and the sine wave was converted into a pulse. This development also occurred too late in the 6 week timeline and while it did work it was a back-up plan in the event the analog circuits failed for some unknown reason.

