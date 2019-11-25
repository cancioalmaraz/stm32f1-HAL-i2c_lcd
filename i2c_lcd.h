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

LCD_STATE i2c_lcd_init(i2c_lcd* dev, i2c_lcd_config* config);

void i2c_lcd_print(i2c_lcd* dev, char* s, uint8_t size);

void i2c_lcd_bl(i2c_lcd* dev, bool state);

void i2c_lcd_home(i2c_lcd* dev);

void i2c_lcd_return(i2c_lcd* dev);

void i2c_lcd_bk(i2c_lcd* dev, bool state);

void i2c_lcd_cursor(i2c_lcd* dev, bool state);

void i2c_lcd_display(i2c_lcd* dev, bool state);

void i2c_lcd_shift(i2c_lcd* dev, bool state);

void i2c_lcd_setCursor(i2c_lcd* dev, uint8_t col, uint8_t row);
