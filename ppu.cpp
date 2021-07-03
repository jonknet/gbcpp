#include "ppu.h"

Ppu::Ppu(SDL_Texture* tex) : sdl_tex{tex} {
	if(tex == nullptr){
	    throw;
	}
}

void Ppu::draw() {

}

Ppu* ppu;