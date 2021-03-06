// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#include "stddefs.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "argh.h"
#include "SDL2/SDL.h"
#include "utils.h"
#include "mm.h"
#include "cpu.h"
#include "system/Ppu.h"
#include <string>
#include <filesystem>
#include "portable-file-dialogs.h"
#include "Instruction.h"

bool ModernBoy::DEBUG = false;
std::string ModernBoy::log_buffer;

int main(int argc, char *argv[]) {

  // Logger

  if (std::filesystem::exists(std::filesystem::path("logs"))) {
	std::filesystem::remove_all(std::filesystem::path("logs"));

  }
  std::filesystem::create_directory(std::filesystem::path("logs"));

  spdlog::set_pattern("[%T:%e] [%L] %v \r\n");
  auto max_size = 1048576 * 10;
  auto max_files = 5;
  auto logger = spdlog::rotating_logger_mt("logger", "logs/app.log", max_size, max_files, false);
  auto memlog = spdlog::rotating_logger_mt("memlog", "logs/mem.log", max_size, max_files, false);
  spdlog::set_default_logger(logger);

  // Cmd line parsing

  argh::parser cmdl(argv);

  if (cmdl["-d"])
	DEBUG = true;

  std::string rom;
  auto selection = pfd::open_file("Select a file", ".",
								  {"Game Boy Rom Files", "*.gb",
								   "All Files", "*"},
								  pfd::opt::multiselect).result();
  assert(!selection.empty());
  rom = selection[0];
  spdlog::info("Selected: {}", rom);

  // SDL

  SDL_Window *win = nullptr, *windebug = nullptr;
  SDL_Renderer *ren = nullptr, *rendebug = nullptr;
  SDL_Texture *tex = nullptr, *texdebug = nullptr;

  init_sdl(win, ren, tex, windebug, rendebug, texdebug);

  // Init modules

  auto *mem = new MemMgr();
  auto *ppu = new Ppu(mem, tex);
  auto *cpu = new Cpu(*mem);

  // Load ROM into memory

  if (GBCPP::MemMgr::load_rom(rom, mem)) {
	cleanup_sdl(tex, ren, win, texdebug, rendebug, windebug);
	SDL_Quit();
	return 2;
  }

  // Main Loop

  int quit = 0;

  while (!quit) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
	  switch (event.type) {
		case SDL_QUIT: quit = 1;
		  break;
	  }
	}

	ppu->tick();
	cpu->tick();

	if ((*mem)[SC] == 0x81) {
	  std::cout << (*mem)[SB];
	}
  }

  cleanup_sdl(tex, ren, win, texdebug, rendebug, windebug);

  SDL_Quit();

  spdlog::shutdown();

  return 0;

};
