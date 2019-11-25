//Just you have to create the instances i2c_lcd and i2c_lcd_config

  i2c_lcd lcd;

  i2c_lcd_config config;

  {

  	//You must do this settings, the most common addres on the modules is 0x27

	  config.addres = 0x27;
	  config.i2c = &hi2c2; //This is the Handler of the interface i2c
	  config.timeout = 100;
	  config.rows = 2;
	  config.cols = 16;

  }

// finally you have inizialite the instance i2c_lcd

  i2c_lcd_init(&lcd, &config); //Also this function return a value (LCD_OK or LCD_ERROR) for veriify the inizialitation


  // and you can print mmessages on the lcd

  char message[11] = "Hello world";
  uint8_t size = sizeof(message);

  i2c_lcd_print(&lcd, message, size);

  //Thank you for watching and i hope this will be useful for you...



