#ifndef SERVO_OUTPUT_H
#define SERVO_OUTPUT_H

#include <stdint.h>

#include "orientation_protocol.h"

#define SERVO_NEUTRAL_US 1500
#define SERVO_MIN_US     1000
#define SERVO_MAX_US     2000

void ServoOutput_Init(void);
void ServoOutput_SetUs(uint8_t servo_index, int16_t pulse_us);
void ServoOutput_SetAllUs(const int16_t pulse_us[SERVO_COUNT]);
const int16_t *ServoOutput_GetAllUs(void);

#endif /* SERVO_OUTPUT_H */
