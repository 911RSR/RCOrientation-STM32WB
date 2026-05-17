#ifndef CUSTOM_APP_H
#define CUSTOM_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "orientation_protocol.h"

typedef enum
{
  CUSTOM_CONN_HANDLE_EVT,
  CUSTOM_DISCON_HANDLE_EVT,
} Custom_App_Opcode_Notification_evt_t;

typedef struct
{
  Custom_App_Opcode_Notification_evt_t Custom_Evt_Opcode;
  uint16_t ConnectionHandle;
} Custom_App_ConnHandle_Not_evt_t;

void Custom_APP_Init(void);
void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification);
void Orientation_SetServoFeedback(int16_t roll_us, int16_t pitch_us, int16_t yaw_us);
void Orientation_PublishServoFeedback(void);
const Orientation_Command_t *Orientation_GetLatestCommand(void);

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_APP_H */
