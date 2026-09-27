# RC Car Build Log

## Day 2 - September 25, 2026

### Main Goal

Define the car's architecture and figure out what hardware is actually
needed before ordering parts.

### What I Did

-   Finalized the main scope of the RC car:
    -   Forward and reverse movement
    -   Left and right turning using differential steering
    -   Variable motor speed
    -   Custom wireless handheld remote
    -   Ultrasonic distance sensing
    -   Automatic collision prevention
    -   Custom 3D-printed chassis and remote enclosure
    -   Custom PCB later in the project
-   Decided to use two independently controlled DC gearmotors instead of
    servo steering.
-   Researched the drivetrain and selected 2× 6V 205 RPM 37D gearmotors.
-   Selected motors that already include:
    -   80 mm wheels
    -   11 PPR 2-channel magnetic encoders
    -   Motor mounting hardware
-   Selected the Cytron MDD3A dual-channel motor driver.
-   Planned the car power system around a 6V NiMH battery.
-   Selected a 5V step-up/step-down regulator for powering the car ESP32
    separately from the motors.
-   Selected a 3.3V HC-SR04 ultrasonic sensor for obstacle detection.
-   Planned the remote around:
    -   A second ESP32
    -   A 2-axis joystick
    -   Direct wireless communication between the two ESP32s
-   Decided to CAD and 3D print the chassis after receiving the physical
    components so the design can use real measurements.
-   Decided to have Sebastian print the chassis/enclosure from my STL
    files instead of relying on the lab printer.
-   Started building a complete BOM and checking component compatibility
    before ordering.

### What I Learned

-   Motor stall current matters when choosing both the motor driver and
    battery.
-   Battery capacity in mAh is different from the amount of current a
    battery can safely supply.
-   A motor driver's current rating needs enough headroom for motor
    startup and stall current.
-   The ESP32 should have a stable regulated power supply instead of
    sharing the raw motor power path directly.
-   Differential steering allows the car to turn without a steering
    servo by controlling the left and right motors independently.
-   Built-in quadrature encoders can later provide wheel speed,
    direction, and position feedback.
-   Mechanical design should be based on the actual dimensions of the
    components rather than guessed dimensions.

### Design Decisions

-   **Drive system:** 2-wheel differential drive + passive swivel caster
-   **Motors:** 2× 6V, 205 RPM, 37D gearmotors
-   **Wheels:** 80 mm
-   **Feedback:** Built-in 11 PPR quadrature encoders
-   **Motor driver:** Cytron MDD3A
-   **Main controller:** ESP32
-   **Remote controller:** ESP32 + 2-axis joystick
-   **Main battery:** 6V NiMH
-   **Obstacle sensor:** 3.3V HC-SR04
-   **Chassis:** Custom CAD + 3D print
-   **PCB:** Design only after the prototype circuit works

### Problems / Engineering Tradeoffs

-   Needed motors that were fast enough for an RC car without requiring
    an oversized motor driver.
-   Had to account for motor stall current rather than only normal
    operating current.
-   Needed to keep ESP32 GPIO voltage compatibility in mind when
    selecting sensors and future encoder wiring.
-   Component availability and shipping times became part of the design
    process.
-   Avoided locking the chassis dimensions before having the actual
    hardware.

### Next

-   Finalize and order the BOM.
-   Continue ESP32 and electronics practice while parts are shipping.
-   Learn PWM motor-speed control.
-   Test the ultrasonic sensor.
-   Begin experimenting with the joystick and wireless communication.
-   Measure the physical components when they arrive and start chassis
    CAD.
