// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com



#include "lib/spdlog/spdlog.h"
#include "lib/spdlog/sinks/rotating_file_sink.h"
#include "lib/argh.h"
#include "lib/SDL2/SDL.h"
#include <string>
#include <filesystem>
#include "lib/portable-file-dialogs.h"



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


  spdlog::shutdown();

  return 0;

};
