#ifndef MIXER_H
#define MIXER_H

#include <stdint.h>

#include "orientation_protocol.h"

void Mixer_ComputeServoUs(const Orientation_Command_t *command, int16_t servo_us[SERVO_COUNT]);

#endif /* MIXER_H */
