#include "stddefs.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "argh.h"
#include "SDL2/SDL.h"
#include "utils.h"
#include "mm.h"
#include "cpu.h"
#include "ppu.h"
#include <string>

using namespace GBCPP;

namespace GBCPP
{
	bool DEBUG = false;

	MemMgr* mem = new MemMgr();
	Ppu* ppu = new Ppu();
	Cpu* cpu = new Cpu(*mem);
}

int main(int argc, char *argv[]) {

  // Logger

  auto formatter = std::make_unique<spdlog::pattern_formatter>();
  formatter->set_pattern("[%T:%e] [%L] [%*] %v");
  spdlog::set_formatter(std::move(formatter));

  auto max_size = 1048576*10;
  auto max_files = 1;
  auto logger = spdlog::rotating_logger_mt("logger", "app.log", max_size, max_files, true);
  logger->flush_on(spdlog::level::info);
  spdlog::set_default_logger(logger);

  // Cmd line parsing

  argh::parser cmdl(argv);

  if (cmdl["-d"])
	DEBUG = true;

  std::string rom;
  if (!(cmdl("--file") >> rom)) {
	spdlog::error("Must provide rom name (-f romfile)");
	return 1;
  }

  // SDL

  if (SDL_Init(SDL_INIT_EVERYTHING)!=0) {
	spdlog::error("SDL_Init failure");
  }

  SDL_Window *win = nullptr, *windebug = nullptr;
  SDL_Renderer *ren = nullptr, *rendebug = nullptr;
  SDL_Texture *tex = nullptr, *texdebug = nullptr;

  init_sdl(win, ren, tex, windebug, rendebug, texdebug);

  // Init modules

  if (GBCPP::MemMgr::load_rom(rom, mem)) {
	cleanup_sdl(tex, ren, win, texdebug, rendebug, windebug);
	SDL_Quit();
	return 2;
  }

  int quit = 0;

  while (!quit) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
	  switch (event.type) {
	  case SDL_QUIT: quit = 1;
		break;
	  }
	}

	ppu->draw();
	cpu->exec();
  }

  cleanup_sdl(tex, ren, win, texdebug, rendebug, windebug);

  SDL_Quit();

  spdlog::shutdown();

  return 0;

}