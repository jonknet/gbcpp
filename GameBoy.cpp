#pragma once
#include "interface/System.h"

namespace ModernBoy {
// Forward declaration
class Registers;

class GameBoy : System {
 public:
 protected:
  GameBoy() : clock_{},
              mmu_(),
              cpu_(),
              ppu_(),
              super() {};
 private:
  Mmu mmu_{};
  Cpu cpu_{};
  constexpr Ppu ppu_{};
  Registers reg_{};
  constexpr Clock clock_{};

};

}