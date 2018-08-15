Electrical Components, Debugging, Diagrams etc.
=== 

## H-bridges

PL2 used a specialty circuit known as an H-bridge to drive its motors at high voltages and currents while maintaining smooth and consistent control. The H-bridge circuits (one per motor) were constructed from multiple MOSFETs, capacitors, and a gate driver IC chip.

## IR detection circuits

Part of the competition course required passing through a gate only when an infrared beacon was emitting a 10kHz frequency (as opposed to 1kHz). To detect these frequencies, two active-filter circuits were constructed using multiple series Op-Amps and RC circuits. The circuits followed a DC-block->amplify->filter->amplify sequence to ensure that each circuit (the 1kHz and 10kHz) output a high DC voltage when the front detector picked up the circuit's respective frequency.
