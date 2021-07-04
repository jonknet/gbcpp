#pragma once
#ifndef __PPU__H
#define __PPU__H

#include "stddefs.h"
#include "SDL2/SDL.h"

namespace PpuNS
{
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
}

#endif