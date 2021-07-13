//
// Created by Commvault on 7/12/2021.
//

#ifndef TESTS_INCLUDE_INTERFACE_SYSTEM_H_GAMEBOY_H_
#define TESTS_INCLUDE_INTERFACE_SYSTEM_H_GAMEBOY_H_

#include "ISystem.h"
namespace ModernBoy {

class GameBoy : public ISystem {

 public:
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual void Reset() = 0;
 protected:
  GameBoy() : clock_(), bus_(), state_(SystemState::Started) {}
};

}

#endif //TESTS_INCLUDE_INTERFACE_SYSTEM_H_GAMEBOY_H_
