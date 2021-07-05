#include "stddefs.h"
#include "cpu.h"
#include "mm.h"
#include <cassert>

using namespace GBCPP;

Cpu::Cpu(GBCPP::MemMgr &mm) {
  pImpl = new CpuImpl(mm);
}
void Cpu::run() {
  pImpl->state->running = true;
}
void Cpu::pause() {
  pImpl->state->running = false;
}
void Cpu::sethalt(bool h) {
  pImpl->state->halted = h;
}
void Cpu::setstop(bool st) {
  pImpl->state->stopped = st;
}
void Cpu::reset() {

}
StateType Cpu::getstate() {
  return *pImpl->state;
}
#define mm (*memmgr)
void Cpu::exec() {
  //spdlog::info("OP{0:x} PC{1:x}", mm[_pc], _pc);
  pImpl->lookup_and_execute();
}
Registers Cpu::getregisters() {
  return *pImpl->reg;
}