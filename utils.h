#pragma once
#include "SDL2-2.0.14/include/SDL.h"
#include <string>

void init_sdl(SDL_Window* win, SDL_Renderer* ren, SDL_Texture* tex, SDL_Window* windebug, SDL_Renderer* rendebug, SDL_Texture* texdebug);
void cleanup_sdl(SDL_Texture* tex, SDL_Renderer* ren, SDL_Window* win, SDL_Texture* texdebug, SDL_Renderer* rendebug, SDL_Window* windebug);
void outputhex(std::string filename);