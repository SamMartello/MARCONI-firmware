/*
 * utils.c
 *
 *  Created on: May 3, 2024
 *      Author: tommaso
 */

#include "utils.h"

#ifdef DEBUG_EN

extern UART_HandleTypeDef huart2;
//extern UART_HandleTypeDef huart4;

#include <string.h>

HAL_StatusTypeDef COM_port_serial_print(const uint8_t* data) {
	return HAL_UART_Transmit(&huart2, data, strlen((const char *)data) + 1, 100);
//	return HAL_UART_Transmit(&huart4, data, strlen((const char *)data) + 1, 100);
}
#endif
