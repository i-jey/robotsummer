Control Code 
=== 

VSCode and PlatformIO
--- 
1. [Download VSCode](https://code.visualstudio.com/download)
2. [Install PlatformIO](https://platformio.org/get-started/ide?install=vscode)

src/
--- 
This is where the main jazz is located. Things have been modularized so far by sequences (i.e bridge deployment sequences, claw sequence, basket sequence). You'll find `.h` and `.cpp` files here (abstracts and implementation respectively). 

All those files may have you feelin' a ways, but have no fear. The header files, `.h` are just templates that declare what functions/variables are in use for a given file (class). Reading through the header files is probably a good way to get an idea of how the code is laid out (i.e the function abstracts). `includes.h` bundles everything together so that we can reference them from the main control loop.

`main.cpp` is where the control loop runs, and where constants are set. The different sequences are called in the loop and the actual reading of sensors/writing to motors occurs within the implementation of each sequence (e.g look at `clawSequence.cpp` and `claw.cpp`). 

I need to hit up Amar to find out if it's better practice to set constants in a separate header file or what, but for now it's a minor point. Right now everything is stateful (i.e )

Files and general description 
--- 
1. `bridge.cpp/bridge.h`: Bridge object, detects edge, raise/lower bridges.
2. `bridgeSequence.cpp/bridgeSequence.h`: State logic for when to deploy bridges
3. `claw.cpp/claw.h`: Claw object, push button trigger, clamp closing/opening, arm raising/lowering. 
4. `clawSequence.cpp/clawSequence.h`: State logic for the process of retrieving an ewok (from detection to droppin' it hot in the basket)
5. `main.cpp`: Constants set, objects created, sequences called in the main control loop.
6. `README.md` that's this thing. If you make changes to the code/logic or if we make a big decision, add in a section with an explanation. 

TODO
---
1. Create (i.e update) the current `control flow diagram`. Understand states of different sequences, when they have to override/interact with one another (e.g when ir detected we need to change claw control state, or when second bridge deploys we need to change motor control state)
2. Add in a `flag` in `bridge.cpp/bridge.h` so we know when to `switch from tape following to edge detection`. After the second edge is detected we need to do some custom movement.
    - Rotate 90 left, drop bridge, move a set distance w/ wheel encoders, rotate 90 left, drive forward w/ edge detection.
3. Write `Gate (IR) sequence` (i.e set flag to keep left claw raised, raise both claws going through gate). **Note**: This means `clawSequence` will probably need to be updated to chillax while the gateSequence is going on. 
4. Need to think about how motor control is going to get passed around, i.e from tape control to edge detection to bridge deployment to resuming. And the again, when second edge is detected, rotation, bride deployed, edge detection initiated. (control flow diagram will help with this)
5. Figure out how to set up the `menu system` (how to navigate, how to set variables) using the OLED and then start up the contorl loop. Lower priority for now, setting up base of architecture first. 
6. Convert types to standard definnition (i.e 8-bit unsigned integer -> uint8_t)

Lastly, thank you for reading. 

![](https://raw.githubusercontent.com/i-jey/robotsummer/master/images/shariar%20ezzat.jpg)