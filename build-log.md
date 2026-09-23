# RC Car Build Log

## Day 1 - September 23, 2026

### What I did
- Set up Arduino Uno with PlatformIO
- Built and programmed an LED circuit
- Connected a pushbutton module
- Programmed the button to control the LED

### What I learned
- Breadboard connections and common ground
- LED polarity and current direction
- Arduino digital inputs and outputs
- HIGH and LOW logic
- Active-low button signals
- Basic Arduino C++ structure

### Problems / Debugging
- LED initially didn't work because its polarity was reversed
- Button logic was inverted because the module is active-low
- Multiple `.cpp` files in PlatformIO caused duplicate `setup()` and `loop()` definitions

### Next
- Learn analog input and PWM
- Define RC car architecture
- Create project proposal
- Choose and order components