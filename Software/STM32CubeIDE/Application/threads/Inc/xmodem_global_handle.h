/*
 * xmodem_global_handle.h
 *
 *  Created on: Jul 4, 2023
 *      Author: calebdavis
 */

#ifndef APPLICATION_THREADS_INC_XMODEM_GLOBAL_HANDLE_H_
#define APPLICATION_THREADS_INC_XMODEM_GLOBAL_HANDLE_H_


#include "xmodem.h"

// Because the Simple Programmer operates in a single thread and therefore has no need for simultaneous
// instances of Xmodem, we can create a single handle to share across the project.

XModem1KHandle XModemGlobalHandle;

#endif /* APPLICATION_THREADS_INC_XMODEM_GLOBAL_HANDLE_H_ */
