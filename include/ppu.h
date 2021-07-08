#pragma once
#include "stddefs.h"
#include "SDL2/SDL.h"
#include <array>
#include <bitset>

namespace GBCPP {

enum class LcdcFlags {
  bg_win_en = 1,
  obj_en = (1 << 1),
  obj_size = (1 << 2),
  bg_map_area = (1 << 3),
  tile_data_area = (1 << 4),
  win_en = (1 << 5),
  win_map_area = (1 << 6),
  lcd_ppu_en = (1 << 7)
};

enum class SpriteFlags {
  bg_win_over_obj = (1 << 7),
  x_flip = (1 << 6),
  y_flip = (1 << 5),
  palette_num = (1 << 4)
};

struct Tile {
  u16 address{};
};

struct Sprite {
  u8 y_pos = 0;
  u8 x_pos = 0;
  u8 index = 0;
  std::bitset<8> flags = {0};
};

const u8 DefaultPalette[4] = { 0xFF, 0xAB, 0x66, 0x00 };

class Ppu {
 public:
  explicit Ppu(SDL_Texture *tex);
  void draw();
private:
  SDL_Texture *sdl_tex;
  static void render_tile_to_colors(Tile& tile, const gb_mem& gb_memory,std::array<u8,8*8>& tile_pixels);

};

}
