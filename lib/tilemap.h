#ifndef TILEMAP_H
#define TILEMAP_H

#include <color.h>
#include <stddef.h>
#include <stdint.h>

/* Target: enough to fill 240x240 display with disjunct tiles */
#define MAX_TILES_8x8_NUM 30
#define MAX_TILES_16x16_NUM 15

enum TileSize {
  TILE8x8 = 8,
  TILE16x16 = 16,
};

/**
 * id is just the n-th tile in the TileMap
 */
typedef struct {
  uint8_t id;
  const uint16_t *tile;
  enum TileSize size;
} Tile;

/**
 * Each TileMap only supports 256 tiles at max
 * Using color565 format. Meaning:
 * 1 pixel = 16 bits = 2 bytes
 * ONLY supports 8x8 or 16x16 tiles
 */
typedef struct {
  uint8_t number_tiles;
  const uint16_t *tilemap;
  enum TileSize tile_size;
} TileMap;

/**
 * Get a pointer to the tile
 */
const uint16_t *tilemap_get_tile(const TileMap *tilemap, uint16_t id);

#endif // !TILEMAP_H
