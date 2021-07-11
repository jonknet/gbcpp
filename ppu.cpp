#include "ppu.h"
#include "mm.h"
#include <cassert>
#include <array>
#include <iterator>
#include <algorithm>
#include <functional>


using namespace GBCPP;

Ppu::Ppu(MemMgr* mem_mgr, SDL_Texture *tex) : sdl_tex{tex} {
  assert(tex != nullptr && mem_mgr != nullptr);
  this->mem_mgr = mem_mgr;
  frm_b = std::array<u8,0x5A00> {0};
}

void Ppu::tick() {
	switch(state.currentMode){
	  case HBLANK_0:
	    if(state.currentCycle > (52 - (state.lineObjects * 3))){
	      if(state.currentLine < 143){
	        state.currentMode = SEARCH_2;
	      } else {
	        state.currentMode = VBLANK_1;
	      }
	      state.currentCycle = -1;
	      state.lineObjects = -1;
		  (*mem_mgr)[LY] += 1;
	    }

	    break;
	  case VBLANK_1:
	    if(state.currentCycle % 114 == 0){
		  (*mem_mgr)[LY] += 1;
	    }
	    if(state.currentCycle > 1140){
	      state.currentMode = SEARCH_2;
	      state.currentCycle = -1;
	    }
	    break;
	  case SEARCH_2:
	    if(state.currentCycle > 20){
	      state.currentMode = DRAW_3;
	      state.currentCycle = -1;
	    }
	    if(state.currentCycle == 0){
	      	search();
	    }
	    break;
	  case DRAW_3:
	    if(state.currentCycle > (43 + (state.lineObjects*3))){
	      state.currentMode = HBLANK_0;
	    }
	    if(state.currentCycle == 0){

	    }
	    break;
	  case IDLE:
	    break;
	}
  	++state.currentCycle;
}

int Ppu::search(){
  state.spritesOnLine.clear();
  int objects = 0;
  int vert_size = 8;
  if((*mem_mgr)[LCDC] & (u8)LcdcFlags::obj_size){
    vert_size = 16;
  }
  for(int i = 0; i < 40 && objects <= 10; i++){
    u16 oam_index = OAM_160B+(i*4);
    u8 tile_y = (*mem_mgr)[oam_index];
    u8 ly = state.currentLine;
    int top_line_y = (tile_y - 16);
    int bottom_line_y = top_line_y + vert_size;

    if((bottom_line_y > 0 && top_line_y < 144) && (ly >= top_line_y && ly <= bottom_line_y)){
		Sprite spr { .y_pos = tile_y,
					 .x_pos = (*mem_mgr)[oam_index+1],
					 .address = static_cast<u16>(((*mem_mgr)[oam_index+2] * 16) + VRAM_8KB),
					 .flags = std::bitset<8>((*mem_mgr)[oam_index+3])};
		state.spritesOnLine.push_back(spr);
    }
  }

  return static_cast<int>(state.spritesOnLine.size());
}

u16 Ppu::get_address_tile_by_index(s16 index){
  assert(index >= -127 && index <= 255);
  if((*mem_mgr)[LCDC] & (u8)LcdcFlags::tile_data_area){
    return (0x8000 + (index * 16));
  } else {
    index = static_cast<u8>(index);
    if(index < 128){
      return (0x9000 + (index * 16));
    } else {
      return (0x8800 + ((index - 128) * 16));
    }
  }
  return 0;
}

void Ppu::draw_line(u8 line){
	assert(line >= 0 && line <= 144);
	u16 bg_address = (((*mem_mgr)[LCDC] & (u8)LcdcFlags::bg_map_area) ? 0x9C00 : 0x9800);
  int scx = (*mem_mgr)[SCX];
  int scy = (*mem_mgr)[SCY];
  int scx_end = scx + 160;
  int scy_end = scy + 144;
  int tile_offset_y = line % 8;
  int tile_offset_x = scx % 8;
  int tile_begin_line = line - tile_offset_y;
  int tile_begin_x = scx - tile_offset_x;
  u16 tile_begin_address = bg_address + ((tile_begin_line / 8) * (16*32)) + ((tile_begin_x / 8) * 16);
  int start_index = scx / 8;

  std::array<u8,8> line_buffer {0};

  for(int i = start_index; i <= (start_index + 20) && i < 32; ++i){
	  render_line_of_tile(tile_begin_address+((i - start_index)*16),tile_offset_y,line_buffer);

	  auto dest = frm_b.begin() + (line * 160) + ((i - start_index) * 8);

	  if(i == start_index && scx - (i*8) > 0){
		  std::copy(line_buffer.begin() + (scx - (i*8)),line_buffer.end(),dest);
		  dest += 8;
	  } else if(i == start_index + 20 && (i*8) + 8 > scx_end){
		  std::copy(line_buffer.begin(),line_buffer.end() - (scx_end - (i*8)),dest);
	  } else {
		  std::copy(line_buffer.begin(),line_buffer.end(),dest);
		  dest += 8;
	  }

  }

  int vert_size = 8;
  if((*mem_mgr)[LCDC] & (u8)LcdcFlags::obj_size){
	vert_size = 16;
  }

  for(auto & spr : state.spritesOnLine) {
	int top_line = spr.y_pos - 16;
	int bottom_line = top_line + vert_size;

	  render_line_of_tile(spr.address,bottom_line - (*mem_mgr)[LY],line_buffer);

  }


}

void Ppu::render_tile_to_colors(u16 address, std::array<u8,8*16>& tile_pixels)
{
	assert(address >= 0x8000 && address <= (0x9800 - 16));

	std::array<u8,8> line_pixels {0};

	for (int i = 0; i < (((*mem_mgr)[LCDC] & (u8)LcdcFlags::obj_size) ? 16 : 8); ++i)
	{
		render_line_of_tile(address,i,line_pixels);
		std::copy_n(line_pixels.begin(),8,tile_pixels.begin() + (i*8));
	}
}

void Ppu::render_line_of_tile(u16 address, u8 linenumber, std::array<u8,8>& line_pixels)
{
	assert(linenumber >= 0 && linenumber < 144);

	u8 line_a = (*mem_mgr)[address + (linenumber * 2)];
	u8 line_b = (*mem_mgr)[address + (linenumber * 2) + 1];

	std::generate_n(line_pixels.begin(),8,[j = 0, line_a = line_a, line_b = line_b]() mutable {
		return DefaultPalette[(((line_b >> j) & 1) << 1) + ((line_a >> j) & 1)];
	});

}




