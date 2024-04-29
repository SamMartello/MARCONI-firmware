/*
 * GNSS.c
 *
 *  Created on: Apr 14, 2024
 *      Author: tommaso
 */

#ifndef GNSS_H
#include "GNSS.h"
#endif

uint8_t gnss_init(gnss_t *dev, UART_HandleTypeDef *huart) {

	dev -> handler = huart;

	return 0;
}

uint8_t gnss_set_init_position_time(gnss_t *dev, config_params_t *params) {

	uint8_t result = HAL_ERROR;

	uint8_t buf[60] = {0};	/* check correctly the right size of the message */

	return result;
}

