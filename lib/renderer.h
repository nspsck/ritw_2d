#ifndef RENDERER_H
#define RENDERER_H

#include <stddef.h>
#include <stdint.h>

// initializes the renderer (if any internal state needs setup)
void renderer_init(void);

// draws a single pixel at (x, y)
void renderer_draw_pixel(int x, int y, uint16_t color);

// fills the entire screen with a given color
void renderer_clear(uint16_t color);

// fills a rectangle at (x, y) with size (w x h)
void renderer_fill_rect(int x, int y, int w, int h, uint16_t color);

// blit a rectangle of content to an area
void renderer_blit(int dst_x, int dst_y, const uint16_t *sprite, int w, int h);

// draw a sprite pixel for pixel to an area, ignoring 0x0000
void renderer_draw_sprite(int dst_x, int dst_y, const uint16_t *sprite, int w,
                          int h);

#endif // !RENDERER_H
