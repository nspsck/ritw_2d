#include "config.h"
#include "pico/stdlib.h"
#include "rp2040_input.h"
#include <color.h>
#include <hardware/adc.h>
#include <hardware/clocks.h>
#include <hardware/flash.h>
#include <hardware/gpio.h>
#include <hardware/pll.h>
#include <hardware/spi.h>
#include <hardware/timer.h>
#include <hardware/vreg.h>
#include <pico/rand.h>
#include <pico/time.h>
#include <renderer.h>
#include <sprite.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

#include "hardware/structs/ssi.h"
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

#define SYS_CLOCK_MHZ 250
#define PERI_CLOCK_MHZ 125

#define WINDOW_WIDTH 128
#define WINDOW_HEIGHT 160
#define TILE_SIZE 8
#define GRID_WIDTH (WINDOW_WIDTH / TILE_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / TILE_SIZE)
#define GRID_SIZE GRID_WIDTH *GRID_HEIGHT
#define BG_COLOR rgb_to_swapped_565(255, 255, 0)
#define SNAKE_COLOR rgb_to_swapped_565(0, 250, 0)
#define FOOD_COLOR rgb_to_swapped_565(244, 10, 55)
#define MSPF (1000 / 60)

typedef struct {
  int x, y;
} Point;

typedef struct {
  Point body[GRID_SIZE];
  int length;
  int dx, dy;
} Snake;

Snake snake;
Point food;
bool running = true;
bool direction_chagned = false;

InputState input;

bool occupied[GRID_WIDTH][GRID_HEIGHT]; // 0 = empty, 1 = occupied
Point empty_cells[GRID_SIZE];
int empty_count;

void rand_food() {
  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      occupied[x][y] = false;
    }
  }
  empty_count = 0;

  for (int i = 0; i < snake.length; i++) {
    occupied[snake.body[i].x][snake.body[i].y] = 1;
  }

  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      if (!occupied[x][y]) {
        empty_cells[empty_count].x = x;
        empty_cells[empty_count].y = y;
        empty_count++;
      }
    }
  }

  int r = get_rand_32() % empty_count;
  food = empty_cells[r];
}

void init() {
  snake.length = 3;
  int head_x = GRID_WIDTH / 2;
  int head_y = GRID_HEIGHT / 2;
  snake.dx = 0;
  snake.dy = -1;
  for (int i = 0; i < snake.length; i++) {
    snake.body[i].x = head_x;
    snake.body[i].y = head_y + i;
  }

  rand_food();
}

void draw_rect(Point p, uint16_t color) {
  renderer_queue_rect(p.x * TILE_SIZE, p.y * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                      color);
}

void update() {
  /* Allow input again */
  direction_chagned = false;

  Point new_head = {.x = snake.body[0].x + snake.dx,
                    .y = snake.body[0].y + snake.dy};

  if (new_head.x < 0)
    new_head.x = GRID_WIDTH - 1;
  if (new_head.x >= GRID_WIDTH)
    new_head.x = 0;
  if (new_head.y < 0)
    new_head.y = GRID_HEIGHT - 1;
  if (new_head.y >= GRID_HEIGHT)
    new_head.y = 0;

  draw_rect(new_head, SNAKE_COLOR);

  /* Game Over */
  for (int i = 0; i < snake.length; i++) {
    if (snake.body[i].x == new_head.x && snake.body[i].y == new_head.y) {
      running = false;
      printf("Game over!\n");
      return;
    }
  }

  /* Snake moves */
  for (int i = snake.length; i > 0; i--) {
    snake.body[i] = snake.body[i - 1];
  }
  snake.body[0] = new_head;

  /* Check food */
  if (new_head.x == food.x && new_head.y == food.y) {
    /* Game Won */
    if (snake.length++ == GRID_SIZE) {
      running = false;
      printf("Game won!\n");
      return;
    }
    rand_food();
  }
  draw_rect(snake.body[snake.length], BG_COLOR);
}

void draw() {
  /*Draw food*/
  draw_rect(food, FOOD_COLOR);
  renderer_process_render_list();
}

void handle_input() {
  if (!direction_chagned) {
    if (input.y) {

      if (snake.dy == 0) {
        snake.dx = 0;
        snake.dy = -1;
        direction_chagned = true;
      }
    } else if (input.a) {
      if (snake.dy == 0) {
        snake.dx = 0;
        snake.dy = 1;
        direction_chagned = true;
      }
    } else if (input.x) {
      if (snake.dx == 0) {
        snake.dx = -1;
        snake.dy = 0;
        direction_chagned = true;
      }
    } else if (input.b) {
      if (snake.dx == 0) {
        snake.dx = 1;
        snake.dy = 0;
        direction_chagned = true;
      }
    }
  }
}

int main() {

  vreg_set_voltage(VREG_VOLTAGE_1_30);
  set_flash_div(SYS_CLOCK_MHZ * MHZ);
  // Set sysclock to 250MHz and periclock to 125MHz
  set_sys_clock_hz(SYS_CLOCK_MHZ * MHZ, false);
  clock_configure(clk_peri, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                  CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                  SYS_CLOCK_MHZ * MHZ, PERI_CLOCK_MHZ * MHZ);
  stdio_init_all();

  // Turn on display
  gpio_init(PIN_TFT_VCC);
  gpio_set_dir(PIN_TFT_VCC, GPIO_OUT);
  gpio_put(PIN_TFT_VCC, 1);

  renderer_init();

  uint16_t fps = 0;
  uint32_t time_ms = us_to_ms(time_us_64());

  init();
  renderer_clear(BG_COLOR);

  /* Draw Snake */
  for (int i = 0; i < snake.length; i++) {
    draw_rect(snake.body[i], SNAKE_COLOR);
  }

  uint32_t now = us_to_ms(time_us_64());
  uint32_t last_tick = us_to_ms(time_us_64());

  input_init();

  adc_set_temp_sensor_enabled(true);
  uint16_t result;

  while (true) {
    if ((us_to_ms(time_us_64()) - time_ms) >= 1000) {
      printf("FPS: %d\n", fps);
      printf("Core Frequency: %d MHz\n", clock_get_hz(clk_sys) / MHZ);
      printf("SPI baudrate: %d MHz\n", spi_get_baudrate(SPI_PORT) / MHZ);
      printf("Peri Frequency: %d MHz\n", clock_get_hz(clk_peri) / MHZ);
      adc_select_input(4);
      result = adc_read();
      printf("Core Temp: %f Celsius\n",
             27 - (result * 3.3f / (1 << 12) - 0.706) / 0.001721);
      fps = 0;
      time_ms = us_to_ms(time_us_64());
    }

    input_poll(&input);
    handle_input();

    now = us_to_ms(time_us_64());
    if (now - last_tick >= 200) {
      update();
      last_tick = now;
    }

    draw();

    fps++;
  }
}
