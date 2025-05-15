#ifndef SPRITE_H
#define SPRITE_H

#include <color.h>
#include <stddef.h>
#include <stdint.h>

/* up to 256 x 256 sprites */
typedef struct {
  uint8_t steps; // 256 steps max, more than enough
  uint8_t width;
  uint8_t height;
  const uint16_t *sprite_array;
} Sprite;

/**
 * Get a pointer to the frame at index `step`
 * Returns pointer to the top-left pixel of the frame (in RGB565).
 */
const uint16_t *sprite_get_frame(const Sprite *sprite, uint8_t step);

#endif // !SPRITE_H
