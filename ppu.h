#pragma once
#include "mm.h"
#include "SDL2-2.0.14/include/SDL.h"

class Ppu
{
private:
	SDL_Texture* sdl_tex;
public:
	Ppu();
	void settexture(SDL_Texture* tex);
	void draw();
};

extern Ppu ppu;