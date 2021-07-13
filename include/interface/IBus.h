#pragma once

#include <memory>
#include "../lib/uuid.h"
#include "Bus.cpp"
#include <vector>

namespace ModernBoy {

enum class MessageState {
  SUCCESSFUL,
  FAIL_INCOMPATIBLE,
  FAIL_UNKNOWN
};

class IBus {
 public:
  auto get_uuid() { return uuid; }
  MessageState send(BusMessage& msg);
 protected:
  IBus(Bus& bus) : bus_{bus} {} = default;
  virtual void receive(BusMessage& msg) = 0;
 private:
  Bus& bus_;
  const auto uuid = uuids::uuid_system_generator{}();
};

  auto IBus::bus_{};

  MessageState IBus::send(BusMessage &msg) {
	bus_.members.push_back(this);
	bus_.msg_queue.push(msg);
  }

}



