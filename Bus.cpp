#pragma once
#include "include/interface/IBus.h"
#include "include/component/BusMessage.h"
#include <queue>
#include <vector>

namespace ModernBoy {

class Bus {
 public:
  Bus() = default;
 protected:
  void processQueue();
  friend class IBus;
 private:
  std::queue<std::reference_wrapper<BusMessage>,std::vector<std::reference_wrapper<BusMessage>>> msg_queue{};
  std::vector<std::reference_wrapper<BusMessage>> members{};
};

void Bus::processQueue() {

}

}
