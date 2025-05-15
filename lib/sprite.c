#include "sprite.h"

const uint16_t *sprite_get_frame(const Sprite *sprite, uint8_t step) {
  if (step >= sprite->steps) {
    step = 0;
  }
  size_t frame_size = sprite->width * sprite->height;
  return &sprite->sprite_array[step * frame_size];
}
