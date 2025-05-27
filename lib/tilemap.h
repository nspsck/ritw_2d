#ifndef TILEMAP_H
#define TILEMAP_H

#include <color.h>
#include <stddef.h>
#include <stdint.h>

enum TileSize {
  TILE8x8 = 8,
  TILE16x16 = 16,
};

/**
 * Each TileSet only supports 256 tiles at max Using color565 format. Meaning: 1
 * pixel = 16 bits = 2 bytes ONLY supports 8x8 or 16x16 tiles.
 */
typedef struct {
  enum TileSize tile_size;
  uint8_t number_tiles;
  const uint16_t *tileset;
} TileSet;

/**
 * A Map that contains all the tiles with their id. All tiles must be of the
 * same size. Either 16x16 or 8x8.
 * map_size: size of the map.
 * map:
 * - each number is a tile_id. tile_id ranges from 1 to 255. 0x00 means the
 * map is empty on the current location, aka, black background
 * - map's dimension width * height.
 * viewport_w, viewport_h for tile_size 8*8 should be a multiple of 8,
 * viewport_w, viewport_h for tile_size 16*16 should be a multiple of 16.
 */
typedef struct {
  enum TileSize tile_size;
  uint16_t viewport_x0;
  uint16_t viewport_y0;
  uint16_t viewport_w;
  uint16_t viewport_h;
  uint16_t width;
  uint16_t height;
  const uint8_t *map;
} Map;

/**
 * Get a pointer to the tile with id in the specific tileset. Returns NULL if id
 * is out of bound.
 */
const uint16_t *tileset_get_tile(const TileSet *tileset, uint8_t id);

#endif // !TILEMAP_H
