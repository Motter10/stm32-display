/*
 * LCD_4.h
 *
 *  Created on: Oct 27, 2020
 *      Author: sandro
 */

#ifndef INC_LCD_4_H_
#define INC_LCD_4_H_

#include <stdint.h>
#include <stdbool.h>


#include "stm32f1xx_hal.h"

#define CTRL_PINS GPIOC
#define RS GPIO_PIN_0
#define RW GPIO_PIN_1
#define EN GPIO_PIN_2

#define DATA_PINS GPIOA
#define FRST_PIN GPIO_PIN_4

void LCD_Send();

void LCD_Write(uint8_t data, bool is_data);

void LCD_Write_Number(int16_t);

void LCD_Write_Buffer(uint8_t* data);

void LCD_Create_Custom_Char(uint8_t loc, uint8_t *msg);

void LCD_Init_Custom_Characters();

void LCD_Init();

#endif /* INC_LCD_4_H_ */
