#include "orientation_app.h"

#include "main.h"
#include "orientation_protocol.h"
#include "common_blesvc.h"
#include "custom_stm.h"
#include "mixer.h"
#include "servo_output.h"

#include <string.h>

static Orientation_Command_t LatestOrientationCommand;
static Servo_Feedback_t LatestServoFeedback = {{SERVO_NEUTRAL_US, SERVO_NEUTRAL_US, SERVO_NEUTRAL_US, SERVO_NEUTRAL_US}, 0};
static uint8_t ServoFeedbackNotificationsEnabled;

static void publish_servo_feedback(void)
{
  if (ServoFeedbackNotificationsEnabled != 0U)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_SERVO_FEEDBACK,
                               (uint8_t *)&LatestServoFeedback);
  }
}

void OrientationApp_Init(void)
{
  memset(&LatestOrientationCommand, 0, sizeof(LatestOrientationCommand));
  ServoOutput_Init();
  memcpy(LatestServoFeedback.servo_us,
         ServoOutput_GetAllUs(),
         sizeof(LatestServoFeedback.servo_us));
}

void OrientationApp_HandleCommand(const uint8_t *payload, uint8_t length)
{
  if ((payload == 0) || (length != sizeof(Orientation_Command_t)))
  {
    return;
  }

  memcpy(&LatestOrientationCommand, payload, sizeof(Orientation_Command_t));
  uint16_t requested_servo_us[SERVO_COUNT] = {0};

  LatestServoFeedback.sequence = LatestOrientationCommand.sequence;
  Mixer_ComputeServoUs(&LatestOrientationCommand, requested_servo_us);
  ServoOutput_SetAllUs(requested_servo_us);
  memcpy(LatestServoFeedback.servo_us,
         ServoOutput_GetAllUs(),
         sizeof(LatestServoFeedback.servo_us));
  publish_servo_feedback();
}

void OrientationApp_SetServoFeedbackNotificationsEnabled(uint8_t enabled)
{
  ServoFeedbackNotificationsEnabled = (enabled != 0U) ? 1U : 0U;
  if (ServoFeedbackNotificationsEnabled != 0U)
  {
    publish_servo_feedback();
  }
}
