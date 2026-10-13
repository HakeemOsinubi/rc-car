
# RC Car Build Log

## Day 1 - September 23, 2026

### What I Did
- Set up my Arduino Uno using PlatformIO in VS Code.
- Built a basic LED circuit and wrote some C++ code to control it.
- Connected a pushbutton module and programmed it to control the LED.
- Experimented with different blinking patterns.

### What I Learned
- Got more comfortable with breadboard connections, common ground, and LED polarity.
- Practiced using `pinMode()`, `digitalWrite()`, and `digitalRead()`.
- Learned how HIGH and LOW signals work.
- Found out that some button modules use active-low logic, meaning pressing the button gives a LOW signal instead of HIGH.
- Got more familiar with the structure of Arduino C++ programs.

### Problems / Debugging
- My LED initially wasn't working because I had the polarity reversed.
- The button was doing the opposite of what I expected because I didn't realize it was active-low.
- I also ran into errors because I had multiple `.cpp` files defining `setup()` and `loop()` in the same PlatformIO project.

### Next
- Learn more about analog inputs and PWM.
- Figure out the architecture of my RC car.
- Start choosing components and putting together a BOM.

---

## Day 2 - September 25, 2026

### Main Goal
Figure out exactly how I want this RC car to work and what components I'll need before spending money on anything.

### What I Did
- Finalized what I want the RC car to do:
  - Move forward and backward.
  - Turn left and right using differential steering.
  - Support variable motor speeds.
  - Use a custom wireless remote.
  - Detect obstacles with an ultrasonic sensor.
  - Automatically stop to prevent collisions.
  - Have a custom 3D-printed chassis and remote enclosure.
  - Eventually use a custom PCB.

- Decided to use two independently controlled DC motors instead of a steering servo.
- Selected two 6V, 205 RPM 37D gearmotors with 80 mm wheels and built-in magnetic encoders.
- Chose the Cytron MDD3A dual-channel motor driver.
- Planned to power the motors using a 6V NiMH battery.
- Selected a separate 5V step-up/step-down regulator for the ESP32.
- Chose a 3.3V-compatible HC-SR04 ultrasonic sensor for obstacle detection.
- Decided to use two ESP32 boards, one for the car and one for the remote, communicating wirelessly.
- Started putting together my BOM and checking whether all the components were compatible.
- Decided to wait until the physical components arrive before designing the chassis in CAD.
- Sebastian offered to print my STL files, which is great because the lab printer isn't exactly amazing.

### What I Learned
- Motor stall current is really important when selecting a motor driver. I can't just look at the normal operating current and call it a day.
- Battery capacity (mAh) and the amount of current a battery can safely supply are two different things.
- The motor driver needs enough current capacity to handle startup and possible stalls.
- The ESP32 needs a stable power supply, which is why I'm using a voltage regulator instead of powering it directly from the motor battery.
- Differential steering is actually pretty cool. Instead of using a servo to turn the wheels, I can control the speed and direction of each motor independently.
- The encoders built into my motors can eventually help me measure wheel speed, direction, and position.
- It's better to design the chassis around actual component measurements rather than guessing dimensions and hoping everything fits.

### Design Decisions

| Component | Decision |
|---|---|
| Drive system | 2-wheel differential drive + caster |
| Motors | 2× 6V, 205 RPM, 37D gearmotors |
| Wheels | 80 mm |
| Encoders | Built-in 11 PPR quadrature encoders |
| Motor driver | Cytron MDD3A |
| Main controller | ESP32 |
| Remote | ESP32 + 2-axis joystick |
| Battery | 6V NiMH |
| Obstacle sensor | 3.3V-compatible HC-SR04 |
| Chassis | Custom CAD and 3D print |
| PCB | Design after the prototype works |

### Problems / Engineering Tradeoffs
- Finding motors that were fast enough without needing an unnecessarily expensive motor driver.
- Making sure the motor driver could handle the motors' stall current.
- Keeping the ESP32's 3.3V GPIO limits in mind when choosing sensors and planning the encoder connections.
- Component availability and shipping were already becoming annoying.
- I didn't want to rush the chassis design before getting the actual hardware.

