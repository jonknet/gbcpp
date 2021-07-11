#include "ppu.h"
#include "mm.h"
#include "SDL2/SDL.h"
#include <cassert>
#include <array>
#include <iterator>
#include <algorithm>
#include <functional>

using namespace GBCPP;

Ppu::Ppu(MemMgr *mem_mgr, SDL_Texture *tex) : sdl_tex{tex},frm_b{0},mem_mgr(mem_mgr) {
  assert(tex != nullptr && mem_mgr != nullptr);
}

void Ppu::tick() {
  switch (state.currentMode) {
	case HBLANK_0:
	  if (state.currentCycle > (52 - (state.spritesOnLine.size() * 3))) {
		if ((*mem_mgr)[LY] < 143) {
		  state.currentMode = SEARCH_2;
		} else {
		  state.currentMode = VBLANK_1;
		}
		state.currentCycle = -1;
		(*mem_mgr)[LY] += 1;
	  }
	  break;
	case VBLANK_1:
	  if (state.currentCycle == 114) {
		(*mem_mgr)[LY] += 1;
	  } else if (state.currentCycle > 1140) {
		state.currentMode = SEARCH_2;
		state.currentCycle = -1;
        (*mem_mgr)[LY] = 0;
	  }
	  break;
	case SEARCH_2:
	  if (state.currentCycle > 20) {
		state.currentMode = DRAW_3;
		state.currentCycle = -1;
	  }
	  if (state.currentCycle == 0) {
		search();
	  }
	  break;
	case DRAW_3:
	  if (state.currentCycle > (43 + (state.spritesOnLine.size() * 3))) {
		state.currentMode = HBLANK_0;
		state.currentCycle = -1;
	  } else if (state.currentCycle == 0) {
        for(int i = 0; i < 144; ++i){
            draw_line();
            (*mem_mgr)[LY] += 1;
        }
	  }
	  break;
	case IDLE: break;
  }
  ++state.currentCycle;
}

int Ppu::search() {
  state.spritesOnLine.clear();
  int vert_size = ((*mem_mgr)[LCDC] & (u8)LcdcFlags::obj_size) ? 16 : 8;
  for (int i = 0; i < 40 && state.spritesOnLine.size() <= 10; i++) {
	u16 oam_index = OAM_160B + (i * 4);
	u8 tile_y = (*mem_mgr)[oam_index];
	u8 ly = (*mem_mgr)[LY];
	int top_line_y = (tile_y - 16);
	int bottom_line_y = tile_y;

	if (top_line_y + vert_size >= 0 && (bottom_line_y + 16 < 160 && top_line_y > -16) && (ly >= top_line_y + 16 && ly <= bottom_line_y)) {
	  Sprite spr{tile_y,
		  (*mem_mgr)[oam_index + 1],
		  static_cast<u16>(((*mem_mgr)[oam_index + 2] * 16) + VRAM_8KB),
		  std::bitset<8>((*mem_mgr)[oam_index + 3])};
	  state.spritesOnLine.push_back(spr);
	}
  }

  return static_cast<int>(state.spritesOnLine.size());
}

u16 Ppu::get_address_tile_by_index(s16 index,TileAddressMode address_mode) {
  assert(index >= -127 && index <= 255);
  if (address_mode == TileAddressMode::MODE_8000) {
	return (0x8000 + (index * 16));
  } else {
	index = static_cast<u8>(index);
	if (index < 128) {
	  return (0x9000 + (index * 16));
	} else {
	  return (0x8800 + ((index - 128) * 16));
	}
  }
}

void Ppu::draw_line() {
  u8 line = (*mem_mgr)[LY];
  u16 bg_map_address = (((*mem_mgr)[LCDC] & (u8)LcdcFlags::bg_map_area) ? 0x9C00 : 0x9800);
  int aligned_line = line / 8;
  int x_index = (*mem_mgr)[SCX] / 8;
  s16 tile_index = static_cast<s16>((*mem_mgr)[bg_map_address + ((aligned_line / 8) * 32) + x_index]);

  std::array<u8, 8> line_buffer{0};

  for (int i = 0; i <= 20 && (i + x_index) < 32; ++i) {
	render_line_of_tile(get_address_tile_by_index(tile_index,
                                                  ((*mem_mgr)[LCDC] & (u8)LcdcFlags::tile_data_area) ? TileAddressMode::MODE_8000 : TileAddressMode::MODE_8800),
                                                  line % 8, line_buffer);

	auto dest = frm_b.begin() + (line * 160) + (i * 8);

	if (i == 0 && ((i + x_index) * 8) < (*mem_mgr)[SCX]) {
	  std::copy(line_buffer.begin() + ((*mem_mgr)[SCX] % 8), line_buffer.end(), dest);
	  dest += 8;
	} else if (i == 20 && ((i + x_index) * 8) + 8 > ((*mem_mgr)[SCX] + 160)) {
	  std::copy(line_buffer.begin(), line_buffer.end() - (8 - ((*mem_mgr)[SCX] % 8)), dest);
	} else {
	  std::copy(line_buffer.begin(), line_buffer.end(), dest);
	  dest += 8;
	}

  }

  for (auto &spr : state.spritesOnLine) {
	int top_line = spr.y_pos - 16;
	int sprite_line = std::abs((*mem_mgr)[LY] - top_line);

	render_line_of_tile(spr.address, sprite_line, line_buffer);

	if(spr.x_pos - 8 < (*mem_mgr)[SCX]){
	    std::copy(line_buffer.begin() + ((*mem_mgr)[SCX] - (spr.x_pos - 8)),line_buffer.end(),
                  frm_b.begin() + ((top_line + sprite_line) * 160) + (((spr.x_pos - 8) < 0) ? 0 : (spr.x_pos - 8)));
	} else if(spr.x_pos > ((*mem_mgr)[SCX] + 160)){
        std::copy(line_buffer.begin(),line_buffer.end() - (spr.x_pos - ((*mem_mgr)[SCX] + 160)),
                  frm_b.begin() + ((top_line + sprite_line) * 160) + (spr.x_pos - 8));
	} else {
        std::copy(line_buffer.begin(),line_buffer.end(),
                  frm_b.begin() + ((top_line + sprite_line) * 160) + (spr.x_pos - 8));
	}

  }

}

void Ppu::render_tile_to_colors(u16 address, std::array<u8, 8 * 16> &tile_pixels) {
  assert(address >= 0x8000 && address <= (0x9800 - 16));

  std::array<u8, 8> line_pixels{0};

  for (int i = 0; i < (((*mem_mgr)[LCDC] & (u8)LcdcFlags::obj_size) ? 16 : 8); ++i) {
	render_line_of_tile(address, i, line_pixels);
	std::copy_n(line_pixels.begin(), 8, tile_pixels.begin() + (i * 8));
  }
}

void Ppu::render_line_of_tile(u16 address, u8 linenumber, std::array<u8, 8> &line_pixels) {
  assert(linenumber >= 0 && linenumber < 16);

  if(linenumber >= 8){
    address += 2;
    linenumber -= 8;
  }

  u8 line_a = (*mem_mgr)[address + (linenumber * 2)];
  u8 line_b = (*mem_mgr)[address + (linenumber * 2) + 1];

  std::generate_n(line_pixels.begin(), 8,
				  [j = 0, line_a = line_a, line_b = line_b]() mutable {
	u8 out = DefaultPalette[(((line_b >> j) & 1) << 1) + ((line_a >> j) & 1)];
	++j;
	return out;
  });

}




