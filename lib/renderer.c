#include "renderer.h"
#include <stdio.h>

void renderer_init(void) { display_init(); }

void renderer_draw_pixel(int x, int y, uint16_t color) {
  if (x < 0 || x >= render_viewport.w || y < 0 || y >= render_viewport.h)
    return;
  display_set_area(x + render_viewport.x0, y + render_viewport.y0,
                   x + render_viewport.x0, y + render_viewport.y0);
  display_write_buffer(&color, 1);
}

void renderer_clear(uint16_t color) { display_clear(color); }

void renderer_set_viewport(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  // viewport should strictly fit in the screen
  if (x < X_OFFSET || w > WRAPPED_WIDTH || y < Y_OFFSET || h > WRAPPED_HEIGHT)
    return;
  render_viewport.x0 = x;
  render_viewport.y0 = y;
  render_viewport.w = w;
  render_viewport.h = h;
}

void renderer_fill_rect(int x, int y, int w, int h, uint16_t color) {
  int x0 = x + render_viewport.x0;
  int y0 = y + render_viewport.y0;
  int x1 = x0 + w - 1;
  int y1 = y0 + h - 1;

  int x_min = render_viewport.x0;
  int y_min = render_viewport.y0;
  int x_max = render_viewport.x0 + render_viewport.w - 1;
  int y_max = render_viewport.y0 + render_viewport.h - 1;

  // Rectangle is completely outside viewport
  if (x1 < x_min || y1 < y_min || x0 > x_max || y0 > y_max) {
    return;
  }

  // Clamp to viewport
  if (x0 < x_min)
    x0 = x_min;
  if (y0 < y_min)
    y0 = y_min;
  if (x1 > x_max)
    x1 = x_max;
  if (y1 > y_max)
    y1 = y_max;

  int clipped_width = x1 - x0 + 1;
  int clipped_height = y1 - y0 + 1;

  display_set_area(x0, y0, x1, y1);
  display_fill_color(color, clipped_width * clipped_height);
}

void renderer_blit(int dst_x, int dst_y, const uint16_t *rect_obj, int w,
                   int h) {
  int x0 = dst_x + render_viewport.x0;
  int y0 = dst_y + render_viewport.y0;
  int x1 = x0 + w - 1;
  int y1 = y0 + h - 1;

  int x_min = render_viewport.x0;
  int y_min = render_viewport.y0;
  int x_max = render_viewport.x0 + render_viewport.w - 1;
  int y_max = render_viewport.y0 + render_viewport.h - 1;

  // Fully outside viewport
  if (x1 < x_min || y1 < y_min || x0 > x_max || y0 > y_max)
    return;

  // Fully inside viewport
  if (x0 >= x_min && y0 >= y_min && x1 <= x_max && y1 <= y_max) {
    display_set_area(x0, y0, x1, y1);
    display_write_buffer(rect_obj, w * h);
    return;
  }

  // Clipped draw, line by line
  for (int row = 0; row < h; ++row) {
    int screen_y = dst_y + row + render_viewport.y0;
    if (screen_y < y_min || screen_y > y_max)
      continue;

    int start_x = dst_x + render_viewport.x0;
    int end_x = start_x + w - 1;

    int clip_start = (start_x < x_min) ? x_min : start_x;
    int clip_end = (end_x > x_max) ? x_max : end_x;
    int visible_width = clip_end - clip_start + 1;

    if (visible_width <= 0)
      continue;

    int rect_offset = row * w + (clip_start - start_x);
    display_set_area(clip_start, screen_y, clip_end, screen_y);
    display_write_buffer(&rect_obj[rect_offset], visible_width);
  }
}

void renderer_draw_sprite(int dst_x, int dst_y, Sprite *sprite) {
  const uint16_t *sp = sprite_get_frame(sprite);
  int w = sprite->width;
  int h = sprite->height;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      uint16_t color = sp[y * w + x];
      if (color != SPRITE_MASK) {
        renderer_draw_pixel(dst_x + x, dst_y + y, color);
      }
    }
  }
  sprite->current_step = ++sprite->current_step % sprite->steps;
}

