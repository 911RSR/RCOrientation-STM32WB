# RCOrientation-STM32WB

STM32WB55 firmware for an Android-controlled roll / pitch / yaw servo receiver.

This project runs on a WeActStudio STM32WB55 CoreBoard and pairs with the companion Android app, [RCOrientation-Android](https://github.com/911RSR/RCOrientation-Android). The Android phone sends orientation commands over BLE; the STM32WB drives RC servos and reports actuator positions back.

## Current status

Working bench prototype:

- advertises as `RCOrient`
- requires BLE pairing, bonding, and encrypted access to the control characteristics
- receives roll / pitch / yaw commands from the Android app
- configures four servo PWM outputs on `PA0..PA3 / TIM2_CH1..CH4`
- reports servo positions back to the app

Servo mixer with direct forwarding:

- roll drives `servo_us[0]`
- pitch drives `servo_us[1]`
- yaw drives `servo_us[2]`
- `servo_us[3]` stays neutral
- approximately `-45 deg .. +45 deg` maps to `1000 us .. 2000 us`

This mixer is mainly for bench testing, although it could teoretically be used for short-range RC plane control; 
A later mixer will map roll / pitch / yaw / gyro demands into the real actuator geometry.

## Hardware

Tested with:

- WeActStudio STM32WB55 CoreBoard
- STM32WB55CGU6
- ST BLE wireless stack already flashed on CPU2

Current bench wiring:

| Function | STM32 pin | Peripheral |
| --- | --- | --- |
| Servo 0 PWM | `PA0` | `TIM2_CH1` |
| Servo 1 PWM | `PA1` | `TIM2_CH2` |
| Servo 2 PWM | `PA2` | `TIM2_CH3` |
| Servo 3 PWM | `PA3` | `TIM2_CH4` |

Servo PWM is currently configured for 300 Hz with 1 us resolution during latency testing.

## BLE protocol

Service UUID: `7B5D0000-6A2E-4D6B-9E6F-8D2B7C5A1100`

| Characteristic | Direction | Properties | Size |
| --- | --- | --- | --- |
| `0x0001` Orientation command | phone -> STM32WB | encrypted write / write without response | 8 bytes |
| `0x0002` Servo feedback | STM32WB -> phone | encrypted read / notify | 10 bytes |

Orientation command payload:

```c
int16_t roll_cdeg;
int16_t pitch_cdeg;
int16_t yaw_cdeg;
uint16_t sequence;
```

Servo feedback payload:

```c
int16_t servo_us[4];
uint16_t sequence;
```

## Build and flash

1. Open `RCOrientation-STM32WB.ioc` in STM32CubeMX or STM32CubeIDE.
2. Let STM32CubeIDE/CubeMX generate the local Eclipse project metadata on your own PC.
3. Build the project in STM32CubeIDE.
4. Flash it to the WeAct STM32WB55 board using your normal ST-LINK / STM32CubeIDE workflow.
5. Pair from the Android app before expecting control traffic to flow.

The `.project`, `.cproject`, `.mxproject`, `.settings/`, and `*.launch` files are intentionally not tracked because STM32CubeIDE fills them with machine-local paths.

## CubeMX regeneration notes

The `.ioc` file is intended to be the source of truth for generated hardware setup. It currently describes:

- `PA0..PA3 / TIM2_CH1..CH4` as PWM outputs for `servo_us[0..3]`
- the `RCOrient` BLE identity
- the orientation-service naming and packet sizes
- bonding enabled with no-input/no-output capability

Application logic should stay outside generated files where practical. Project-specific code currently lives in:

- `Core/Inc/orientation_protocol.h`
- `Core/Inc/orientation_app.h` and `Core/Src/orientation_app.c`
- `Core/Inc/mixer.h` and `Core/Src/mixer.c`
- `Core/Inc/servo_output.h` and `Core/Src/servo_output.c`

BLE service and generated application hooks currently live in:

- `STM32_WPAN/App/custom_stm.*`
- `STM32_WPAN/App/custom_app.*`

Current CubeMX versions used during development regenerate duplicate BLE stubs for the read+notify feedback characteristic. Until ST fixes that generator bug, run this after code generation:

```powershell
.\tools\fix-cubemx-ble-duplicates.ps1
```

## Security note

The present configuration requires a bonded encrypted BLE link. It uses a simple no-input/no-output pairing mode, so it does not provide man-in-the-middle protection during the very first pairing ceremony.

## Related project

- Android app: [RCOrientation-Android](https://github.com/911RSR/RCOrientation-Android)
