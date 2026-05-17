#include "common_blesvc.h"
#include "custom_stm.h"
#include "orientation_protocol.h"

typedef struct
{
  uint16_t OrientationServiceHdle;
  uint16_t OrientationCommandHdle;
  uint16_t ServoFeedbackHdle;
} CustomContext_t;

#define UUID_128_SUPPORTED 1
#define CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET 2
#define CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET 1

uint16_t SizeOrientation_Command = ORIENTATION_COMMAND_SIZE_BYTES;
uint16_t SizeServo_Feedback = SERVO_FEEDBACK_SIZE_BYTES;
static CustomContext_t CustomContext;

static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *pckt);

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do { \
  uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
  uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
  uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
  uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
} while (0)

/* 7B5D0000-6A2E-4D6B-9E6F-8D2B7C5A1100 */
#define COPY_ORIENTATION_SERVICE_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x7B,0x5D,0x00,0x00,0x6A,0x2E,0x4D,0x6B,0x9E,0x6F,0x8D,0x2B,0x7C,0x5A,0x11,0x00)

static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value = SVCCTL_EvtNotAck;
  hci_event_pckt *event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  if (event_pckt->evt == HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE)
  {
    evt_blecore_aci *blecore_evt = (evt_blecore_aci*)event_pckt->data;
    if (blecore_evt->ecode == ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE)
    {
      aci_gatt_attribute_modified_event_rp0 *attribute_modified =
          (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
      Custom_STM_App_Notification_evt_t Notification = {0};

      if (attribute_modified->Attr_Handle ==
          (CustomContext.ServoFeedbackHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
      {
        return_value = SVCCTL_EvtAckFlowEnable;
        Notification.Custom_Evt_Opcode =
            (attribute_modified->Attr_Data[0] == COMSVC_Notification)
                ? CUSTOM_STM_SERVO_FEEDBACK_NOTIFY_ENABLED_EVT
                : CUSTOM_STM_SERVO_FEEDBACK_NOTIFY_DISABLED_EVT;
        Custom_STM_App_Notification(&Notification);
      }
      else if (attribute_modified->Attr_Handle ==
               (CustomContext.OrientationCommandHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
      {
        return_value = SVCCTL_EvtAckFlowEnable;
        Notification.Custom_Evt_Opcode = CUSTOM_STM_ORIENTATION_COMMAND_WRITE_EVT;
        Notification.DataTransfered.pPayload = attribute_modified->Attr_Data;
        Notification.DataTransfered.Length = attribute_modified->Attr_Data_Length;
        Custom_STM_App_Notification(&Notification);
      }
    }
  }

  return return_value;
}

void SVCCTL_InitCustomSvc(void)
{
  Char_UUID_t uuid;
  uint8_t max_attr_record = 6;

  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

  COPY_ORIENTATION_SERVICE_UUID(uuid.Char_UUID_128);
  aci_gatt_add_service(UUID_TYPE_128,
                       (Service_UUID_t *)&uuid,
                       PRIMARY_SERVICE,
                       max_attr_record,
                       &(CustomContext.OrientationServiceHdle));

  uuid.Char_UUID_16 = 0x0001;
  aci_gatt_add_char(CustomContext.OrientationServiceHdle,
                    UUID_TYPE_16,
                    &uuid,
                    SizeOrientation_Command,
                    CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP,
                    ATTR_PERMISSION_ENCRY_WRITE,
                    GATT_NOTIFY_ATTRIBUTE_WRITE,
                    0x10,
                    CHAR_VALUE_LEN_CONSTANT,
                    &(CustomContext.OrientationCommandHdle));

  uuid.Char_UUID_16 = 0x0002;
  aci_gatt_add_char(CustomContext.OrientationServiceHdle,
                    UUID_TYPE_16,
                    &uuid,
                    SizeServo_Feedback,
                    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                    ATTR_PERMISSION_ENCRY_READ,
                    GATT_NOTIFY_NOTIFICATION_COMPLETION,
                    0x10,
                    CHAR_VALUE_LEN_CONSTANT,
                    &(CustomContext.ServoFeedbackHdle));
}

tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  switch (CharOpcode)
  {
    case CUSTOM_STM_ORIENTATION_COMMAND:
      return aci_gatt_update_char_value(CustomContext.OrientationServiceHdle,
                                        CustomContext.OrientationCommandHdle,
                                        0,
                                        SizeOrientation_Command,
                                        pPayload);
    case CUSTOM_STM_SERVO_FEEDBACK:
      return aci_gatt_update_char_value(CustomContext.OrientationServiceHdle,
                                        CustomContext.ServoFeedbackHdle,
                                        0,
                                        SizeServo_Feedback,
                                        pPayload);
    default:
      return BLE_STATUS_INVALID_PARAMS;
  }
}

tBleStatus Custom_STM_App_Update_Char_Variable_Length(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload, uint8_t size)
{
  (void)size;
  return Custom_STM_App_Update_Char(CharOpcode, pPayload);
}

tBleStatus Custom_STM_App_Update_Char_Ext(uint16_t Connection_Handle, Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  (void)Connection_Handle;
  return Custom_STM_App_Update_Char(CharOpcode, pPayload);
}


