#include "mixer.h"
#include "servo_output.h"
#include <math.h>
#define cdeg_to_rad (1.745329e-4f)
#define rad_to_cdeg (5.729578e3f)
#define us_per_rad (636.61f)


static int16_t clamp_i16(int32_t value, int16_t min_value, int16_t max_value)
{
  if (value < min_value) return min_value;
  if (value > max_value) return max_value;
  return (int16_t)value;
}


static uint16_t servo_cdeg_to_us(int16_t angle_cdeg)
{
  int32_t pulse_us = SERVO_NEUTRAL_US + (((int32_t)angle_cdeg * 1000) / 9000);
  return clamp_i16(pulse_us, SERVO_MIN_US, SERVO_MAX_US);
}


static uint16_t servo_rad_to_us(float angle_rad)
{
  float pulse_us = SERVO_NEUTRAL_US + ( angle_rad * us_per_rad);
  if (pulse_us > SERVO_MAX_US) return SERVO_MAX_US;
  else if (pulse_us < SERVO_MIN_US) return SERVO_MIN_US;
  else return (uint16_t) pulse_us;
}


/* Direct feed-forwarding mixer */
void __attribute__((weak)) servo_mixer( const float RPY[], float ServoPos[] )
{
	ServoPos[0] = RPY[0];
	ServoPos[1] = RPY[1];
	ServoPos[2] = RPY[2];
}

void Mixer_ComputeServoUs(const Orientation_Command_t *command, uint16_t servo_us[SERVO_COUNT])
{
  float RPY[3], ServoPos[4];
  if ((command == 0) || (servo_us == 0))
  {
    return;
  }
  /* Scale the command input, convert cdeg to radians and align with ISO 1151 */
  RPY[0] =  cdeg_to_rad * command->roll_cdeg;
  RPY[1] = -cdeg_to_rad * command->pitch_cdeg;  // Android's pitch is inverted when compared to ISO 1151
  RPY[2] =  cdeg_to_rad * command->yaw_cdeg;
  servo_mixer( RPY, ServoPos );
  /* convert radians to us as used for RC servo control */
  servo_us[0] = servo_rad_to_us( ServoPos[0] );
  servo_us[1] = servo_rad_to_us( ServoPos[1] );
  servo_us[2] = servo_rad_to_us( ServoPos[2] );
  servo_us[3] = SERVO_NEUTRAL_US;
}
