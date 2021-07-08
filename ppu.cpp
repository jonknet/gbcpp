#include "ppu.h"
#include <cassert>

using namespace GBCPP;

Ppu::Ppu(SDL_Texture *tex) : sdl_tex{tex} {
  assert(tex != nullptr);
}

void Ppu::draw() {

}

void Ppu::render_tile_to_colors(Tile& tile, const gb_mem &gb_memory, std::array<u8,8*8>& tile_pixels) {
  assert(tile.address >= 0x8000 && tile.address <= (0x9800 - 16));

  u8 line_a = 0;
  u8 line_b = 0;

  for (int i = 0; i < 16; i += 2) {
	line_a = gb_memory[tile.address];
	line_b = gb_memory[tile.address + 1];

	for(int j = 0; j < 8; j++){
	  tile_pixels[(i*8)+(7-j)] = DefaultPalette[(((line_b >> j) & 1) << 1) + ((line_a >> j) & 1)];
	}
  }



}
