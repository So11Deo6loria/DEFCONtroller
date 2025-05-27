/*
 * device_status.h
 *
 *  Created on: May 26, 2025
 *      Author: caleb
 */

#ifndef APPLICATION_THREADS_COMMON_DEVICE_STATUS_H_
#define APPLICATION_THREADS_COMMON_DEVICE_STATUS_H_

#include <stdint.h>

typedef struct {
    char pin[8];
    char hudState[16];
    char firmwareVersion[16];
    uint8_t debugEnabled; // 0 = Disabled, 1 = Enabled
} DeviceStatus_t;

extern DeviceStatus_t gDeviceStatus;

#endif /* APPLICATION_THREADS_COMMON_DEVICE_STATUS_H_ */
