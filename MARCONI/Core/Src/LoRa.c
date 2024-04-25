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

void LORA_init(LORA_t *dev, UART_HandleTypeDef *huart, GPIO_TypeDef *aux_port, GPIO_TypeDef *m0_port, GPIO_TypeDef *m1_port, uint16_t aux_pin, uint16_t m0_pin, uint16_t m1_pin) {

	dev -> handler = huart;
	dev -> aux_gpio_port = aux_port;
	dev -> m0_gpio_port = m0_port;
	dev -> m1_gpio_port = m1_port;
	dev -> aux_gpio_pin = aux_pin;
	dev -> m0_gpio_pin = m0_pin;
	dev -> m1_gpio_pin = m1_pin;
}

GPIO_PinState read_AUX_pin(LORA_t *dev) {

	return HAL_GPIO_ReadPin(dev -> aux_gpio_port, dev -> aux_gpio_pin);
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

uint8_t change_baud_rate(LORA_t *dev, LORA_BAUD_RATE baud_rate) {

	uint8_t result = HAL_ERROR;

	/* read the related register, keep the other bits and modify only the baud rate bits with proper mask */

	uint8_t rate = 0;

	if (read_register(dev, REG0, &rate, 1) == HAL_OK) {
		result = HAL_OK;

		rate &= 0x1F;
		rate |= (baud_rate << 5);

		uint8_t buf[4] = {REGISTER_WRITE_COMMAND, REG0, 1, rate};

		result = write_register(dev, buf, 4);

		// add check for response
	}

	return result;
}

uint8_t change_air_data_rate(LORA_t* dev, LORA_AIR_DATA_RATE data_rate) {

	uint8_t result = HAL_ERROR;

	uint8_t rate = 0;

	if (read_register(dev, REG0, &rate, 1) == HAL_OK) {
		result = HAL_OK;

		rate &= 0xF8;
		rate |= data_rate;

		uint8_t buf[4] = {REGISTER_WRITE_COMMAND, REG0, 1, rate};

		result = write_register(dev, buf, 4);

		// add check for response
	}

	return result;
}

uint8_t change_packet_size(LORA_t* dev, LORA_PACKET_SIZE packet_size) {

	uint8_t result = HAL_ERROR;

	uint8_t size = 0;

	if (read_register(dev, REG1, &size, 1) == HAL_OK) {
		result = HAL_OK;

		//possibly add a control that check whether the value we want to set is already set

		size &= 0x3F;
		size |= (packet_size << 6);

		uint8_t buf[4] = {REGISTER_WRITE_COMMAND, REG1, 1, size};

		result = write_register(dev, buf, 4);

		// add check for response
	}

	return result;
}

uint8_t change_transmission_power(LORA_t* dev, LORA_TRANSMISSION_POWER power) {

	uint8_t result = HAL_ERROR;

	uint8_t curr_power = 0;

	if (read_register(dev, REG1, &curr_power, 1) == HAL_OK) {
		result = HAL_OK;

		//possibly add a control that check whether the value we want to set is already set

		curr_power &= 0xFC;
		curr_power |= power;

		uint8_t buf[4] = {REGISTER_WRITE_COMMAND, REG1, 1, curr_power};

		result = write_register(dev, buf, 4);

		// add check for response
	}

	return result;
}

uint8_t change_operating_mode(LORA_t* dev, LORA_OPERATING_MODE mode) {

	uint8_t result = HAL_ERROR;

	uint8_t mode0 = mode & 0x01;
	uint8_t mode1 = mode & 0x02;

	HAL_GPIO_WritePin(dev -> m0_gpio_port, dev -> m0_gpio_pin, (mode0 == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET));
	HAL_GPIO_WritePin(dev -> m1_gpio_port, dev -> m1_gpio_pin, (mode1 == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET));

	return result;
}

uint8_t transmit_packet(LORA_t *dev, uint8_t *buf) {

	uint8_t result = HAL_ERROR;

	return result;
}

uint8_t receive_packet(LORA_t *dev, uint8_t *buf) {

	uint8_t result = HAL_ERROR;

	return result;
}
