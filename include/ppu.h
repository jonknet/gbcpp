#pragma once
#include "mm.h"
#include "SDL2/SDL.h"

class Ppu
{
private:
	SDL_Texture* sdl_tex;
public:
	Ppu(SDL_Texture* tex = nullptr);
	void settexture(SDL_Texture* tex);
	void draw();
};

extern Ppu* ppu;