### Next
- Finalize the BOM and order everything.
- Continue practicing with the ESP32 and Arduino.
- Learn PWM motor control.
- Experiment with the ultrasonic sensor.
- Start working on the joystick and wireless communication.

---

## Days 3–4 - October 11–13, 2026

### Main Goal
Get the joystick working with my ESP32, read its position, and figure out how to turn those readings into useful inputs for the RC car.

### What I Did

#### Hardware
- Went through my RC car components and checked what I already had.
- Started working on the custom remote.
- Connected the joystick to my ESP32:

| Joystick | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| VER | GPIO 34 |
| HOR | GPIO 35 |

- Used USB to power the ESP32 while testing, so I didn't need to worry about the remote's batteries yet.

#### Firmware
- Created a PlatformIO project for the remote using the Arduino framework and C++.
- Organized my repository so the remote and car will have separate firmware projects.
- Configured serial communication at 115200 baud.
- Wrote my own program to read the joystick's vertical and horizontal positions using `analogRead()`.
- Used `Serial.print()` and `Serial.println()` to display both readings on the same line.
- Compiled and uploaded the program to the ESP32.

And it actually worked. Seeing the numbers change when I moved the joystick was ridiculously satisfying. 

#### Joystick Calibration

After getting the readings working, I measured the joystick's values in different positions.

| Direction | Vertical | Horizontal |
|---|---:|---:|
| Center | 1930 | 1955 |
| Forward | 4095 | ~1950 |
| Backward | 0 | ~1950 |
| Right | ~1920 | 0 |
| Left | ~1925 | 4095 |

A few things I noticed:
- Pushing forward increases the vertical reading.
- Pushing backward decreases it.
- Pushing left increases the horizontal reading.
- Pushing right decreases it.
- The joystick doesn't rest at exactly 2048 on either axis.

I also noticed that the readings fluctuate by about ±7 even when I'm not touching the joystick.

Obviously, I don't want the motors responding to every tiny fluctuation, so I added a deadzone of ±50 around each axis's center.

That means:
- Vertical readings between 1880 and 1980 are treated as 1930.
- Horizontal readings between 1905 and 2005 are treated as 1955.

### Problems / Debugging
- At first, the joystick kept giving me zeros unless something was touching the ESP32. I eventually got it working, but I didn't establish exactly what caused the issue.
- I initially messed up the parentheses in my `abs()` condition. I accidentally put the comparison inside `abs()`, which meant the program was checking a Boolean instead of the absolute difference.
- Fixed the parentheses and got the deadzone logic working.

### What I Learned
- The ESP32 development board has an onboard voltage regulator that converts its 5V supply to 3.3V.
- The 5V/VIN pin is a power connection, not a GPIO pin that makes 5V signals safe.
- ESP32 GPIO pins aren't 5V tolerant, so I have to be careful when connecting sensors.
- GPIO 34 and 35 can read analog voltages using the ESP32's ADC.
- The ADC gives readings from approximately 0 to 4095 with the default 12-bit resolution.
- `Serial.begin(115200)` initializes serial communication, while `Serial.print()` actually sends data to the Serial Monitor.
- Learned more about how PlatformIO, the Arduino framework, and C++ fit together.
- Learned why it's important to calibrate a joystick using its actual readings instead of assuming its center is exactly 2048.
- Learned how a deadzone prevents small fluctuations from being interpreted as movement.
- Started learning how to normalize joystick readings to a range of -1 to +1.

### Current Status
- [x] Component inventory
- [x] PlatformIO remote project
- [x] Joystick wiring
- [x] Joystick-reading firmware
- [x] Live Serial Monitor readings
- [x] Joystick calibration
- [x] Deadzone implementation
- [ ] Throttle normalization
- [ ] Steering normalization
- [ ] Differential-drive calculations
- [ ] ESP-NOW communication

### Next
- Finish normalizing the joystick readings so full forward is +1, full backward is -1, and the center is 0.
- Do the same thing for steering.
- Figure out how to combine throttle and steering to calculate the speed of each motor.
- Start learning ESP-NOW so the remote can communicate with the car wirelessly.

The joystick is officially working. Now I just need to make it actually control something.
