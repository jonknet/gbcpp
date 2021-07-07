#pragma once
#include "SDL2/SDL.h"

namespace GBCPP {
class Ppu {
private:
  SDL_Texture *sdl_tex;
public:
  explicit Ppu(SDL_Texture *tex);
  void draw();
};

}
