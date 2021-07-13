#pragma once
#include "SDL2/SDL.h"
#include <string>
#include "stddefs.h"
#include "CpuImpl.h"

using namespace GBCPP;

void init_sdl(SDL_Window *&win, SDL_Renderer *&ren, SDL_Texture *&tex, SDL_Window *&windebug, SDL_Renderer *&rendebug,
			  SDL_Texture *&texdebug);
void cleanup_sdl(SDL_Texture *tex, SDL_Renderer *ren, SDL_Window *win, SDL_Texture *texdebug, SDL_Renderer *rendebug,
			SDL_Window *windebug);
void outputhex(const std::string& filename);
void lookup_registry_name_by_address(const void* address, Registers& regs);

