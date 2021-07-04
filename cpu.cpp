#include "stddefs.h"

using namespace CpuNS;

Cpu::Cpu() : s{ 0 }
{
	af = 0x01B0;
	bc = 0x0013;
	de = 0x00D8;
	hl = 0x014D;
	pc = 0;
	sp = 0xFFFE;
	s.ime = true;
	s.cycles = 0;
	s.lastop = -1;
	s.halted = false;
	s.stopped = false;
	s.running = false;
}
Cpu::Cpu(MemMgr* mm) : Cpu()
{
	if (mm == nullptr)
	{ static_assert(true); }
	if (m == nullptr)
	{ m = mm; }
}
void Cpu::run()
{
	s.running = true;
}
void Cpu::pause()
{
	s.running = false;
}
void Cpu::sethalt(bool h)
{
	s.halted = h;
}
void Cpu::setstop(bool st)
{
	s.stopped = st;
}
void Cpu::reset()
{
	init();
}
State& Cpu::getstate()
{
	return s;
}
#define mm (*memmgr)
void Cpu::exec()
{
	s.lastop = mm[0];
	//spdlog::info("OP{0:x} PC{1:x}", mm[_pc], _pc);

	lookup_and_execute();
	_pc += ((*mm[_pc] != 0xCB) ? opDefinesTbl[*mm[_pc]].length : cbDefinesTbl[*mm[_pc + 1]].length);
	cycles += ((*mm[_pc] != 0xCB) ? opDefinesTbl[*mm[_pc]].cycles[0] : cbDefinesTbl[*mm[_pc + 1]].cycles[0]);
}
Registers& Cpu::getregisters()
{
	return r;
}
static Cpu* cpu{ nullptr };