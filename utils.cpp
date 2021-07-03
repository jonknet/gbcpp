#include "stddefs.h"
#include "utils.h"
#include <array>
#include <fstream>
#include "mm.h"
#include "fmt/include/fmt/core.h"
#include "fmt/include/fmt/format.h"

using namespace std;

void init_sdl(SDL_Window* win, SDL_Renderer* ren, SDL_Texture* tex, SDL_Window* windebug, SDL_Renderer* rendebug, SDL_Texture* texdebug) {
	win = SDL_CreateWindow("GBinCpp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, SDL_WINDOW_RESIZABLE);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 256, 256);

	if (DEBUG) {
		windebug = SDL_CreateWindow("Debugger", 100, 100, 256, 512, 0);
		rendebug = SDL_CreateRenderer(windebug, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		texdebug = SDL_CreateTexture(rendebug, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 256, 512);
	}
}

void cleanup_sdl(SDL_Texture* tex, SDL_Renderer* ren, SDL_Window* win, SDL_Texture* texdebug, SDL_Renderer* rendebug, SDL_Window* windebug) {
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	if (DEBUG) {
		SDL_DestroyTexture(texdebug);
		SDL_DestroyRenderer(rendebug);
		SDL_DestroyWindow(windebug);
	}
}

void outputhex(std::string filename) {
	fstream file;
	file.open(filename, fstream::out | fstream::trunc);

	if (!file) {
		spdlog::error("Error opening output hex file");
		return;
	}

	(*mm)[BOOT] = 1;

	static std::array<u8, 0x10000>* v = mm->getarray();

	for (int i = 0; i < 0x7EEE; i += 16) {
		file << fmt::format("{:<#04x} ", i);

		file << fmt::format("{:04x} {:04x} {:04x} {:04x} ", v->at(i), v->at(i + 1), v->at(i + 2), v->at(i + 3)) <<
			fmt::format("{:04x} {:04x} {:04x} {:04x} ", v->at(i + 4), v->at(i + 5), v->at(i + 6), v->at(i + 7)) <<
			fmt::format("{:04x} {:04x} {:04x} {:04x} ", v->at(i + 8), v->at(i + 9), v->at(i + 10), v->at(i + 11)) <<
			fmt::format("{:04x} {:04x} {:04x} {:04x} ", v->at(i + 12), v->at(i + 13), v->at(i + 14), v->at(i + 15)) << endl;
	}

	(*mm)[BOOT] = 0;

	file.close();
}

