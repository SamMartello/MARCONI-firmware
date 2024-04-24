/*
 * LoRa.c
 *
 *  Created on: Apr 14, 2024
 *      Author: tommaso
 */

#ifndef LORA_H
#include "LoRa.h"
#endif

/* TRANSMISSION MODES
 *
 * 		Target Address 2 bytes 	:= ADDR
 * 		Target Channel 1 byte 	:= CH
 *
 * 		Fixed Transmission --> ADDR:CH:DATA --> only the device which matches address and channel receive the data
 *
 * 		Broadcasting Transmission --> FFFF:CH:DATA --> all the devices which match the channel receive the data
 *
 * 		If the module's address is set to 0xFFFF then it will receive data from all the devices which transmit on its channel
 */

static uint8_t read_register(LORA_t *dev, uint8_t reg_addr, uint8_t *buf, uint8_t len) {

	uint8_t result = HAL_ERROR;

	/* Read Register
		 * Command: C1 + starting address + length
		 * Response: C1 + starting address + length + parameters
	*/

	uint8_t address[3] = {0xC1, 0x0, 0x0};
	address[2] = reg_addr;
	address[3] = len;

	if (HAL_UART_Transmit(dev -> handler, address, sizeof(address) / sizeof(uint8_t), LORA_TIMEOUT_MS) == HAL_OK) {
		if (HAL_UART_Receive(dev -> handler, buf, (sizeof(address) / sizeof(uint8_t)) + len, LORA_TIMEOUT_MS) == HAL_OK) {
			result = HAL_OK;
		}
	}

	return result;
}

//FIXME check if length is correct
static uint8_t write_register(LORA_t *dev, uint8_t *buf, uint8_t len) {

	uint8_t result = HAL_ERROR;

	/* Write Register
		 * Command: C0 + starting address + length + parameters
		 * Response: C1 + starting address + length + parameters
	*/

	if (HAL_UART_Transmit(dev -> handler, buf, len, LORA_TIMEOUT_MS) == HAL_OK) {
		if (HAL_UART_Receive(dev -> handler, buf, len, LORA_TIMEOUT_MS) == HAL_OK) {
			result = HAL_OK;
		}
	}

	return result;
}

void LORA_init(LORA_t *dev, UART_HandleTypeDef *huart, GPIO_TypeDef *port, uint16_t pin) {

	dev -> handler = huart;
	dev -> gpio_port = port;
	dev -> gpio_pin = pin;
}

static GPIO_PinState read_AUX_pin(LORA_t *dev) {

	return HAL_GPIO_ReadPin(dev -> gpio_port, dev -> gpio_pin);
}

uint8_t switch_frequency(LORA_t *dev, float freq) {

	uint8_t result = HAL_ERROR;

	/* set the default value of channel control wrt the default frequency of the module */
	uint8_t channel_control = FREQUENCY_DEFAULT - FREQUENCY_BASE;

	/* check on frequency boundaries */
	/* we allow only a frequency range from 850.125 MHz to 930.125 MHz */
	if (freq < FREQUENCY_DEFAULT) {
		channel_control = 0;
	} else if (freq > FREQUENCY_MAX) {
		channel_control = 80;
	} else {
		channel_control = freq - FREQUENCY_BASE;
	}

	/* composition of packet to be sent to write_register function to actually set the new frequency */
	uint8_t buf[4] = {REGISTER_WRITE_COMMAND, REG2, 1, channel_control};

	result = write_register(dev, buf, 4);

	/* check on the result of the write_register function */
	if (result == HAL_OK) {
		/* if HAL_OK we check whether the LoRa module answers with a WRONG_FORMAT_RESPONSE */
		if (buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFF) {
			result = HAL_ERROR;
		}
		/* otherwise we check that everything went good */
		else if (buf[0] == REGISTER_READ_COMMAND && buf[1] == REG2 && buf[2] == 1 && buf[3] == channel_control) {
			result = HAL_OK;
		}
	}

	return result;
}
