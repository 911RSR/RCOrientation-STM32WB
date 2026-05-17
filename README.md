# RCOrientation-STM32WB

STM32WB55 firmware for an Android-controlled roll / pitch / yaw servo receiver.

This project was derived from a known-good STM32WB BLE project for the WeActStudio STM32WB55 CoreBoard, then simplified into a dedicated orientation receiver.

The firmware currently:

- advertises as `RCOrient`
- exposes a custom BLE orientation service
- requires bonded encrypted BLE access for the control characteristics
- receives roll, pitch, and yaw commands from the Android app
- drives a first RC servo test channel on `PA0 / TIM2_CH1`
- reports servo positions back to the Android app

## Hardware

Tested with:

- WeActStudio STM32WB55 CoreBoard
- STM32WB55CGU6
- BLE wireless stack already flashed on CPU2

Current one-servo bench test:

- servo signal: `PA0`
- timer output: `TIM2_CH1`
- servo PWM: 50 Hz, 1 us timer resolution

## BLE protocol

Service UUID: `7B5D0000-6A2E-4D6B-9E6F-8D2B7C5A1100`

Characteristics:

- `0x0001` Orientation command, phone -> STM32WB, encrypted write / write without response, 8 bytes
- `0x0002` Servo feedback, STM32WB -> phone, encrypted read / notify, 8 bytes

Orientation command payload:

```c
int16_t roll_cdeg;
int16_t pitch_cdeg;
int16_t yaw_cdeg;
uint16_t sequence;
```

Servo feedback payload:

```c
int16_t servo_roll_us;
int16_t servo_pitch_us;
int16_t servo_yaw_us;
uint16_t sequence;
```

## Current servo mapping

For the first bench test only:

- roll controls the physical servo on `PA0`
- approximately `-45 deg .. +45 deg` maps to `1000 us .. 2000 us`
- pitch and yaw feedback currently report neutral `1500 us`

The intended next step is to add the real three-axis servo mapping and choose final output pins in CubeMX.

## Project structure

The application-specific protocol is defined in:

- `Core/Inc/orientation_protocol.h`

BLE service and application hooks live in:

- `STM32_WPAN/App/custom_stm.*`
- `STM32_WPAN/App/custom_app.*`

## Development notes

The project is intended to remain CubeMX-friendly: generated files should contain only small glue sections, while larger application logic should live in separate source files.

The companion Android app project is `RCOrientation-Android`.

