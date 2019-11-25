#pragma once

#include "stm32f1xx_hal.h"
#include<stdbool.h>

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2
#define LCD_K  3

typedef struct {

	uint8_t addres, cols, rows;
	uint8_t row_offsets[4];
	uint32_t timeout;
	I2C_HandleTypeDef* i2c;


} i2c_lcd_config;

typedef struct {

	i2c_lcd_config config;
	uint8_t universal[1],
			entry_mode[1],
			display_control[1],
			function_set[1];

} i2c_lcd;

typedef enum {
	LCD_OK,
	LCD_ERROR
} LCD_STATE;

//This function is for inizialitate the instance
LCD_STATE i2c_lcd_init(i2c_lcd* dev, i2c_lcd_config* config);

//This function print the message contains in s
void i2c_lcd_print(i2c_lcd* dev, char* s, uint8_t size);

//This function turn on or turn off the back ligth
void i2c_lcd_bl(i2c_lcd* dev, bool state);

//This function is command home, clear and set cursor on 0,0
void i2c_lcd_home(i2c_lcd* dev);

//Just return the set cursor on 0,0
void i2c_lcd_return(i2c_lcd* dev);

//With this you can show or hide the blink
void i2c_lcd_bk(i2c_lcd* dev, bool state);

//With this you can show or hide the cursor
void i2c_lcd_cursor(i2c_lcd* dev, bool state);

//This function show or hide the information on display, but the information is saved while the module is powered
void i2c_lcd_display(i2c_lcd* dev, bool state);

//really i dont know, i dont use this function, just i make it because i see in the datasheet XD
void i2c_lcd_shift(i2c_lcd* dev, bool state);

//This function move the cursor to the column and row that you give it
void i2c_lcd_setCursor(i2c_lcd* dev, uint8_t col, uint8_t row);
