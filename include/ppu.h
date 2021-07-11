#pragma once
#include "stddefs.h"
#include <array>
#include <bitset>
#include <vector>


struct SDL_Texture;

namespace GBCPP {

class MemMgr;

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

enum class PaletteColors {
  WHITE = 0,
  LT_GRAY = 1,
  DRK_GRAY = 2,
  BLACK = 3
};

enum class SpriteFlags {
  bg_win_over_obj = (1 << 7),
  x_flip = (1 << 6),
  y_flip = (1 << 5),
  palette_num = (1 << 4)
};

enum class TileAddressMode {
  MODE_8000 = 1,
  MODE_8800 = 0
};

enum PpuMode {
  HBLANK_0,
  VBLANK_1,
  SEARCH_2,
  DRAW_3,
  IDLE
};

struct Tile {
  u16 address{};
};

struct Sprite {
  u8 y_pos = 0;
  u8 x_pos = 0;
  u16 address = 0;
  std::bitset<8> flags = {0};
};

struct PpuState {
  PpuMode currentMode = SEARCH_2;
  int currentCycle = 0;
  int currentX = 0;
  std::vector<Sprite> spritesOnLine = std::vector<Sprite>(10);
};

const u8 DefaultPalette[4] = { 0xFF, 0xAB, 0x66, 0x00 };

class Ppu {
 public:
  explicit Ppu(MemMgr* mem_mgr, SDL_Texture *tex);
  void tick();

protected:
  SDL_Texture *sdl_tex;
  MemMgr* mem_mgr;
  std::array<u8,0x5A00> frm_b{};
  PpuState state;
  void draw_line();
  int search();

  void render_tile_to_colors(u16 address, std::array<u8,8*16>& tile_pixels);
  void render_line_of_tile(u16 address, u8 linenumber, std::array<u8,8>& line_pixels);

  static u16 get_address_tile_by_index(s16 index, TileAddressMode address_mode);

};

}
