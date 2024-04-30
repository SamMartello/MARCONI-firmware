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

	/* message synopsys
	 * $PSTMINITGPS,<Lat>,<LatRef>,<Lon>,<LonRef>,<Alt>,<Day>,<Month>,<Year>,<Hour>,<Minute>,<Second>*<checksum><cr><lf>
	 */

	uint8_t buf[65] = {0};

	uint8_t latitude[9] = {0};
	uint8_t lat_dir[2] = {0};
	uint8_t longitude[10] = {0};
	uint8_t lon_dir[2] = {0};
	uint8_t altitude[5] = {0};
	uint8_t day[3] = {0};
	uint8_t month[3] = {0};
	uint8_t year[5] = {0};
	uint8_t hour[3] = {0};
	uint8_t minute[3] = {0};
	uint8_t second[3] = {0};

	sprintf(latitude, "%8.3f", params -> latitude);
	lat_dir[0] = params -> lat_dir;
	sprintf(longitude, "%9.3f", params -> longitude);
	lon_dir[0] = params -> lon_dir;
	sprintf(altitude, "%d", params -> altitude);
	sprintf(day, "%d", params -> day);
	sprintf(month, "%d", params -> month);
	sprintf(year, "%d", params -> year);
	sprintf(hour, "%d", params -> hour);
	sprintf(minute, "%d", params -> minute);
	sprintf(second, "%d", params -> second);

	strcat(buf, GPS_TIME_POS_INIT);
	strcat(buf, ",");
	strcat(buf, latitude);
	strcat(buf, ",");
	strcat(buf, lat_dir);
	strcat(buf, ",");
	strcat(buf, longitude);
	strcat(buf, ",");
	strcat(buf, lon_dir);
	strcat(buf, ",");
	strcat(buf, altitude);
	strcat(buf, ",");
	strcat(buf, day);
	strcat(buf, ",");
	strcat(buf, month);
	strcat(buf, ",");
	strcat(buf, year);
	strcat(buf, ",");
	strcat(buf, hour);
	strcat(buf, ",");
	strcat(buf, minute);
	strcat(buf, ",");
	strcat(buf, second);
	strcat(buf, "\r\n");

	// TODO build the initialization string for the gnss

//	strncat(buf, GPS_TIME_POS_INIT, strlen(GPS_TIME_POS_INIT))
//	offset = snprintf(buf, 8, "%8.3f", params -> latitude);
//	offset = snprintf(buf + offset, 1, "%c", params -> lat_dir);
//	offset = snprintf(buf + offset, )

	return result;
}

uint8_t parse_gga_message(gnss_t *dev, uint8_t *msg) {

	uint8_t result = HAL_ERROR;

	/* $GPGGA,<Timestamp>,<Lat>,<N/S>,<Long>,<E/W>,<GPSQual>,<Sats>,<HDOP>,<Alt>,<AltVal>,
	 * <GeoSep>,<GeoVal>,<DGPSAge>,<DGPSRef>*<checksum><cr><lf> */

	if (strncmp(msg, GGA, strlen(GGA)) == 0) {
		result = HAL_OK;
	} else {
		return result;
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

uint8_t parse_gll_message(gnss_t *dev, uint8_t *msg) {

	uint8_t result = HAL_ERROR;

	/* $GPGLL,<Lat>,<N/S>,<Long>,<E/W>,<Timestamp>,<Status>,<mode indicator>*<checksum><cr><lf> */
	/* example: $GPGLL,4055.04673,N,01416.54941,E,110505.000,A,A*54 */

	if (strncmp(msg, GLL, strlen(GLL)) == 0) {
		result = HAL_OK;
	} else {
		return result;
	}

	/* still to understand whether the latitude and longitude return a string of 10 characters or 11, in the case below
	 * 10 character is assumed */

	memcpy(dev -> latitude, &msg[7], 10);
	dev -> latitude_dir = msg[18];
	memcpy(dev -> longitude, &msg[20], 10);
	dev -> longitude_dir = msg[31];
	memcpy(dev -> utc_time, &msg[33], 10);

	return result;
}

uint8_t assemble_ground_station_packet(gnss_t *dev, uint8_t *buf) {

	uint8_t result = HAL_ERROR;

	/* packet structure */

	return result;
}



















