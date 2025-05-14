#ifndef ST7789_H
#define ST7789_H

#include "src/config.h"
#include <stddef.h>
#include <stdint.h>

#ifdef DISPLAY_WIDTH
#define ST7789_WIDTH (DISPLAY_WIDTH)
#else
#define ST7789_WIDTH 240
#endif // !ST7789_WIDTH

#ifdef DISPLAY_HEIGHT
#define ST7789_HEIGHT (DISPLAY_HEIGHT)
#else
#define ST7789_HEIGHT 240
#endif // !ST7789_HEIGHT

#ifndef SPI_PORT
#define SPI_PORT spi0
#endif /* ifndef SPI_PORT */

#ifndef PIN_SCK
#define PIN_SCK 18
#endif /* ifndef PIN_SCK */

#ifndef PIN_TX
#define PIN_TX 19
#endif /* ifndef PIN_TX */

#ifndef PIN_CS
#define PIN_CS 17
#endif /* ifndef PIN_CS */

#ifndef PIN_DC
#define PIN_DC 16
#endif /* ifndef PIN_DC */

#ifndef PIN_RST
#define PIN_RST 20
#endif /* ifndef PIN_RST */

void st7789_init(void);
void st7789_set_area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void st7789_write_cmd(uint8_t cmd);
void st7789_write_data(const uint8_t *data, size_t len);
void st7789_write_buffer(const uint16_t *data, size_t len);
void st7789_fill_color(uint16_t color, size_t len);
void st7789_clear(uint16_t color);

#endif
