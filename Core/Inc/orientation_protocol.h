#ifndef ORIENTATION_PROTOCOL_H
#define ORIENTATION_PROTOCOL_H

#include <stdint.h>

#define ORIENTATION_COMMAND_SIZE_BYTES 8u
#define SERVO_FEEDBACK_SIZE_BYTES      8u

typedef struct __attribute__((packed))
{
  int16_t roll_cdeg;
  int16_t pitch_cdeg;
  int16_t yaw_cdeg;
  uint16_t sequence;
} Orientation_Command_t;

typedef struct __attribute__((packed))
{
  int16_t servo_roll_us;
  int16_t servo_pitch_us;
  int16_t servo_yaw_us;
  uint16_t sequence;
} Servo_Feedback_t;

#endif /* ORIENTATION_PROTOCOL_H */
