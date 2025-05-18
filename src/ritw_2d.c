#include "config.h"
#include "hardware/structs/ssi.h"
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

static void __no_inline_not_in_flash_func(set_flash_div)(int freq) {
  const int max_freq = 133 * MHZ;
  int div = (freq + max_freq - 1) / max_freq;
  if (div % 2 == 1) {
    div += 1;
  }
  while (ssi_hw->sr & SSI_SR_BUSY_BITS) {
    ;
  }
  hw_clear_bits(&ssi_hw->ssienr, SSI_SSIENR_SSI_EN_BITS);
  ssi_hw->baudr = div;
  hw_set_bits(&ssi_hw->ssienr, SSI_SSIENR_SSI_EN_BITS);
}

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

#define SYS_CLOCK_MHZ 400
#define PERI_CLOCK_MHZ 200

int main() {

  stdio_init_all();
  vreg_set_voltage(VREG_VOLTAGE_1_30);
  // Set sysclock to 250MHz and periclock to 125MHz
  set_sys_clock_hz(SYS_CLOCK_MHZ * MHZ, false);
  clock_configure(clk_peri, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                  CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                  SYS_CLOCK_MHZ * MHZ, PERI_CLOCK_MHZ * MHZ);
  set_flash_div(clk_sys);

  // Turn on display
  gpio_init(PIN_TFT_VCC);
  gpio_set_dir(PIN_TFT_VCC, GPIO_OUT);
  gpio_put(PIN_TFT_VCC, 1);

  renderer_init();

  spi_init(SPI_PORT, 62.5 * MHZ);

  adc_init();
  adc_set_temp_sensor_enabled(true);
  adc_select_input(4);
  uint16_t result = adc_read();

  uint16_t fps = 0;
  uint32_t time_ms = us_to_ms(time_us_64());

  int dx = 4;
  int dy = 5;

  int dst_x = 0;
  int dst_y = 0;

  uint16_t color = rgb_to_swapped_565(250, 250, 0);
  renderer_clear(color);

  while (true) {
    if ((us_to_ms(time_us_64()) - time_ms) >= 1000) {
      printf("FPS: %d\n", fps);
      printf("Core Frequency: %d MHz\n", clock_get_hz(clk_sys) / MHZ);
      printf("SPI baudrate: %d MHz\n", spi_get_baudrate(SPI_PORT) / MHZ);
      printf("Peri Frequency: %d MHz\n", clock_get_hz(clk_peri) / MHZ);
      printf("flashclk div: %d\n", PICO_FLASH_SPI_CLKDIV);
      printf("Core Temp: %f Celsius\n",
             27 - (result * 3.3f / (1 << 12) - 0.706) / 0.001721);
      result = adc_read();
      fps = 0;
      time_ms = us_to_ms(time_us_64());
    }

    renderer_draw_sprite(dst_x, dst_y, &sprite, 10);
    // sleep_ms(5);
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