void renderer_draw_map(const TileSet *tileset, Map *map) {
  int tile_size = map->tile_size;
  if (render_viewport.w != map->viewport_w * tile_size ||
      render_viewport.h != map->viewport_h * tile_size) {
    return;
  }
#ifdef MAP_VIEWPORT_EUALS_RENDERVIEWPORT
  display_set_area(render_viewport.x0, render_viewport.y0,
                   render_viewport.x0 + render_viewport.w,
                   render_viewport.y0 + render_viewport.h);
#endif /* ifdef MAP_VIEWPORT_EUALS_RENDERVIEWPORT */
  uint8_t id;
  for (int y = 0; y < map->viewport_h; y++) {
    for (int x = 0; x < map->viewport_w; x++) {
      id = map->map[map->viewport_x0 + x + map->width * (map->viewport_y0 + y)];
#ifdef MAP_VIEWPORT_EUALS_RENDERVIEWPORT
      display_write_buffer(tileset_get_tile(tileset, id),
                           tile_size * tile_size);
#else
      renderer_blit(x * map->tile_size, y * map->tile_size,
                    tileset_get_tile(tileset, id), map->tile_size,
                    map->tile_size);

#endif /* ifdef MAP_VIEWPORT_EUALS_RENDERVIEWPORT */
    }
  }
}

void renderer_queue_tilemap(const TileSet *tileset, Map *map,
                            enum TileSize tile_size) {
  if (render_count >= MAX_RENDER_JOBS)
    return;
  if (tile_size == tileset->tile_size && tileset->tile_size == map->tile_size) {
    render_list[render_count++] =
        (RenderJob){.type = RENDER_TILEMAP, .tilemap = {tileset, map}};
  }
}

void renderer_queue_sprite(int x, int y, Sprite *sprite) {
  if (render_count >= MAX_RENDER_JOBS)
    return;
  render_list[render_count++] =
      (RenderJob){.type = RENDER_SPRITE, .sprite = {x, y, sprite}};
}

void renderer_queue_rect(int x, int y, int w, int h, uint16_t color) {
  if (render_count >= MAX_RENDER_JOBS)
    return;
  render_list[render_count++] =
      (RenderJob){.type = RENDER_RECT, .rect = {x, y, w, h, color}};
}

void renderer_queue_pixel(int x, int y, uint16_t color) {
  if (render_count >= MAX_RENDER_JOBS)
    return;
  render_list[render_count++] =
      (RenderJob){.type = RENDER_PIXEL, .pixel = {x, y, color}};
}

void renderer_process_render_list(void) {
  for (size_t i = 0; i < render_count; i++) {
    RenderJob *job = &render_list[i];
    switch (job->type) {
    case RENDER_TILEMAP:
      renderer_draw_map(job->tilemap.tileset, job->tilemap.map);
    case RENDER_SPRITE:
      renderer_draw_sprite(job->sprite.x, job->sprite.y, job->sprite.sprite);
      break;
    case RENDER_RECT:
      renderer_fill_rect(job->rect.x, job->rect.y, job->rect.w, job->rect.h,
                         job->rect.color);
      break;
    case RENDER_PIXEL:
      renderer_draw_pixel(job->pixel.x, job->pixel.y, job->pixel.color);
      break;
    }
  }
  render_count = 0;
}

void render_list_remove_at(size_t index) {
  if (index >= render_count)
    return;
  for (size_t i = index; i < render_count - 1; i++) {
    render_list[i] = render_list[i + 1];
  }
  render_count--;
}

void render_list_remove_sprite(const Sprite *sprite) {
  // check from last one to avoid same sprites being overlooked
  for (size_t i = render_count; i > 0; --i) {
    if (render_list[i].type == RENDER_SPRITE &&
        render_list[i].sprite.sprite == sprite) {
      render_list_remove_at(i);
    }
  }
}

bool render_list_add(RenderJob job) {
  if (render_count >= MAX_RENDER_JOBS)
    return false;
  render_list[render_count++] = job;
  return true;
}
