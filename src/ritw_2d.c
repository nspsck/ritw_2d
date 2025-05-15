#include "config.h"
#include "pico/stdlib.h"
#include <color.h>
#include <hardware/adc.h>
#include <hardware/clocks.h>
#include <hardware/flash.h>
#include <hardware/gpio.h>
#include <hardware/pll.h>
#include <hardware/spi.h>
#include <hardware/structs/clocks.h>
#include <hardware/timer.h>
#include <hardware/vreg.h>
#include <pico/time.h>
#include <renderer.h>
#include <sprite.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const uint16_t bnw[16 * 16] = {
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

const Sprite sprite = {
    .height = 16, .width = 16, .steps = 1, .sprite_array = bnw};

int main() {

  stdio_init_all();
  vreg_set_voltage(VREG_VOLTAGE_1_30);
  // Set sysclock to 250MHz and periclock to 125MHz
  set_sys_clock_hz(400 * MHZ, false);
  clock_configure(clk_peri, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                  CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 400 * MHZ,
                  200 * MHZ);

  // Turn on display
  gpio_init(PIN_TFT_VCC);
  gpio_set_dir(PIN_TFT_VCC, GPIO_OUT);
  gpio_put(PIN_TFT_VCC, 1);

  renderer_init();

  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);

  uint16_t fps = 0;
  uint32_t time_ms = us_to_ms(time_us_64());

  int dx = 4;
  int dy = 5;

  int dst_x = 0;
  int dst_y = 0;

  uint16_t color = rgb_to_swapped_565(250, 250, 0);
  renderer_clear(color);

  bool toggle = true;

  while (true) {
    if ((us_to_ms(time_us_64()) - time_ms) >= 1000) {
      printf("FPS: %d\n", fps);
      printf("Core Frequency: %d MHz\n", clock_get_hz(clk_sys) / MHZ);
      printf("SPI baudrate: %d KHz\n", spi_get_baudrate(SPI_PORT) / KHZ);
      printf("Peri Frequency: %d MHz\n", clock_get_hz(clk_peri) / MHZ);
      printf("flashclk div: %d\n", PICO_FLASH_SPI_CLKDIV);
      gpio_put(25, toggle);
      toggle = !toggle;
      fps = 0;
      time_ms = us_to_ms(time_us_64());
    }

    renderer_draw_sprite(dst_x, dst_y, &sprite, 10);
    renderer_fill_rect(dst_x, dst_y, sprite.width, sprite.height, color);

    if (dst_x >= 128 || dst_y >= 160) {
      dst_x = dst_y = 0;
    } else {
      dst_x += dx;
      dst_y += dy;
    }
    fps++;
  }
}
