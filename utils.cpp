#include "stddefs.h"
#include "spdlog/spdlog.h"
#include "utils.h"
#include <array>
#include <fstream>
#include "fmt/core.h"
#include "fmt/format.h"
#include "SDL2/SDL.h"
#include <cstdarg>

using namespace GBCPP;

void init_sdl(SDL_Window*& win,
			  SDL_Renderer*& ren,
			  SDL_Texture*& tex,
			  SDL_Window*& windebug,
			  SDL_Renderer*& rendebug,
			  SDL_Texture*& texdebug) {

  if (SDL_Init(SDL_INIT_EVERYTHING)!=0) {
	spdlog::error("SDL_Init failure");
	assert(0);
  }
  win = SDL_CreateWindow("GBinCpp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, SDL_WINDOW_RESIZABLE);
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 256, 256);

  if (DEBUG) {
	windebug = SDL_CreateWindow("Debugger", 100, 100, 256, 512, 0);
	rendebug = SDL_CreateRenderer(windebug, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	texdebug = SDL_CreateTexture(rendebug, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 256, 512);
  }
}

void cleanup_sdl(SDL_Texture *tex,
				 SDL_Renderer *ren,
				 SDL_Window *win,
				 SDL_Texture *texdebug,
				 SDL_Renderer *rendebug,
				 SDL_Window *windebug) {
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  if (DEBUG) {
	SDL_DestroyTexture(texdebug);
	SDL_DestroyRenderer(rendebug);
	SDL_DestroyWindow(windebug);
  }
}

void outputhex(const std::string& filename) {
  std::fstream file;
  file.open(filename, std::fstream::out | std::fstream::trunc);

  if (!file) {
	spdlog::error("Error opening output hex file");
	return;
  }

//

  static std::array<u8, 0x10000> *v = nullptr;

  for (int i = 0; i < 0x7EEE; i += 16) {
	file << fmt::format("{:<#04x} ", i);

	file << fmt::format("{:04x} {:04x} {:04x} {:04x} ", v->at(i), v->at(i + 1), v->at(i + 2), v->at(i + 3)) <<
		 fmt::format("{:04x} {:04x} {:04x} {:04x} ", v->at(i + 4), v->at(i + 5), v->at(i + 6), v->at(i + 7)) <<
		 fmt::format("{:04x} {:04x} {:04x} {:04x} ", v->at(i + 8), v->at(i + 9), v->at(i + 10), v->at(i + 11)) <<
		 fmt::format("{:04x} {:04x} {:04x} {:04x} ", v->at(i + 12), v->at(i + 13), v->at(i + 14), v->at(i + 15))
		 << std::endl;
  }

  //mem[BOOT] = 0;

  file.close();
}

void lookup_registry_name_by_address(const void* address, Registers& regs){
  if(address == (void*)regs.a){

  }
}

