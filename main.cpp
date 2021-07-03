#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "argh.h"
#include "mm.h"
#include "cpu.h"
#include "ppu.h"
#include "SDL2/SDL.h"
#include <string>
#include "customflags.h"

void init_sdl(SDL_Window* win, SDL_Renderer* ren, SDL_Texture* tex, SDL_Window* windebug, SDL_Renderer* rendebug, SDL_Texture* texdebug);
void cleanup_sdl(SDL_Texture* tex, SDL_Renderer* ren, SDL_Window* win, SDL_Texture* texdebug, SDL_Renderer* rendebug, SDL_Window* windebug);

bool DEBUGMODE = false;

int main(int argc, char* argv[]) {

	// Logger

	auto formatter = std::make_unique<spdlog::pattern_formatter>();
	formatter->add_flag<registers_flag>('r');
	formatter->add_flag<opcode_flag>('*').set_pattern("[%T:%e] [%L] [%*] %v [%r]");
	spdlog::set_formatter(std::move(formatter));

	auto max_size = 1048576 * 10;
	auto max_files = 1;
	auto logger = spdlog::rotating_logger_mt("logger", "app.log", max_size, max_files,true);
	logger->flush_on(spdlog::level::info);
	spdlog::set_default_logger(logger);

	// Cmd line parsing

	argh::parser cmdl(argv);

	if (cmdl["-d"]) DEBUGMODE = true;

	std::string rom;
	if (!(cmdl("--file") >> rom)) {
		spdlog::error("Must provide rom name (-f romfile)");
		return 1;
	}

	// SDL

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		spdlog::error("SDL_Init failure");
	}

	SDL_Window* win = NULL, * windebug = NULL;
	SDL_Renderer* ren = NULL, * rendebug = NULL;
	SDL_Texture* tex = NULL, * texdebug = NULL;

	init_sdl(win, ren, tex, windebug, rendebug, texdebug);

	ppu.settexture(tex);

	if (MM.loadrom(rom)) {
		cleanup_sdl(tex, ren, win, texdebug, rendebug, windebug);
		SDL_Quit();
		return 2;
	}

	int quit = 0;

	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			}
		}

		ppu.draw();
		cpu.exec();
	}

	cleanup_sdl(tex, ren, win, texdebug, rendebug, windebug);

	SDL_Quit();

	spdlog::shutdown();

	return 0;

}

void init_sdl(SDL_Window* win, SDL_Renderer* ren, SDL_Texture* tex, SDL_Window* windebug, SDL_Renderer* rendebug, SDL_Texture* texdebug) {


	win = SDL_CreateWindow("GBinCpp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, SDL_WINDOW_RESIZABLE);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 256, 256);

	if (DEBUGMODE) {
		windebug = SDL_CreateWindow("Debugger", 100, 100, 256, 512, 0);
		rendebug = SDL_CreateRenderer(windebug, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		texdebug = SDL_CreateTexture(rendebug, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 256, 512);
	}
}

void cleanup_sdl(SDL_Texture* tex, SDL_Renderer* ren, SDL_Window* win, SDL_Texture* texdebug, SDL_Renderer* rendebug, SDL_Window* windebug) {
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	if (DEBUGMODE) {
		SDL_DestroyTexture(texdebug);
		SDL_DestroyRenderer(rendebug);
		SDL_DestroyWindow(windebug);
	}
}