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
 * 		FREQUENCY		ADDRESS		CHANNEL		AIR_RATE		BAUD_RATE	SERIAL_FORMAT	POWER
 * 		873.125MHz		0x0000		0x17		2.4kbps			9600			8N1			30dBm
 *
 */

/* REGISTER REG0 CONFIGURATION */

/* UART Serial Port Baud Rate (bps) REG0 | b7 | b6 | b5 | */

typedef enum {
	BAUD_RATE_1200 = 0b000,
	BAUD_RATE_2400 = 0b001,
	BAUD_RATE_4800 = 0b010,
	BAUD_RATE_9600 = 0b011,	/* DEFAULT */
	BAUD_RATE_19200	= 0b100,
	BAUD_RATE_38400	= 0b101,
	BAUD_RATE_57600	= 0b110,
	BAUD_RATE_115200 = 0b111
} LORA_BAUD_RATE;

/* Air Data Rate (bps) REG0 | b2 | b1 | b0 | */

typedef enum {
	AIR_DATA_RATE_2_4K_0 = 0b000,
	AIR_DATA_RATE_2_4K_1 = 0b001,
	AIR_DATA_RATE_2_4K_2 = 0b010,	/* DEFAULT */
	AIR_DATA_RATE_4_8K = 0b011,
	AIR_DATA_RATE_9_6K = 0b100,
	AIR_DATA_RATE_19_2K = 0b101,
	AIR_DATA_RATE_38_4K = 0b110,
	AIR_DATA_RATE_62_5K = 0b111
} LORA_AIR_DATA_RATE;

/* REGISTER REG1 CONFIGURATION */

/* Sub-Packet Setting REG1 | b7 | b6 | */

typedef enum {
	PACKET_SIZE_200_BYTES = 0b00,		/* DEFAULT */
	PACKET_SIZE_128_BYTES = 0b01,
	PACKET_SIZE_64_BYTES = 0b10,
	PACKET_SIZE_32_BYTES = 0b11
} LORA_PACKET_SIZE;

/* Transmitting Power REG1 | b1 | b0 | */

typedef enum {
	TRANSMISSION_POWER_30dBm = 0b00,		/* DEFAULT */
	TRANSMISSION_POWER_27dBm = 0b01,
	TRANSMISSION_POWER_24dBm = 0b10,
	TRANSMISSION_POWER_21dBm = 0b11
} LORA_TRANSMISSION_POWER;

/* REGISTER REG3 CONFIGURATION */

/* Enable RSSI Byte REG3 | b7 | */

#define RSSI_BYTE_DISABLE			0x0		/* DEFAULT */
#define RSSI_BYTE_ENABLE			0x1

/* Transmission Method REG3 | b6 | */
#define TRANSMISSION_METHOD_TRANSPARENT		0x0		/* DEFAULT */
#define TRANSMISSION_METHOD_FIXED			0x1		/* During fixed transmission,
													   the module recognizes the first 3 bytes of serial data as:
													   address high + address low + channel,
													   and use it as a wireless transmission target */


#define WRONG_FORMAT_RESPONDS	0XFFFFFF
#define LORA_TIMEOUT_MS			100

typedef enum {
	MODE_NORMAL	= 0b00,
	MODE_SENDING = 0b01,
	MODE_RECEIVING = 0b10,
	MODE_DEEP_SLEEP	= 0b11
} LORA_OPERATING_MODE;

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
	GPIO_TypeDef *aux_gpio_port;
	GPIO_TypeDef *m0_gpio_port;
	GPIO_TypeDef *m1_gpio_port;
	uint16_t aux_gpio_pin;
	uint16_t m0_gpio_pin;
	uint16_t m1_gpio_pin;

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

void LORA_init(LORA_t *dev, UART_HandleTypeDef *huart, GPIO_TypeDef *aux_port, GPIO_TypeDef *m0_port, GPIO_TypeDef *m1_port, uint16_t aux_pin, uint16_t m0_pin, uint16_t m1_pin);

GPIO_PinState read_AUX_pin(LORA_t *dev);

uint8_t switch_frequency(LORA_t *dev, float frequency);

uint8_t change_baud_rate(LORA_t *dev, LORA_BAUD_RATE baud_rate);

uint8_t change_air_data_rate(LORA_t* dev, LORA_AIR_DATA_RATE data_rate);

uint8_t change_packet_size(LORA_t* dev, LORA_PACKET_SIZE packet_size);

uint8_t change_transmission_power(LORA_t* dev, LORA_TRANSMISSION_POWER power);

uint8_t change_operating_mode(LORA_t* dev, LORA_OPERATING_MODE mode);

uint8_t transmit_packet(LORA_t *dev, uint8_t *buf);

uint8_t receive_packet(LORA_t *dev, uint8_t *buf);

#endif /* LORA_H */
