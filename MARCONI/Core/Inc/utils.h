/*
 * utils.h
 *
 *  Created on: May 3, 2024
 *      Author: tommaso
 */

#ifndef UTILS_H
#define UTILS_H

#define DEBUG_EN

#ifdef DEBUG_EN

#include "stm32g0xx_hal.h"

HAL_StatusTypeDef COM_port_serial_print(const uint8_t* data);
#endif

#endif /* UTILS_H */
