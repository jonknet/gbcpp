// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once
#include "stddefs.h"
#include <array>
#include <bitset>
#include <vector>
#include "boost/sml.hpp"

using namespace sml;

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
  VBLANK_1 = 1140,
  SEARCH_2 = 20,
  DRAW_3 = 43,
  HBLANK_0 = 51,
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
  friend class ppustates;
  friend struct Event;
  SDL_Texture *sdl_tex;
  MemMgr* mem_mgr;
  std::array<u8,0x5A00> frm_b{};
  PpuState ppu_state_;

  void draw_line();
  int search();

  void render_tile_to_colors(u16 address, std::array<u8,8*16>& tile_pixels);
  void render_line_of_tile(u16 address, u8 linenumber, std::array<u8,8>& line_pixels);
  static u16 get_address_tile_by_index(s16 index, TileAddressMode address_mode);

  static int max_cycles(PpuMode mode,int numSprites){
	switch(mode){
	  case VBLANK_1:
	  case SEARCH_2:
	    return (int)mode;
	  case HBLANK_0:
	    return (int)(HBLANK_0 - extra_cycles);
	  case DRAW_3:
	    return (int)(DRAW_3 + extra_cycles);
	  case IDLE:
	  default:
	    return 0;
	}
  }



  // Events

  struct Event {
	int max_cycles = 0;
  };
  struct Begin : Event {};
  struct ModeChange : Event {};
  struct Search : Event {
	::GBCPP::Ppu::
  };
  struct Draw;
  struct Hblank;
  struct Vblank;
  struct FinishLine : Event {};
  struct FinishFrame : Event {};
  struct VblankEnd : Event {};
  struct Tick;

namespace sml = boost::sml;

  struct sm_tick {
    using self = sm_tick;

	const auto idle = state<class idle>;
	return make_transition_table (
	 	*state<ppustates> + event<Tick> = state<ppustates>
	);
  };

  struct ppustates {
    using self = ppustates;

    private const Ppu* ppu_inst_;
    public ppustates(Ppu& ppu_inst) : ppu_inst_(ppu_inst){ };

	auto operator()() {
	  using namespace sml;

	  auto mode_guard = [] (Ppu& ppu_inst) {
		return (ppu_inst_->ppu_state_.currentCycle > Ppu::max_cycles(e.currentMode,ppu_inst_->ppu_state_.spritesOnLine.size()) ? true : false);
	  };

	  auto mode_change_action = [](){
		ppu_inst_->ppu_state_.currentCycle = 0;
	  };

	  auto tick = [](){
		sml::state<class ppustates> i;

	  };

	};

	const auto idle = state<class idle>;
	return make_transition_table (
			*idle + event<Search> = "s1"
			,"s1"_s + event<Draw> [mode_guard] = "s2"_s
			,"s2"_s + event<Hblank> = "s3"_s
			,"s3"_s + event<Vblank> = "s4"_s
			,"s4"_s + event<Search> = "s1"_s
	);

  };


  friend struct transitions {
    auto operator()() const noexcept {

	}
  };


};

}
