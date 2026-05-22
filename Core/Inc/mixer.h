#ifndef MIXER_H
#define MIXER_H

#include <stdint.h>

#include "orientation_protocol.h"

/* @brief Convert orientation commands into servo timing
 * param[in] command
 * param[out] servo_us
 */
void Mixer_ComputeServoUs(const Orientation_Command_t *command, uint16_t servo_us[SERVO_COUNT]);

#endif /* MIXER_H */
