/*
 * GNSS.h
 *
 *  Created on: Apr 14, 2024
 *      Author: tommaso
 */

/* USER MANUAL AT: https://www.st.com/resource/en/user_manual/um2229-teseoliv3-gnss-module--software-manual-stmicroelectronics.pdf */

#ifndef GNSS_H
#define GNSS_H

#include <stdint.h>
#include "stm32g0xx.h"
#include "main.h"

#define GPS_TIME_POS_INIT			"$PSTMINITGPS"
#define GPS_UTC_TIME_INIT			"$PSTMINITTIME"

#define GPS_COLD_START				"$PSTMCOLD"
#define GPS_WARM_START				"$PSTMWARM"
#define GPS_HOT_START				"$PSTMHOT"
#define GPS_SYS_RESET				"$PSTMSRR"
#define GPS_ENGINE_RESET				"$PSTMGPSRESET"
#define GPS_ENGINE_SUSPEND			"$PSTMGPSSUSPEND"
#define GPS_ENGINE_RESTART			"$PSTMGPSRESTART"

#define SET_SYSTEM_PARAMETERS		"$PSTMSETPAR"
#define GET_SYSTEM_PARAMETERS		"$PSTMGETPART"
#define SAVE_SYSTEM_PARAMETERS		"$PSTMSAVEPAR"
#define RESTORE_SYSTEM_PARAMETERS	"$PSTMRESTOREPAR"
#define SEND_NMEA_MESSAGES			"$PSTMNMEAREQUEST"

#define RF_TEST_MODE_ON				"$PSTMRFTESTON"
#define RF_TEST_MODE_OFF				"$PSTMRFTESTOFF"

//XXX /* page 181 for NMEA Port Baudrate settings */


/* STANDARD NMEA MESSAGE TALKER ID */
#define GNS		"--GNS"		/* default: ON  | global position system FIX data */
#define GGA		"GPGGA"		/* default: ON  | global position system FIX data */
#define GLL		"GPGLL"		/* default: ON  | geographic position Latitude/Longitude */
#define GSA		"--GSA"		/* default: ON  | GPS DOP and active satellites. "GP", "GL", and "GN" talker id are supported according to the software configuration */
#define GSV		"--GSV"		/* default: ON  | GPS satellites in view. "GP", "GL", and "GN" talker id are supported according to the software configuration */
#define RMC		"GPRMC"		/* default: ON  | Recommended minimum specific GNSS data */
#define ZDA		"GPZDA"		/* default: OFF | time and date */

#endif /* GNSS_H */
