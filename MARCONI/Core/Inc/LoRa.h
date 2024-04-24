/*
 * LoRa.h
 *
 *  Created on: Apr 14, 2024
 *      Author: tommaso
 */

#ifndef LORA_H
#define LORA_H

#include "stm32g0xx_hal.h"
#include <stdint.h>
#include "main.h"

/* 	FACTORY DEFAULT PARAMETER
 *
 * 		FREQUENCY	ADDRESS		CHANNEL		AIR_RATE		BAUD_RATE	SERIAL_FORMAT	POWER
 * 		873.125MHz	0x0000		0x17			2.4kbps			9600			8N1			30dBm
 *
 */

/* REGISTER REG0 CONFIGURATION */

/* UART Serial Port Baud Rate (bps) REG0 | b7 | b6 | b5 | */

#define BAUD_RATE_1200		0b000
#define BAUD_RATE_2400		0b001
#define BAUD_RATE_4800		0b010
#define BAUD_RATE_9600		0b011	/* DEFAULT */
#define BAUD_RATE_19200		0b100
#define BAUD_RATE_38400		0b101
#define BAUD_RATE_57600		0b110
#define BAUD_RATE_115200		0b111

/* Air Data Rate (bps) REG0 | b2 | b1 | b0 | */

#define AIR_DATA_RATE_2_4K	0b000
#define AIR_DATA_RATE_2_4K	0b001
#define AIR_DATA_RATE_2_4K	0b010	/* DEFAULT */
#define AIR_DATA_RATE_4_8K	0b011
#define AIR_DATA_RATE_9_6K	0b100
#define AIR_DATA_RATE_19_2K	0b101
#define AIR_DATA_RATE_38_4K	0b110
#define AIR_DATA_RATE_62_5K	0b111



/* REGISTER REG1 CONFIGURATION */

/* Sub-Packet Setting REG1 | b7 | b6 | */

#define PACKET_SIZE_200_BYTES	0b00		/* DEFAULT */
#define PACKET_SIZE_128_BYTES	0b01
#define PACKET_SIZE_64_BYTES		0b10
#define PACKET_SIZE_32_BYTES		0b11

/* Transmitting Power REG1 | b1 | b0 | */

#define TRANSMISSION_POWER_30dBm		0b00		/* DEFAULT */
#define TRANSMISSION_POWER_27dBm		0b01
#define TRANSMISSION_POWER_24dBm		0b10
#define TRANSMISSION_POWER_21dBm		0b11



/* REGISTER REG3 CONFIGURATION */

/* Enable RSSI Byte REG3 | b7 | */

#define RSSI_BYTE_DISABLE		0x0		/* DEFAULT */
#define RSSI_BYTE_ENABLE			0x1

/* Transmission Method REG3 | b6 | */
#define TRANSMISSION_METHOD_TRANSPARENT		0x0		/* DEFAULT */
#define TRANSMISSION_METHOD_FIXED			0x1		/* During fixed transmission,
													   the module recognizes the first 3 bytes of serial data as:
													   address high + address low + channel,
													   and use it as a wireless transmission target */


#define WRONG_FORMAT_RESPONDS	0XFFFFFF
#define LORA_TIMEOUT_MS			100

#define MODE_NORMAL				0b00
#define MODE_SENDING			0b01
#define MODE_RECEIVING			0b10
#define MODE_DEEP_SLEEP			0b11

#define FREQUENCY_BASE			850.125f
#define FREQUENCY_DEFAULT		873.125f
#define FREQUENCY_MAX			930.125f

#define REGISTER_WRITE_COMMAND	0xC0
#define REGISTER_READ_COMMAND	0xC1

/* REGISTER DESCRIPTION */
#define ADDRESS_HIGH	0x0
#define ADDRESS_LOW		0x1
#define REG0			0x2
#define REG1			0x3
#define REG2			0x4
#define REG3			0x5
#define CRYPTO_HIGH		0x6
#define CRYPTO_LOW		0x7

typedef struct {

	UART_HandleTypeDef *handler;
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	// read and write function

} LORA_t;

/*
 * 	RESET FLOW
 *
 * 		When the module is powered, AUX pin outputs low level immediately,
 * 		conducts hardware self-check and sets the operating mode based on M0 and M1 pins.
 * 		During ALL the process, AUX remains low level. As soon as the process is completed,
 * 		the AUX pin outputs high level and the module starts to work according to the operating mode.
 * 		Therefore before the module starts to work normally, the user need to wait rising edge of AUX pin.
 */

/*
 * 	SELF-CHECK PROCEDURE
 *
 * 		Self-check procedure happens only when the procedure of power-on reset,
 * 		i.e. after a reset or when exiting from a deep sleep mode.
 */

void LORA_init(LORA_t *dev, UART_HandleTypeDef *huart, GPIO_TypeDef *port, uint16_t pin);

uint8_t switch_frequency(LORA_t *dev, float frequency);

#endif /* INC_LORA_H_ */
