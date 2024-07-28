/*
 * GNSS.c
 *
 *  Created on: Apr 14, 2024
 *      Author: tommaso
 */

#ifndef GNSS_H
#include "GNSS.h"
#endif

#define TIMESTAMP_PACKET 	"TIME:"
#define LATITUDE_PACKET 	",LATITUDE:"
#define LONGITUDE_PACKET	",LONGITUDE:"
#define ALTITUDE_PACKET		",ALTITUDE:"

static uint8_t gnss_write(gnss_t *dev, const uint8_t *buf, uint16_t timeout) {

	uint8_t result = HAL_ERROR;

	result = HAL_UART_Transmit(dev -> handler, buf, (uint16_t)sizeof(buf), (uint32_t) timeout);

	return result;
}

static uint8_t gnss_read(gnss_t *dev, uint8_t *buf, uint16_t timeout) {

	uint8_t result = HAL_ERROR;

	//TODO implement using HAL_UART_Receive
	//XXX better use tx_buf and rx_buf or just one buffer?

	if (HAL_UART_Transmit(dev -> handler, buf, (uint16_t)sizeof(buf), (uint32_t) timeout) == HAL_OK) {
		do {
			result = HAL_UART_Receive(dev -> handler, buf, (uint16_t)sizeof(buf), (uint32_t) timeout);
		} while (result != HAL_OK && result != HAL_TIMEOUT);
	}

	return result;
}

uint8_t set_parameter(gnss_t *dev, uint16_t id, uint8_t *buf) {

	uint8_t result = HAL_ERROR;
	uint8_t buf_id[7] = {0};

	sprintf(buf_id, ",3%03d,", id);

	return result;
}

uint8_t get_parameter(gnss_t *dev, CONFIG_BLOCK_TYPE type, uint16_t id) {

	uint8_t result = HAL_ERROR;



	return result;
}

uint8_t save_parameter(gnss_t *dev) {

	uint8_t result = HAL_ERROR;
//XXX	uint8_t response[17] = {0};
	uint8_t response[20] = {0};

	if (gnss_write(dev, (const uint8_t*)SAVE_SYSTEM_PARAMETERS, GNSS_TIMEOUT_MS) == HAL_OK) {
		gnss_read(dev, response, GNSS_TIMEOUT_MS);
		//XXX check result
		if (strncmp((const char *)response, (const char *)SAVE_SYS_PARAMETERS_OK, strlen(SAVE_SYS_PARAMETERS_OK)) == 0) {
			result = HAL_OK;
		}
		/*
		 * XXX Try to read the response in a SAVE_SYS_PARAMETERS_OK size fit buffer instead of an oversized buffer
		 * which can also fit SAVE_SYS_PARAMTERS_ERROR message. Then check for OK result, theoretically if the message
		 * is the error one strncmp should still be able to "detect the error".
		 */
	}

	return result;
}

uint8_t restore_factory_parameters(gnss_t *dev) {

	uint8_t result = HAL_ERROR;
	uint8_t response[20] = {0};

	if (gnss_write(dev, (const uint8_t *)RESTORE_SYSTEM_PARAMETERS, GNSS_TIMEOUT_MS) == HAL_OK) {
		gnss_read(dev, response, GNSS_TIMEOUT_MS);
		//XXX check result
		if (strncmp((const char *)response, (const char *)RESTORE_SYS_PARAMETERS_OK, strlen(RESTORE_SYS_PARAMETERS_OK)) == 0) {
			result = HAL_OK;
		}
		/*
		 * XXX Try to read the response in a RESTORE_SYS_PARAMETERS_OK size fit buffer instead of an oversized buffer
		 * which can also fit RESTORE_SYS_PARAMETERS_ERROR message. Then check for OK result, theoretically if the message
		 * is the error one strncmp should still be able to "detect the error".
		 */

		if (result == HAL_ERROR) {
			return result;
		}

		result = HAL_ERROR;
		if (gnss_write(dev, (const uint8_t *)GPS_SYS_REBOOT, GNSS_TIMEOUT_MS) == HAL_OK) {
			result = HAL_OK;
		}
	}

	return result;
}

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

uint8_t gnss_set_nmea_messages(gnss_t *dev, uint32_t msg_mask_list_low, uint32_t msg_mask_list_high, uint8_t rate_scaler) {

	uint8_t result = HAL_ERROR;

	uint8_t listlow[9] = {0};
	uint8_t listhigh[9] = {0};
	uint8_t rate[4] = {0};
//	uint8_t response[17] = {0};
	uint8_t response[20] = {0};

	uint8_t buf[39] = {0};

	/* message synopsys
	 * $PSTMCFGMSGL,<listid>,<rate>,<listlow>,<listhigh>*<checksum><cr><lf>
	 * 						 cdb190   cdb201   cdb228
	 */

	sprintf((char *)listlow, "%08x", msg_mask_list_low);
	sprintf((char *)listhigh, "%08x", msg_mask_list_high);
	sprintf((char *)rate, "%03d", rate_scaler);

	strcat((char *)buf, (const char *)CONFIG_MESSAGE_LIST);
	strcat((char *)buf, (const char *)",0,");
	strcat((char *)buf, (const char *)rate);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)listlow);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)listhigh);
	strcat((char *)buf, (const char *)"\r\n");

	//XXX /* send the string to the module via UART */

	if (gnss_write(dev, (const uint8_t*)buf, GNSS_TIMEOUT_MS) == HAL_OK) {
		gnss_read(dev, response, GNSS_TIMEOUT_MS);

		if (strncmp((const char *)response, (const char *)MESSAGE_LIST_OK, sizeof(MESSAGE_LIST_OK)) == 0) {
			result = HAL_OK;
		}
		// check the result message $PSTMCFGMSGLOK or $PSTMCFGMSGLERROR
	}

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

	/* packet structure
	 * TIME:HHMMSS,LATITUDE:DDMM.MMMMM,D,LONGITUDE:DDMM.MMMMM,D,AAAAAA
	 */

	/* could be useful to add a termination character and also a starting character? */

	strcat((char *)buf, (const char *)TIMESTAMP_PACKET);
	strcat((char *)buf, (const char *)dev -> utc_time);
	strcat((char *)buf, (const char *)LATITUDE_PACKET);
	strcat((char *)buf, (const char *)dev -> latitude);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)dev -> latitude_dir);
	strcat((char *)buf, (const char *)LONGITUDE_PACKET);
	strcat((char *)buf, (const char *)dev -> longitude);
	strcat((char *)buf, (const char *)",");
	strcat((char *)buf, (const char *)dev -> longitude_dir);
	strcat((char *)buf, (const char *)ALTITUDE_PACKET);
	strcat((char *)buf, (const char *)dev -> altitude);

	result = HAL_OK;

	return result;
}
