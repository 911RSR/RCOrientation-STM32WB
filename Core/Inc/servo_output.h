#ifndef SERVO_OUTPUT_H
#define SERVO_OUTPUT_H

#include <stdint.h>

#include "orientation_protocol.h"

/* The physical mapping depends on the servo specification.
 * For my servos:
 *    Preliminary mapping: +/-45 deg -> 1000..2000 us.
 *    Clockwise angle (when facing the top of the servo) = lower us.
 * */
#define SERVO_NEUTRAL_US 1500
#define SERVO_MIN_US     1000
#define SERVO_MAX_US     2000

void ServoOutput_Init(void);
void ServoOutput_SetUs(uint8_t servo_index, uint16_t pulse_us);
void ServoOutput_SetAllUs(const uint16_t pulse_us[SERVO_COUNT]);
const uint16_t *ServoOutput_GetAllUs(void);

#endif /* SERVO_OUTPUT_H */
