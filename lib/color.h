#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

#define rgb_to_565(r, g, b)                                                    \
  ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))

#define color_swap_bytes(c) (((c) >> 8) | ((c) << 8))

/**
 * Convert r,g,b to color565 ready for the renderer to draw. Should be only used
 * for drawing functions.
 */
uint16_t rgb_to_swapped_565(uint8_t r, uint8_t g, uint8_t b);

#endif // COLOR_H
