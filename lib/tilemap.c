#include "tilemap.h"

const uint16_t *tileset_get_tile(const TileSet *tileset, uint8_t id) {
  if (!tileset || !tileset->tileset || id >= tileset->number_tiles)
    return NULL;

  size_t pixels_per_tile = tileset->tile_size * tileset->tile_size;
  return &tileset->tileset[id * pixels_per_tile];
}
