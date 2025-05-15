#ifndef RENDERER_H
#define RENDERER_H

#include <sprite.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tilemap.h>

#define MAX_RENDER_JOBS 64

enum RenderType {
  RENDER_TILEMAP = 0,
  RENDER_SPRITE = 1,
  RENDER_RECT = 2,
  RENDER_PIXEL = 3,
};

typedef struct {
  enum RenderType type;
  union {
    struct {
      int x, y;
      const Sprite *sprite;
      uint8_t step;
    } sprite;
    struct {
      const TileMap *tilemap;
      Tile *tiles[MAX_TILES_8x8_NUM];
    } tilemap8x8;
    struct {
      const TileMap *tilemap;
      Tile *tiles[MAX_TILES_16x16_NUM];
    } tilemap16x16;
    struct {
      int x, y, w, h;
      uint16_t color;
    } rect;
    struct {
      int x, y;
      uint16_t color;
    } pixel;
  };
} RenderJob;

static RenderJob render_list[MAX_RENDER_JOBS];
static size_t render_count = 0;

/**
 * Initializes the renderer (if any internal state needs setup) up on needs.
 */
void renderer_init(void);

/**
 * Draws a single pixel at (x, y).
 */
void renderer_draw_pixel(int x, int y, uint16_t color);

/**
 * Fills the entire screen with a given color.
 */
void renderer_clear(uint16_t color);

/**
 * fills a rectangle at (x, y) with size (w x h).
 */
void renderer_fill_rect(int x, int y, int w, int h, uint16_t color);

/**
 * Blit a rectangle of content to an area.
 */
void renderer_blit(int dst_x, int dst_y, const uint16_t *sprite, int w, int h);

/**
 * Draw a sprite pixel for pixel to an area, ignoring 0x0000.
 */
void renderer_draw_sprite(int dst_x, int dst_y, const Sprite *sprite,
                          uint8_t step);

/**
 * Queue a list of tiles (can be used a map) to be rendered.
 */
void renderer_queue_tilemap(const TileMap *tilemap, Tile tiles[],
                            enum TileSize tile_size);

/**
 * Queue a sprite to be rendered, step automatically increased by one up on
 * next. render
 */
void renderer_queue_sprite(int x, int y, const Sprite *sprite, uint8_t step);

/**
 * queue a rect to be rendered.
 */
void renderer_queue_rect(int x, int y, int w, int h, uint16_t color);

/**
 * queue a pixel to be rendered, serioysly, why do you need this?
 */
void renderer_queue_pixel(int x, int y, uint16_t color);

/**
 * Renders everything on to the screen, in the order items were added to the
 * list, then set render_count to 0 with causes new items to be added again at
 * the beginning of the render_list, hence achieving a "clearing render queue"
 * effect. Note you have to make sure the lifecycle of added items are managed.
 */
void renderer_process_render_list(void);

/**
 * Removes the item at given index if the index is in bound, then shift
 * everything to fill gap caused by removing the index. Decrease render_count by
 * 1 on sucess.
 */
void render_list_remove_at(size_t index);

/**
 * Returns true  if given sprite is removed. Decrease render_count by 1 on
 * sucess.
 */
bool render_list_remove_sprite(const Sprite *sprite_ptr);

/**
 * Returns true  if given job is added. Increase render_count by 1 on
 * sucess.
 */
bool render_list_add(RenderJob job);

#endif // !RENDERER_H
