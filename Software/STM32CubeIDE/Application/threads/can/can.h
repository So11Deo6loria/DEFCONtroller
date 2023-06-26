/*
 * can.h
 *
 *  Created on: Jul 7, 2022
 *      Author: nate
 */

#ifndef SRC_CAN_CAN_H_
#define SRC_CAN_CAN_H_

#ifdef __cplusplus
 extern "C" {
#endif
#include "stdint.h"

void CanChallangeThread( void * Blah );

void CanChallengeButtonPressed( uint8_t State );
#ifdef __cplusplus
 }
#endif

#endif /* SRC_CAN_CAN_H_ */
