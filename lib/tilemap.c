#include "tilemap.h"

const uint16_t *tilemap_get_tile(const TileMap *tilemap, uint16_t id) {
  if (!tilemap || !tilemap->tilemap || id >= tilemap->number_tiles)
    return NULL;

  size_t pixels_per_tile = tilemap->tile_size * tilemap->tile_size;
  return &tilemap->tilemap[id * pixels_per_tile];
}
