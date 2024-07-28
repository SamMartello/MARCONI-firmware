/*
 * GNSS.h
 *
 *  Created on: Apr 14, 2024
 *      Author: tommaso
 */

/* USER MANUAL AT: https://www.st.com/resource/en/user_manual/um2229-teseoliv3-gnss-module--software-manual-stmicroelectronics.pdf */

#ifndef GNSS_H
#define GNSS_H

#include "main.h"

#define GNSS_TIMEOUT_MS				100

#define GPS_TIME_POS_INIT			"$PSTMINITGPS"
#define GPS_UTC_TIME_INIT			"$PSTMINITTIME"

#define GPS_COLD_START				"$PSTMCOLD"
#define GPS_WARM_START				"$PSTMWARM\r\n"
#define GPS_HOT_START				"$PSTMHOT\r\n"
#define GPS_SYS_REBOOT				"$PSTMSRR\r\n"
#define GPS_ENGINE_RESET			"$PSTMGPSRESET\r\n"
#define GPS_ENGINE_SUSPEND			"$PSTMGPSSUSPEND\r\n"
#define GPS_ENGINE_RESTART			"$PSTMGPSRESTART\r\n"

#define GET_RTC_TIME				"$PSTMGETRTCTIME\r\n"

#define SET_SYSTEM_PARAMETERS		"$PSTMSETPAR"
#define GET_SYSTEM_PARAMETERS		"$PSTMGETPART"

#define SAVE_SYSTEM_PARAMETERS		"$PSTMSAVEPAR\r\n"
#define SAVE_SYS_PARAMETERS_OK		"$PSTMSAVEPAROK\r\n"
#define SAVE_SYS_PARAMTERS_ERROR	"$PSTMSAVEPARERROR\r\n"

#define RESTORE_SYSTEM_PARAMETERS	"$PSTMRESTOREPAR\r\n"
#define RESTORE_SYS_PARAMETERS_OK	"$PSTMRESTOREPAROK\r\n"
#define RESTORE_SYS_PARAMETERS_ERR	"$PSTMRESTOREPARERROR\r\n"

/* this command allows to define the message list, so we can decide which message we want to receive from the module */
#define CONFIG_MESSAGE_LIST			"$PSTMCFGMSGL"
#define MESSAGE_LIST_OK				"$PSTMCFGMSGLOK\r\n"
#define MESSAGE_LIST_ERRROR			"$PSTMCFGMSGLERROR\r\n"

/* page 85 */
#define SET_GNSS_CONSTELLATION		"$PSTMCFGCONST"

#define SEND_NMEA_MESSAGES			"$PSTMNMEAREQUEST"

#define RF_TEST_MODE_ON				"$PSTMRFTESTON"
#define RF_TEST_MODE_OFF			"$PSTMRFTESTOFF"

//TODO implement setpar, getpar, savepar, setportcfg, resetpar functions
// see each cdb id table to see if a system reset is required or not

//XXX /* page 181 for NMEA Port Baudrate settings - CDB-ID 102 */
//XXX /* page 190 for NMEA messages list - CDB-ID 201-228 */
//XXX /* page 182 for NMEA Msg-List 0 output rate scaling factor - CDB-ID 190 */
//XXX /* page 185 for fix-rate GNSS value - CDB-ID 303 */

/* page 8, 71 for parameters changing */
/* to actually change a parameter:
 * $PSTMSETPAR		changes the parameter
 * $PSTMSAVEPAR		saves the configuration
 * $PSTMSRR			resets the module to make changes effective
 */

typedef enum {
	GNSS_BAUD_RATE_300 = 0x0,
	GNSS_BAUD_RATE_600 = 0x1,
	GNSS_BAUD_RATE_1200 = 0x2,
	GNSS_BAUD_RATE_2400 = 0x3,
	GNSS_BAUD_RATE_4800 = 0x4,
	GNSS_BAUD_RATE_9600 = 0x5,
	GNSS_BAUD_RATE_14400 = 0x6,
	GNSS_BAUD_RATE_19200 = 0x7,
	GNSS_BAUD_RATE_38400 = 0x8,
	GNSS_BAUD_RATE_57600 = 0x9,
	GNSS_BAUD_RATE_115200 = 0xA,	/* DEFAULT */
	GNSS_BAUD_RATE_230400 = 0xB,
	GNSS_BAUD_RATE_460800 = 0xC,
	GNSS_BAUD_RATE_921600 = 0xD
} GNSS_NMEA_BAUD_RATE;

typedef enum {
	GPGNS = 0x1,
	GPGGA = 0x2,
	GPGSA = 0x4,
	GPRMC = 0x40,
	GPGSV = 0x80000,
	GPGLL = 0x100000,
	GPZDA = 0x1000000
} NMEA_MESSAGE_LIST;

typedef enum {
	CURRENT_CONFIG = 0x1,
	DEFAULT_CONFIG = 0x2,
	NVM_STORED_CONFIG = 0x3
} CONFIG_BLOCK_TYPE;


/* STANDARD NMEA MESSAGE TALKER ID */
#define GNS		"--GNS"		/* default: ON  | global position system FIX data */
#define GGA		"GPGGA"		/* default: ON  | global position system FIX data */
#define GLL		"GPGLL"		/* default: ON  | geographic position Latitude/Longitude */
#define GSA		"--GSA"		/* default: ON  | GPS DOP and active satellites. "GP", "GL", and "GN" talker id are supported according to the software configuration */
#define GSV		"--GSV"		/* default: ON  | GPS satellites in view. "GP", "GL", and "GN" talker id are supported according to the software configuration */
#define RMC		"GPRMC"		/* default: ON  | Recommended minimum specific GNSS data */
#define ZDA		"GPZDA"		/* default: OFF | time and date */

typedef struct {
	UART_HandleTypeDef *handler;

	/* do we need to put string termination character \0 ? */
	/* uint8_t utc_time[11]; */ /* hour + minute + second + millisecond */
	uint8_t utc_time[7]; /* hour + minute + second */
	uint8_t latitude[11];
	uint8_t latitude_dir[2];
	uint8_t longitude[11];
	uint8_t longitude_dir[2];
	uint8_t altitude[7];
	uint8_t altitude_meas_unit[2];

	/* add write and read function */
} gnss_t;

typedef struct {
	float latitude;
	uint8_t lat_dir;
	float longitude;
	uint8_t lon_dir;
	uint16_t altitude;
	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} config_params_t;

uint8_t gnss_init(gnss_t *dev, UART_HandleTypeDef *huart);

uint8_t gnss_set_init_position_time(gnss_t *dev, config_params_t *params);

uint8_t gnss_change_baud_rate(gnss_t *dev, GNSS_NMEA_BAUD_RATE rate);

uint8_t gnss_set_nmea_messages(gnss_t *dev, uint32_t msg_mask_list_low, uint32_t msg_mask_list_high, uint8_t rate_scaler);

uint8_t parse_gga_message(gnss_t *dev, uint8_t *msg);

uint8_t parse_gll_message(gnss_t *dev, uint8_t *msg);

uint8_t assemble_gnss_packet(gnss_t *dev, uint8_t *buf);

uint8_t set_parameter(gnss_t *dev, uint16_t id, uint8_t *buf);

uint8_t get_parameter(gnss_t *dev, CONFIG_BLOCK_TYPE type, uint16_t id);

uint8_t save_parameter(gnss_t *dev);

uint8_t restore_factory_parameters(gnss_t *dev);

#endif /* GNSS_H */
