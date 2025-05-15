#include <color.h>

uint16_t rgb_to_swapped_565(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t color565 = rgb_to_565(r, g, b);
  return color_swap_bytes(color565);
}
