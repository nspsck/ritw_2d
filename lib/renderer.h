#ifndef RENDERER_H
#define RENDERER_H

/* config.h has to be imported earlier than driver_select.h */
#include "src/config.h"
#include <driver_select.h>
#include <sprite.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tilemap.h>

/**
 * any value in style 0x00** performs the same, as soon as you change the higher
 * bits, you lose ~1.8% performance on drawing pixels
 */
#define SPRITE_MASK 0x00ff

#ifndef X_OFFSET
#define X_OFFSET 0
#endif // !X_OFFSET

#ifndef Y_OFFSET
#define Y_OFFSET 0
#endif // !Y_OFFSET

#define WRAPPED_WIDTH (DISPLAY_WIDTH - 2 * X_OFFSET)
#define WRAPPED_HEIGHT (DISPLAY_HEIGHT - 2 * Y_OFFSET)

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
      Sprite *sprite;
    } sprite;
    struct {
      const TileSet *tileset;
      Map *map;
    } tilemap;
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

typedef struct {
  uint16_t x0;
  uint16_t y0;
  uint16_t w;
  uint16_t h;
} ViewPort;

static RenderJob render_list[MAX_RENDER_JOBS];
static size_t render_count = 0;
static ViewPort render_viewport = {X_OFFSET, Y_OFFSET, WRAPPED_WIDTH,
                                   WRAPPED_HEIGHT};

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
void renderer_draw_sprite(int dst_x, int dst_y, Sprite *sprite);

/**
 * Queue a list of tiles (can be used a map) to be rendered. Also checks the
 * dimensions just for memory safety. No operation on failed dimension check.
 */
void renderer_queue_tilemap(const TileSet *tileset, Map *map,
                            enum TileSize tile_size);

/**
 * Queue a sprite to be rendered, step automatically increased by one up on
 * next render.
 */
void renderer_queue_sprite(int x, int y, Sprite *sprite);

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
 * !!! Note the index starts with 1 !!!
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
