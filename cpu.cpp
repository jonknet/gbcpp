#include "cpu.h"
#include "mm.h"
#include "ops.h"

Cpu::Cpu() {
	init();
}

void Cpu::init() {
	*R.af = 0x01B0;
	*R.bc = 0x0013;
	*R.de = 0x00D8;
	*R.hl = 0x014D;
	*R.pc = 0;
	*R.sp = 0xFFFE;
	ime = true;
	cycles = 0;
	lastop = -1;  
	halted = false;
	stopped = false;
	running = false;
}

void Cpu::run() {
	running = true;
}

void Cpu::pause() {
	running = false;
}

void Cpu::sethalt(bool h) {
	halted = h;
}

void Cpu::setstop(bool s) {
	stopped = s;
}

void Cpu::reset() {
	init();
}

s16 Cpu::getlastop() {
	return lastop;
}
CpuState Cpu::getstate() {
	CpuState state = { _af,_bc,_de,_hl,_pc,_sp };
	return state;
}

void Cpu::exec() {
	lastop = mm[_pc];
	//spdlog::info("OP{0:x} PC{1:x}", mm[_pc], _pc);

	#include "opswitch.inc"

	_pc += ((mm[_pc] != 0xCB) ? opDefinesTbl[mm[_pc]].length : cbDefinesTbl[mm[_pc + 1]].length);

	cycles += ((mm[_pc] != 0xCB) ? opDefinesTbl[mm[_pc]].cycles[0] : cbDefinesTbl[mm[_pc + 1]].cycles[0]);
	
}

Cpu* cpu;