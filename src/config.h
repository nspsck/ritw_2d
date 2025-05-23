/**
 * This file should be imported by all of the drivers' .h file
 */
#ifndef CONFIG_H
#define CONFIG_H

#define DRIVER_ST7789

// SPI Defines
#define SPI_PORT spi1
#define PIN_SCK 11
#define PIN_TX 10
#define PIN_CS 8
#define PIN_DC 25
#define PIN_RST 27

// Display Defines
#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 240
#define X_OFFSET 0
#define Y_OFFSET 0

#endif
