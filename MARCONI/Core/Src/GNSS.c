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

	sprintf((char *)latitude, "%8.3f", params -> latitude);
	lat_dir[0] = params -> lat_dir;
	sprintf((char *)longitude, "%9.3f", params -> longitude);
	lon_dir[0] = params -> lon_dir;

	/* when integer transformed into string we have no leading zeros to make the string with a fixed size
	 * FIXED: fixed using format specifier */
	sprintf((char *)altitude, "%04d", params -> altitude);
	sprintf((char *)day, "%02d", params -> day);
	sprintf((char *)month, "%02d", params -> month);
	sprintf((char *)year, "%04d", params -> year);
	sprintf((char *)hour, "%02d", params -> hour);
	sprintf((char *)minute, "%02d", params -> minute);
	sprintf((char *)second, "%02d", params -> second);

	strcat((char *)buf, (const char *)GPS_TIME_POS_INIT);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)latitude);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)lat_dir);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)longitude);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)lon_dir);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)altitude);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)day);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)month);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)year);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)hour);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)minute);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)second);
	strcat((char *)buf, (const char *)"\r\n");

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

	if (strncmp((const char *)msg, (const char *)GGA, strlen(GGA)) == 0) {
		result = HAL_OK;
	} else {
		return result;
	}

	//XXX
	/* still to understand whether the latitude and longitude return a string of 10 characters or 11, in the case below
	 * 10 character is assumed */

	/* memcpy(dev -> utc_time, &msg[7], 10);	/* copy the time in the related variable */
	memcpy(dev -> utc_time, &msg[7], 6);	/* copy the time in the related variable, do not copy millisecond */
	memcpy(dev -> latitude, &msg[18], 10);
	memcpy(dev -> latitude_dir, &msg[29], 1);
	memcpy(dev -> longitude, &msg[31], 10);
	memcpy(dev -> longitude_dir, &msg[42], 1);
	memcpy(dev -> altitude, &msg[54], 6);
	memcpy(dev -> altitude_meas_unit, &msg[61], 1);

	return result;
}

uint8_t parse_gll_message(gnss_t *dev, uint8_t *msg) {

	uint8_t result = HAL_ERROR;

	/* $GPGLL,<Lat>,<N/S>,<Long>,<E/W>,<Timestamp>,<Status>,<mode indicator>*<checksum><cr><lf> */
	/* example: $GPGLL,4055.04673,N,01416.54941,E,110505.000,A,A*54 */

	if (strncmp((const char *)msg, (const char *)GLL, strlen(GLL)) == 0) {
		result = HAL_OK;
	} else {
		return result;
	}

	/* still to understand whether the latitude and longitude return a string of 10 characters or 11, in the case below
	 * 10 character is assumed */

	memcpy(dev -> latitude, &msg[7], 10);
	memcpy(dev -> latitude_dir, &msg[18], 1);
	memcpy(dev -> longitude, &msg[20], 10);
	memcpy(dev -> longitude_dir, &msg[31], 1);
	memcpy(dev -> utc_time, &msg[33], 10);

	return result;
}

uint8_t assemble_gnss_packet(gnss_t *dev, uint8_t *buf) {

	uint8_t result = HAL_ERROR;

	const uint8_t time[6] = "TIME:";
	const uint8_t lat[11] = ",LATITUDE:";
	const uint8_t lon[12] = ",LONGITUDE:";
	const uint8_t alt[11] = ",ALTITUDE:";

	strcat((char *)buf, (const char *)time);
	strcat((char *)buf, (const char *)dev -> utc_time);
	strcat((char *)buf, (const char *)lat);
	strcat((char *)buf, (const char *)dev -> latitude);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)dev -> latitude_dir);
	strcat((char *)buf, (const char *)lon);
	strcat((char *)buf, (const char *)dev -> longitude);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)dev -> longitude_dir);
	strcat((char *)buf, (const char *)alt);
	strcat((char *)buf, (const char *)dev -> altitude);

	/* packet structure */

	return result;
}



















