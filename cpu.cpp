
#include "cpu.h"
#include "stddefs.h"
#include "mm.h"
#include "ops.h"
#include "cpuimpl.h"
#include <cassert>

using namespace GBCPP;

Cpu::Cpu(GBCPP::MemMgr &mm) {
  pImpl = new CpuImpl(&mm);
}

void Cpu::tick(){

}

void Cpu::run() {
  pImpl->s.running = true;
}
void Cpu::pause() {
  pImpl->s.running = false;
}
void Cpu::sethalt(bool h) {
  pImpl->s.halted = h;
}
void Cpu::setstop(bool st) {
  pImpl->s.stopped = st;
}
void Cpu::reset() {

}
CpuState Cpu::getstate() {
  return pImpl->s;
}

void Cpu::exec() {
  //spdlog::info("OP{0:x} PC{1:x}", mm[_pc], _pc);
  pImpl->lookup_and_execute();
}
Registers Cpu::getregisters() {
  return pImpl->r;
}

