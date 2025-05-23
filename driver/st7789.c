#include "st7789.h"

/* FPS does not seem to increase anymore for pi pico */
#define BUFFER_SIZE 256

static inline void dc_select() { gpio_put(PIN_DC, 1); }
static inline void dc_deselect() { gpio_put(PIN_DC, 0); }
static inline void cs_select() { gpio_put(PIN_CS, 0); }
static inline void cs_deselect() { gpio_put(PIN_CS, 1); }

void st7789_write_cmd(uint8_t cmd) {
  dc_deselect();
  cs_select();
  spi_write_blocking(SPI_PORT, &cmd, 1);
  cs_deselect();
}

void st7789_write_data(const uint8_t *data, size_t len) {
  dc_select();
  cs_select();
  spi_write_blocking(SPI_PORT, data, len);
  cs_deselect();
}

void st7789_write_buffer(const uint16_t *data, size_t len) {
  dc_select();
  cs_select();
  spi_write_blocking(SPI_PORT, (uint8_t *)data, len * 2);
  cs_deselect();
}

void st7789_set_area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  uint8_t data[] = {x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF};
  st7789_write_cmd(0x2A); // Column addr set (CASET)
  st7789_write_data(data, 4);

  data[0] = y0 >> 8;
  data[1] = y0 & 0xFF;
  data[2] = y1 >> 8;
  data[3] = y1 & 0xFF;
  st7789_write_cmd(0x2B); // Row addr set (RASET)
  st7789_write_data(data, 4);

  st7789_write_cmd(0x2C); // Write to RAM
}

void st7789_clear(uint16_t color) {
  st7789_set_area(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);
  st7789_fill_color(color, ST7789_WIDTH * ST7789_HEIGHT);
}

void st7789_fill_color(uint16_t color, size_t len) {
  uint16_t buffer[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = color;
  }
  while (len > 0) {
    int chunk = len > BUFFER_SIZE ? BUFFER_SIZE : len;
    st7789_write_buffer(buffer, chunk);
    len -= chunk;
  }
}

void st7789_init(void) {
  gpio_init(PIN_DC);
  gpio_set_dir(PIN_DC, GPIO_OUT);
  gpio_init(PIN_CS);
  gpio_set_dir(PIN_CS, GPIO_OUT);
  gpio_init(PIN_RST);
  gpio_set_dir(PIN_RST, GPIO_OUT);

  spi_init(SPI_PORT, 62.5 * 1000 * 1000);
  gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
  gpio_set_function(PIN_TX, GPIO_FUNC_SPI);

  gpio_put(PIN_RST, 0);
  sleep_ms(50);
  gpio_put(PIN_RST, 1);
  sleep_ms(50);

  cs_deselect();

  // Initialization sequence
  st7789_write_cmd(0x01); // Software reset
  sleep_ms(150);
  st7789_write_cmd(0x11); // Sleep out
  sleep_ms(150);
  st7789_write_cmd(0x3A);
  uint8_t colmod = 0x55;
  st7789_write_data(&colmod, 1); // 16-bit color
  st7789_write_cmd(0x36);
  uint8_t madctl = 0x70;
  st7789_write_data(&madctl, 1); // Memory access control
  st7789_write_cmd(0x21);        // Inversion ON
  st7789_write_cmd(0x29);        // Display on
}
