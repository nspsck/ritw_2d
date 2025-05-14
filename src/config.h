/**
 * This file should be imported by all of the drivers' .h file
 */
#ifndef CONFIG_H
#define CONFIG_H

#define DRIVER_ST7789

// SPI Defines
#define SPI_PORT spi0
#define PIN_SCK 18
#define PIN_TX 19
#define PIN_CS 17
#define PIN_DC 16
#define PIN_RST 20

// Display Defines
#define PIN_BL null
#define PIN_TFT_VCC 21
#define DISPLAY_WIDTH 132
#define DISPLAY_HEIGHT 162
#define X_OFFSET 2
#define Y_OFFSET 1
#endif
