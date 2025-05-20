#ifndef DRIVER_SELECT_H
#define DRIVER_SELECT_H

#include <stddef.h>
#include <stdint.h>

/**
 * Every driver should provide the following functions:
 *
 * display_init(void): go through necessary steps to set up the display.
 *
 * display_set_area(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1): allow
 * the renderer to set a specific area to draw to.
 *
 * display_write_cmd(uint8_t cmd): write command to display to control display's
 * hardware specific features.
 *
 * display_write_data(const uint8_t *data, size_t len): pass cmd (command) data
 * to the display.
 *
 * display_write_buffer(const uint16_t *data, size_t len): write color data to
 * the display.
 *
 * display_clear(uint16_t color): wipe all content of the display with given
 * color.
 *
 * display_fill_color(uint16_t color, size_t len): used for drawing functions,
 * allow effecient color-filling without given buffer storing color data.
 */

#ifdef DRIVER_ST7789
#include "st7789.h"

static inline void display_init(void) { st7789_init(); }

static inline void display_set_area(uint16_t x0, uint16_t y0, uint16_t x1,
                                    uint16_t y1) {
  st7789_set_area(x0, y0, x1, y1);
}

static inline void display_write_cmd(uint8_t cmd) { st7789_write_cmd(cmd); }

static inline void display_write_data(const uint8_t *data, size_t len) {
  st7789_write_data(data, len);
}

static inline void display_write_buffer(const uint16_t *data, size_t len) {
  st7789_write_buffer(data, len);
}

static inline void display_clear(uint16_t color) { st7789_clear(color); }

static inline void display_fill_color(uint16_t color, size_t len) {
  st7789_fill_color(color, len);
}
#endif // DRIVER_ST7789

#endif // DRIVER_SELECT_H
