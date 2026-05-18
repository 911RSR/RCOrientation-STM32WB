#ifndef ORIENTATION_APP_H
#define ORIENTATION_APP_H

#include <stdint.h>

void OrientationApp_Init(void);
void OrientationApp_HandleCommand(const uint8_t *payload, uint8_t length);
void OrientationApp_SetServoFeedbackNotificationsEnabled(uint8_t enabled);

#endif /* ORIENTATION_APP_H */
