#include "sprite.h"

const uint16_t *sprite_get_frame(const Sprite *sprite) {
  size_t frame_size = sprite->width * sprite->height;
  return &sprite->sprite_array[sprite->current_step * frame_size];
}
