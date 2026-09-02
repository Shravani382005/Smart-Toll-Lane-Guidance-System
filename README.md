# Smart Toll Lane Guidance System

## Project Overview

The Smart Toll Lane Guidance System is an embedded system designed to monitor the availability of toll booth lanes and guide vehicles toward available lanes.
The system uses IR sensors to detect vehicles in each lane. The lane status is displayed using LEDs and a 16×2 LCD. A servo motor is used for automatic gate control.

## Objectives

- Monitor the status of toll booth lanes.
- Detect vehicles using IR sensors.
- Indicate busy and available lanes.
- Display lane information on a 16×2 LCD.
- Control the toll gate automatically using a servo motor.

## Hardware Components

- PIC18F4520 Microcontroller
- 12 × IR Sensors
- 16×2 LCD Display
- Red LEDs
- Green LEDs
- Servo Motor
- Power Supply

##Software & Tools

- Embedded C
- MPLAB
- XC8 Compiler
- Proteus

## Working Principle

The system consists of four toll lanes.Each lane uses three IR sensors to detect the presence of vehicles.If all three sensors of a lane detect a vehicle, the lane is considered **BUSY** and the red LED is turned ON.If the lane is available, the green LED is turned ON.The 16×2 LCD displays the available lane information.A servo motor is used to control the toll booth gate.

## Features

- Four-lane monitoring
- Automatic vehicle detection
- Real-time lane status
- Red/green LED indication
- 16×2 LCD display
- Automatic gate control
- PIC18F4520-based embedded system

##  System Architecture
          IR Sensors
              │
              ▼
       ┌──────────────┐
       │  PIC18F4520  │
       │ Microcontroller│
       └──────┬───────┘
              │
       ┌──────┼─────────┐
       ▼      ▼         ▼
     LEDs     LCD     Servo
   Indicator  Display  Motor


## Proteus Simulation

The following video demonstrates the working of the Smart Toll Lane Guidance System in Proteus.
[▶ Watch Proteus Simulation](./proteus-simulation.mp4)
