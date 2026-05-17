#ifndef CUSTOM_STM_H
#define CUSTOM_STM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ble.h"

typedef enum
{
  CUSTOM_STM_ORIENTATION_COMMAND,
  CUSTOM_STM_SERVO_FEEDBACK,
} Custom_STM_Char_Opcode_t;

typedef enum
{
  CUSTOM_STM_ORIENTATION_COMMAND_WRITE_EVT,
  CUSTOM_STM_SERVO_FEEDBACK_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_SERVO_FEEDBACK_NOTIFY_DISABLED_EVT,
  CUSTOM_STM_NOTIFICATION_COMPLETE_EVT,
  CUSTOM_STM_BOOT_REQUEST_EVT
} Custom_STM_Opcode_evt_t;

typedef struct
{
  uint8_t * pPayload;
  uint8_t   Length;
} Custom_STM_Data_t;

typedef struct
{
  Custom_STM_Opcode_evt_t Custom_Evt_Opcode;
  Custom_STM_Data_t       DataTransfered;
  uint16_t                ConnectionHandle;
  uint8_t                 ServiceInstance;
  uint16_t                AttrHandle;
} Custom_STM_App_Notification_evt_t;

extern uint16_t SizeOrientation_Command;
extern uint16_t SizeServo_Feedback;

void SVCCTL_InitCustomSvc(void);
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification);
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload);
tBleStatus Custom_STM_App_Update_Char_Variable_Length(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload, uint8_t size);
tBleStatus Custom_STM_App_Update_Char_Ext(uint16_t Connection_Handle, Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload);

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_STM_H */
