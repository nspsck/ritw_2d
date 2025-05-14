#include "config.h"
#include "pico/stdlib.h"
#include <hardware/clocks.h>
#include <hardware/flash.h>
#include <hardware/gpio.h>
#include <hardware/pll.h>
#include <hardware/spi.h>
#include <hardware/timer.h>
#include <pico/time.h>
#include <renderer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h> // For memset, memcpy if needed

uint16_t sprite[16 * 16] = {
    0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
    0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000,
    0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0x0000,
    0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
    0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000,
    0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
    0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000,
    0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
    0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000,
    0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
    0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000,
    0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
    0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000,
    0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF};

int main() {
  // Set sysclock to 250MHz and periclock to 125MHz
  set_sys_clock_hz(250 * MHZ, false);
  clock_configure(clk_peri, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                  CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 250 * MHZ,
                  125 * MHZ);

  stdio_init_all();

  // Turn on display
  gpio_init(PIN_TFT_VCC);
  gpio_set_dir(PIN_TFT_VCC, GPIO_OUT);
  gpio_put(PIN_TFT_VCC, 1);

  renderer_init();

  uint16_t fps = 0;
  uint32_t time_ms = us_to_ms(time_us_64());

  int dx = 4;
  int dy = 5;

  int dst_x = 0;
  int dst_y = 0;

  renderer_clear(0x0ff0);

  while (true) {
    if ((us_to_ms(time_us_64()) - time_ms) >= 1000) {
      printf("FPS: %d\n", fps);
      printf("Core Frequency: %d MHz\n", clock_get_hz(clk_sys) / MHZ);
      printf("SPI baudrate: %d KHz\n", spi_get_baudrate(SPI_PORT) / KHZ);
      fps = 0;
      time_ms = us_to_ms(time_us_64());
    }

    renderer_blit(dst_x, dst_y, sprite, 16, 16);
    sleep_ms(100);
    renderer_fill_rect(dst_x, dst_y, 16, 16, 0x0ff0);
    dst_x += dx;
    dst_y += dy;

    if (dst_x > 120) {
      dst_x = 0;
    }
    if (dst_y > 150) {
      dst_y = 0;
    }

    fps++;
  }
}
