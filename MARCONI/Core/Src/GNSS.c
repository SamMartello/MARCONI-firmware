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

	uint8_t buf[65] = {0};



	return result;
}

uint8_t parse_gga_message(gnss_t *dev, uint8_t *msg) {

	uint8_t result = HAL_ERROR;

	/* $GPGGA,<Timestamp>,<Lat>,<N/S>,<Long>,<E/W>,<GPSQual>,<Sats>,<HDOP>,<Alt>,<AltVal>,
	 * <GeoSep>,<GeoVal>,<DGPSAge>,<DGPSRef>*<checksum><cr><lf> */

	if (strncmp(msg, GGA, strlen(GGA)) != 0) {
		return result;
	} else {
		result = HAL_OK;
	}

	//XXX
	/* still to understand whether the latitude and longitude return a string of 10 characters or 11, in the case below
	 * 10 character is assumed */

	memcpy(dev -> utc_time, &msg[7], 10);	/* copy the time in the related variable */
	memcpy(dev -> latitude, &msg[18], 10);
	dev -> latitude_dir = msg[29];
	memcpy(dev -> longitude, &msg[31], 10);
	dev -> longitude_dir = msg[42];
	memcpy(dev -> altitude, &msg[54], 6);
	dev -> altitude_meas_unit = msg[61];

	return result;
}
