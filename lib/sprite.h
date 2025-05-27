#ifndef SPRITE_H
#define SPRITE_H

#include <color.h>
#include <stddef.h>
#include <stdint.h>

/* up to 256 x 256 sprites */
typedef struct {
  uint8_t steps; // 256 steps max, more than enough
  uint8_t current_step;
  uint8_t width;
  uint8_t height;
  const uint16_t *sprite_array;
} Sprite;

/**
 * Get a pointer to the given sprite's current frame.
 */
const uint16_t *sprite_get_frame(const Sprite *sprite);

#endif // !SPRITE_H
