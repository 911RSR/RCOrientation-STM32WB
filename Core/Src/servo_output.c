#include "servo_output.h"

#include "main.h"
#include "tim.h"

static int16_t ServoUs[SERVO_COUNT] = {
    SERVO_NEUTRAL_US,
    SERVO_NEUTRAL_US,
    SERVO_NEUTRAL_US,
    SERVO_NEUTRAL_US,
};

static const uint32_t ServoTimerChannels[SERVO_COUNT] = {
    TIM_CHANNEL_1,
    TIM_CHANNEL_2,
    TIM_CHANNEL_3,
    TIM_CHANNEL_4,
};

static int16_t clamp_servo_us(int16_t pulse_us)
{
  if (pulse_us < SERVO_MIN_US) return SERVO_MIN_US;
  if (pulse_us > SERVO_MAX_US) return SERVO_MAX_US;
  return pulse_us;
}

void ServoOutput_Init(void)
{
  /* Only PA0..PA2 are populated on the bench today; PA3 is configured but left idle. */
  for (uint8_t servo_index = 0; servo_index < 3U; servo_index++)
  {
    if (HAL_TIM_PWM_Start(&htim2, ServoTimerChannels[servo_index]) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_TIM_SET_COMPARE(&htim2, ServoTimerChannels[servo_index], (uint16_t)ServoUs[servo_index]);
  }
}

void ServoOutput_SetUs(uint8_t servo_index, int16_t pulse_us)
{
  if (servo_index >= SERVO_COUNT)
  {
    return;
  }

  ServoUs[servo_index] = clamp_servo_us(pulse_us);

  /* Servo 3 / PA3 is reserved in the model but not physically populated yet. */
  if (servo_index < 3U)
  {
    __HAL_TIM_SET_COMPARE(&htim2, ServoTimerChannels[servo_index], (uint16_t)ServoUs[servo_index]);
  }
}

void ServoOutput_SetAllUs(const int16_t pulse_us[SERVO_COUNT])
{
  if (pulse_us == 0)
  {
    return;
  }

  for (uint8_t servo_index = 0; servo_index < SERVO_COUNT; servo_index++)
  {
    ServoOutput_SetUs(servo_index, pulse_us[servo_index]);
  }
}

const int16_t *ServoOutput_GetAllUs(void)
{
  return ServoUs;
}
