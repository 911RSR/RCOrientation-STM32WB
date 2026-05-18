#include "mixer.h"

#include "servo_output.h"

static int16_t clamp_i16(int32_t value, int16_t min_value, int16_t max_value)
{
  if (value < min_value) return min_value;
  if (value > max_value) return max_value;
  return (int16_t)value;
}

static int16_t attitude_cdeg_to_servo_us(int16_t angle_cdeg)
{
  /* Dummy bench mixer mapping: +/-45 deg -> 1000..2000 us */
  int32_t pulse_us = SERVO_NEUTRAL_US + (((int32_t)angle_cdeg * 1000) / 9000);
  return clamp_i16(pulse_us, SERVO_MIN_US, SERVO_MAX_US);
}

void Mixer_ComputeServoUs(const Orientation_Command_t *command, int16_t servo_us[SERVO_COUNT])
{
  if ((command == 0) || (servo_us == 0))
  {
    return;
  }

  /* Temporary direct-forwarding mixer for bench testing. */
  servo_us[0] = attitude_cdeg_to_servo_us(command->roll_cdeg);
  servo_us[1] = attitude_cdeg_to_servo_us(command->pitch_cdeg);
  servo_us[2] = attitude_cdeg_to_servo_us(command->yaw_cdeg);
  servo_us[3] = SERVO_NEUTRAL_US;
}
