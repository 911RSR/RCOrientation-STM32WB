#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "orientation_protocol.h"
#include "tim.h"
#include <string.h>

typedef struct
{
  uint8_t Servo_Feedback_Notification_Status;
  uint16_t ConnectionHandle;
} Custom_App_Context_t;

static Custom_App_Context_t Custom_App_Context;
static Orientation_Command_t LatestOrientationCommand;
static Servo_Feedback_t LatestServoFeedback = {1500, 1500, 1500, 0};

static int16_t clamp_i16(int32_t value, int16_t min_value, int16_t max_value)
{
  if (value < min_value) return min_value;
  if (value > max_value) return max_value;
  return (int16_t)value;
}

static int16_t roll_cdeg_to_servo_us(int16_t roll_cdeg)
{
  /* Temporary first-test mapping: +/-45 deg -> 1000..2000 us */
  int32_t pulse_us = 1500 + (((int32_t)roll_cdeg * 1000) / 9000);
  return clamp_i16(pulse_us, 1000, 2000);
}

void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  switch (pNotification->Custom_Evt_Opcode)
  {
    case CUSTOM_STM_ORIENTATION_COMMAND_WRITE_EVT:
      if (pNotification->DataTransfered.Length == sizeof(Orientation_Command_t))
      {
        memcpy(&LatestOrientationCommand,
               pNotification->DataTransfered.pPayload,
               sizeof(Orientation_Command_t));

        LatestServoFeedback.servo_roll_us = roll_cdeg_to_servo_us(LatestOrientationCommand.roll_cdeg);
        LatestServoFeedback.servo_pitch_us = 1500;
        LatestServoFeedback.servo_yaw_us = 1500;
        LatestServoFeedback.sequence = LatestOrientationCommand.sequence;

        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint16_t)LatestServoFeedback.servo_roll_us);
        Orientation_PublishServoFeedback();
      }
      break;

    case CUSTOM_STM_SERVO_FEEDBACK_NOTIFY_ENABLED_EVT:
      Custom_App_Context.Servo_Feedback_Notification_Status = 1;
      Orientation_PublishServoFeedback();
      break;

    case CUSTOM_STM_SERVO_FEEDBACK_NOTIFY_DISABLED_EVT:
      Custom_App_Context.Servo_Feedback_Notification_Status = 0;
      break;

    default:
      break;
  }
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  switch (pNotification->Custom_Evt_Opcode)
  {
    case CUSTOM_CONN_HANDLE_EVT:
      Custom_App_Context.ConnectionHandle = pNotification->ConnectionHandle;
      break;
    case CUSTOM_DISCON_HANDLE_EVT:
      Custom_App_Context.ConnectionHandle = 0;
      Custom_App_Context.Servo_Feedback_Notification_Status = 0;
      break;
    default:
      break;
  }
}

void Custom_APP_Init(void)
{
  memset(&LatestOrientationCommand, 0, sizeof(LatestOrientationCommand));
  if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, LatestServoFeedback.servo_roll_us);
}

void Orientation_SetServoFeedback(int16_t roll_us, int16_t pitch_us, int16_t yaw_us)
{
  LatestServoFeedback.servo_roll_us = roll_us;
  LatestServoFeedback.servo_pitch_us = pitch_us;
  LatestServoFeedback.servo_yaw_us = yaw_us;
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint16_t)roll_us);
}

void Orientation_PublishServoFeedback(void)
{
  if (Custom_App_Context.Servo_Feedback_Notification_Status != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_SERVO_FEEDBACK,
                               (uint8_t *)&LatestServoFeedback);
  }
}

const Orientation_Command_t *Orientation_GetLatestCommand(void)
{
  return &LatestOrientationCommand;
}
