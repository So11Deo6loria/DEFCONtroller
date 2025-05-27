/*
 * device_status.c
 *
 *  Created on: May 26, 2025
 *      Author: caleb
 */

#include "device_status.h"

DeviceStatus_t gDeviceStatus = {
    .pin = "2005",
    .hudState = "REcon",
    .firmwareVersion = "0.3",
    .debugEnabled = 0
};
