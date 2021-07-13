#pragma once

namespace ModernBoy {
// Forward Declarations
class Cpu;
class Mmu;
class Ppu;
class Clock;
class MessageBus;

enum class SystemState {
  Started,
  Running,
  Stopped
};

/* Abstract class representing any systems interface */
  class ISystem { ;
   public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Reset() = 0;
   protected:
	ISystem() = delete;
	virtual void main_loop() = 0;
   private:
	auto state_ = SystemState::Stopped;
	static Bus bus_;
  };

  auto ISystem::bus_{};

}