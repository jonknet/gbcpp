
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
  if(pImpl->s.current_state == OpState::RUNNING){
    if(pImpl->s.remaining_cycles == 4){
      pImpl->s.current_state = OpState::IDLE;
    }
    pImpl->s.remaining_cycles -= 4;
  } else if(pImpl->s.current_state == OpState::IDLE){
    pImpl->s.current_state = OpState::RUNNING;
    if(((*pImpl->m)[pImpl->r.pc]) == 0xCB){
      pImpl->s.remaining_cycles = 4 + cbDefinesTbl[((*pImpl->m)[pImpl->r.pc+1])].cycles[0];
    } else {
      pImpl->s.remaining_cycles = opDefinesTbl[((*pImpl->m)[pImpl->r.pc])].cycles[0];
    }
    exec();
  }
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

