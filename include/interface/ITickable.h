#pragma once

#include <stack>
#include <any>

namespace ModernBoy {

  class ITickable {
   public:
	virtual void tick() {}
   protected:
    ITickable() = delete;
  };

}