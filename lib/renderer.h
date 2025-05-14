#ifndef RENDERER_H
#define RENDERER_H

#include <stddef.h>
#include <stdint.h>

// Initializes the renderer (if any internal state needs setup)
void renderer_init(void);

// Draws a single pixel at (x, y)
void renderer_draw_pixel(int x, int y, uint16_t color);

// Fills the entire screen with a given color
void renderer_clear(uint16_t color);

// Fills a rectangle at (x, y) with size (w x h)
void renderer_fill_rect(int x, int y, int w, int h, uint16_t color);

// Copies a width x height sprite to the framebuffer at (dst_x, dst_y)
void renderer_blit(int dst_x, int dst_y, const uint16_t *sprite, int width,
                   int height);

#endif // !RENDERER_H
