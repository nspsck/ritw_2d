#include "renderer.h"
#include "src/config.h"
#include <driver_select.h>

#ifndef X_OFFSET
#define X_OFFSET 0
#endif // !X_OFFSET

#ifndef Y_OFFSET
#define Y_OFFSET 0
#endif // !Y_OFFSET

#define WRAPPED_WIDTH (DISPLAY_WIDTH - 2 * X_OFFSET)
#define WRAPPED_HEIGHT (DISPLAY_HEIGHT - 2 * Y_OFFSET)

// Static framebuffer (RGB565)
static uint16_t framebuffer[WRAPPED_WIDTH * WRAPPED_HEIGHT];

void renderer_init(void) { display_init(); }

void renderer_draw_pixel(int x, int y, uint16_t color) {
  if (x < 0 || x >= WRAPPED_WIDTH || y < 0 || y >= WRAPPED_HEIGHT)
    return;

  display_set_area(x + X_OFFSET, y + Y_OFFSET, x + X_OFFSET, y + Y_OFFSET);
  display_write_buffer(&color, 1);
}

void renderer_clear(uint16_t color) { display_clear(color); }

void renderer_fill_rect(int x, int y, int w, int h, uint16_t color) {
  int x0 = x + X_OFFSET;
  int y0 = y + Y_OFFSET;
  int x1 = x0 + w - 1;
  int y1 = y0 + h - 1;

  int x_min = X_OFFSET;
  int y_min = Y_OFFSET;
  int x_max = X_OFFSET + WRAPPED_WIDTH - 1;
  int y_max = Y_OFFSET + WRAPPED_HEIGHT - 1;

  // Rectangle is completely outside viewport
  if (x1 < x_min || y1 < y_min || x0 > x_max || y0 > y_max) {
    return;
  }

  // Clamp to viewport
  if (x0 < x_min)
    x0 = x_min;
  if (y0 < y_min)
    y0 = y_min;
  if (x1 > x_max)
    x1 = x_max;
  if (y1 > y_max)
    y1 = y_max;

  int clipped_width = x1 - x0 + 1;
  int clipped_height = y1 - y0 + 1;

  display_set_area(x0, y0, x1, y1);
  display_fill_color(color, clipped_width * clipped_height);
}

void renderer_blit(int dst_x, int dst_y, const uint16_t *sprite, int width,
                   int height) {
  int x0 = dst_x + X_OFFSET;
  int y0 = dst_y + Y_OFFSET;
  int x1 = x0 + width - 1;
  int y1 = y0 + height - 1;

  int x_min = X_OFFSET;
  int y_min = Y_OFFSET;
  int x_max = X_OFFSET + WRAPPED_WIDTH - 1;
  int y_max = Y_OFFSET + WRAPPED_HEIGHT - 1;

  // Fully outside viewport
  if (x1 < x_min || y1 < y_min || x0 > x_max || y0 > y_max)
    return;

  // Fully inside viewport
  if (x0 >= x_min && y0 >= y_min && x1 <= x_max && y1 <= y_max) {
    display_set_area(x0, y0, x1, y1);
    display_write_buffer(sprite, width * height);
    return;
  }

  // Clipped draw, line by line
  for (int row = 0; row < height; ++row) {
    int screen_y = dst_y + row + Y_OFFSET;
    if (screen_y < y_min || screen_y > y_max)
      continue;

    int start_x = dst_x + X_OFFSET;
    int end_x = start_x + width - 1;

    int clip_start = (start_x < x_min) ? x_min : start_x;
    int clip_end = (end_x > x_max) ? x_max : end_x;
    int visible_width = clip_end - clip_start + 1;

    if (visible_width <= 0)
      continue;

    int sprite_offset = row * width + (clip_start - start_x);
    display_set_area(clip_start, screen_y, clip_end, screen_y);
    display_write_buffer(&sprite[sprite_offset], visible_width);
  }
}
