#include "i2c_lcd.h"

static void en_high(i2c_lcd* dev){
	dev->universal[0] |= (1<<LCD_EN);
	HAL_I2C_Master_Transmit(dev->config.i2c, (dev->config.addres)<<1, dev->universal, 1, dev->config.timeout);
}

static void en_low(i2c_lcd* dev){
	dev->universal[0] &= ~(1<<LCD_EN);
	HAL_I2C_Master_Transmit(dev->config.i2c, (dev->config.addres)<<1, dev->universal, 1, dev->config.timeout);

}

static void rs_high(i2c_lcd* dev){
	dev->universal[0] |= (1<<LCD_RS);
	HAL_I2C_Master_Transmit(dev->config.i2c, (dev->config.addres)<<1, dev->universal, 1, dev->config.timeout);
}

static void rs_low(i2c_lcd* dev){
	dev->universal[0] &= ~(1<<LCD_RS);
	HAL_I2C_Master_Transmit(dev->config.i2c, (dev->config.addres)<<1, dev->universal, 1, dev->config.timeout);
}

static void pulse_en(i2c_lcd* dev){

	en_high(dev);
	HAL_Delay(1);
	en_low(dev);
	HAL_Delay(1);

}

static void write(i2c_lcd* dev, uint8_t data){

	uint8_t mask = 0b00001111;
	dev->universal[0] &= mask;
	dev->universal[0] |= (data<<4);
	HAL_I2C_Master_Transmit(dev->config.i2c, (dev->config.addres)<<1, dev->universal, 1, dev->config.timeout);

	pulse_en(dev);

}

static void putChar(i2c_lcd* dev, char s){
	
	rs_high(dev);
	
	write(dev, s>>4);
	write(dev, s);
	
	rs_low(dev);
		
}

static void i2c_lcd_cmd(i2c_lcd* dev, uint8_t cmd){

	write(dev, cmd>>4);
	write(dev, cmd);

}

LCD_STATE i2c_lcd_init(i2c_lcd* dev, i2c_lcd_config* config){
	
	dev->config = *config;

	LCD_STATE state = LCD_ERROR;

	if (HAL_I2C_IsDeviceReady(dev->config.i2c, (dev->config.addres)<<1, 10, dev->config.timeout) == HAL_OK){
		state = LCD_OK;
	}

	dev->universal[0] = 0x00;
	dev->display_control[0] = 0x0F;
	dev->entry_mode[0] = 0x05;
	dev->function_set[0] = 0x20;

	dev->config.row_offsets[0] = 0x00;
	dev->config.row_offsets[1] = 0x40;
	dev->config.row_offsets[2] = 0x00 + dev->config.rows;
	dev->config.row_offsets[3] = 0x40 + dev->config.rows;


	HAL_I2C_Master_Transmit(dev->config.i2c, (dev->config.addres)<<1, dev->universal, 1, dev->config.timeout);

	HAL_Delay(50);

	i2c_lcd_bl(dev, true);

	write(dev, 0x03);
	HAL_Delay(5);
	write(dev, 0x03);
	HAL_Delay(5);
	write(dev, 0x03);
	HAL_Delay(1);
	write(dev, 0x02);

	if (dev->config.rows>1){
		dev->function_set[0] |= (1<<3);
		i2c_lcd_cmd(dev, *dev->function_set);
	}
	i2c_lcd_display(dev, true);
	i2c_lcd_cursor(dev, true);
	i2c_lcd_bk(dev, false);
	i2c_lcd_home(dev);

	return state;

}

void i2c_lcd_print(i2c_lcd* dev, char *s, uint8_t size){

	while(size>0){
		putChar(dev, *s++);
		size--;
	}

}

void i2c_lcd_bl(i2c_lcd* dev, bool state){

	if (state){
		dev->universal[0] |= (1<<LCD_K);
	}
	else{
		dev->universal[0] &= ~(1<<LCD_K);
	}
	HAL_I2C_Master_Transmit(dev->config.i2c, (dev->config.addres)<<1, dev->universal, 1, dev->config.timeout);

}

void i2c_lcd_home(i2c_lcd* dev){
	i2c_lcd_cmd(dev, 1);
}

void i2c_lcd_return(i2c_lcd* dev){
	uint8_t cmd = 0x02;
	i2c_lcd_cmd(dev, cmd);
}

void i2c_lcd_bk(i2c_lcd* dev, bool state){
	if (state){
		dev->display_control[0] |= 1;
	}
	else{
		dev->display_control[0] &= ~1;
	}
	i2c_lcd_cmd(dev, *dev->display_control);
}

void i2c_lcd_cursor(i2c_lcd* dev, bool state){
	if (state){
		dev->display_control[0] |= (1<<1);
	}
	else{
		dev->display_control[0] &= ~(1<<1);
	}
	i2c_lcd_cmd(dev, *dev->display_control);
}

void i2c_lcd_display(i2c_lcd* dev, bool state){
	if (state){
		dev->display_control[0] |= (1<<2);
	}
	else{
		dev->display_control[0] &= ~(1<<2);
	}
	i2c_lcd_cmd(dev, *dev->display_control);
}

void i2c_lcd_shift(i2c_lcd* dev, bool state){
	if (state){
		dev->entry_mode[0] |= (1<<0);
	}
	else{
		dev->entry_mode[0] &= ~(1<<0);
	}
	i2c_lcd_cmd(dev, *dev->entry_mode);
}

void i2c_lcd_setCursor(i2c_lcd* dev, uint8_t col, uint8_t row){
	const size_t max_lines = sizeof(dev->config.row_offsets) / sizeof(*dev->config.row_offsets);
	if (row >= max_lines) {
	row = max_lines - 1;
	}
	if (row >= dev->config.rows) {
	row = dev->config.rows - 1;
	}
	i2c_lcd_cmd(dev, 0x80 | (col + dev->config.row_offsets[row]));
}
