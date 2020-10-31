/*
 * LCD_4.c
 *
 *  Created on: October 27, 2020
 *      Author: Sandro Motter
 */
#include "LCD_4.h"

/**
 * @brief envia byte para LCD
 *
 * Aciona pino enable, espera 15ms, desaciona pino enable
 *
 * @retval Nenhum
 */
void LCD_Send() {
	HAL_GPIO_WritePin(CTRL_PINS, EN, 1);
	HAL_Delay(15);
	HAL_GPIO_WritePin(CTRL_PINS, EN, 0);
}

/**
 * @brief monta bytes para serem enviados ao LCD
 *
 * Primeiro: monta nibble mais significativo, envia com LCD_Send()
 * Segundo: monta nibble menos significativo, envia com LCD_Send()
 * Byte pode ser dado ou instrução
 *
 * @retval Nenhum
 * @param[in] data byte a ser enviado
 * @param[in] is_data 1 se for dado, 0 se for instrução
 */
void LCD_Write(uint8_t data, bool is_data) {
	HAL_GPIO_WritePin(CTRL_PINS, RS, is_data);  //instrução / data
	HAL_GPIO_WritePin(CTRL_PINS, RW, 0);		//escrita

	for (uint8_t i = 0; i < 4; i++) //nibble mais significativo
			{
		HAL_GPIO_WritePin(DATA_PINS, (uint16_t) FRST_PIN << i,
				data & (1 << (i + 4)));
	}

	LCD_Send();

	for (uint8_t i = 0; i < 4; i++) //nibble menos significativo
			{
		HAL_GPIO_WritePin(DATA_PINS, (uint16_t) FRST_PIN << i, data & (1 << i));
	}

	LCD_Send();
}

/**
 * @brief envia string ao display
 *
 *
 * @retval Nenhum
 * @param[in] *data string a ser enviado
 */
void LCD_Write_Buffer(uint8_t *data) {
	for (uint8_t i = 0; i < *data!= '\0'; ++i) {
		if(i == 17)
		{
			LCD_Write(0xC0, 0);
		}
		//caracter espciais
		switch (*data) {
			case 195:
				data++;
				switch (*data) {
					case 161:
						LCD_Write(0, 1);
						break;
					case 169:
						LCD_Write(1, 1);
						break;
					case 167:
						LCD_Write(2, 1);
						break;
					case 163:
						LCD_Write(3, 1);
						break;
					case 181:
						LCD_Write(4, 1);
						break;
				}
				data++;
				break;
			default:
				LCD_Write(*data++, 1);
				break;
		}
	}
}

/**
 * @brief envia numero inteiro de 16 bits para o display
 *
 * transforma numero em string e envia
 *
 * @retval Nenhum
 * @param[in] value valor inteiro a ser enviado para o display
 */
void LCD_Write_Number(int16_t value) {
	char str[10];
	sprintf(str, "%d", value);
	uint8_t lenght = 0;
	for (uint8_t i = 0; str[i] != '\0'; i++) {
		lenght++;
	}
	LCD_Write_Buffer(str);
}

/**
 * @brief escreve na CGRAM caracter customizado
 *
 *	escreve caracter na CGRAM a partir do endereço 0x40
 *
 * @retval Nenhum
 * @param[in] loc localização para escrever o caracter
 * @param[in] *c ponteiro para o vetor de 8 bytes do caracter
 */
void LCD_Create_Custom_Char(uint8_t loc, uint8_t *c) {
	unsigned char i;
	if (loc < 8) {
		LCD_Write(0x40 + (loc * 8), 0); /* aponta para 0x40 + loc * 8 */
		for (i = 0; i < 8; i++) /* escreve 8 bytes de cada caracter */
			LCD_Write(c[i], 1);
	}
}

/**
 * @brief Inicializa caracteres customizados
 *
 *	Define vetor de caracteres customizados e escreve na CGRAM
 *
 * @retval Nenhum
 */
void LCD_Init_Custom_Characters() {
	uint8_t character1[8] = { 0x02, 0x04, 0x0E, 0x01, 0x0F, 0x11, 0x0F,
			0x00 };
	uint8_t character2[8] = { 0x02, 0x04, 0x0E, 0x11, 0x1F, 0x10, 0x0E,
			0x00 };
	uint8_t character3[8] = { 0x00, 0x0E, 0x10, 0x10, 0x11, 0x0E, 0x04,
			0x00 };
	uint8_t character4[8] = { 0x1F, 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F,
			0x00 };
	uint8_t character5[8] = { 0x1F, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E,
			0x00 };
	LCD_Create_Custom_Char(0, character1);
	LCD_Create_Custom_Char(1, character2);
	LCD_Create_Custom_Char(2, character3);
	LCD_Create_Custom_Char(3, character4);
	LCD_Create_Custom_Char(4, character5);
}

/**
 * @brief Inicializa display em modo de 4 bits
 *
 *	Inicializa em modo de 4 bits
 *	2 linhas 5x7
 *	Display aceso com cursor fixo
 *	Escreve deslocando a mensagem para a esquerda
 *	Inicializa caracteres especiais
 *
 * @retval Nenhum
 */
void LCD_Init() {
	HAL_Delay(15);
	LCD_Write(0x03, 0); // 1
	LCD_Write(0x03, 0); // 2
	LCD_Write(0x03, 0); // 3
	LCD_Write(0x02, 0); // 4
	LCD_Write(0x02, 0); // 5
	LCD_Write(0x08, 0); // 6
	LCD_Write(0x00, 0); // 7
	LCD_Write(0x08, 0); // 8
	LCD_Write(0x00, 0); // 9
	LCD_Write(0x01, 0); // 10
	LCD_Write(0x00, 0); // 11
	LCD_Write(0x01, 0); // 12

	LCD_Write(0x28, 0); //2 linhas 5x7 (4 bits)
	LCD_Write(0x0E, 0); //Display aceso c/ cursor fixo
	LCD_Write(0x06, 0); //Escreve deslocando a mensagem para a esquerda

	LCD_Init_Custom_Characters();
}

