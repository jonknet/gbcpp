#ifndef STDDEFS_H
#define STDDEFS_H

#include <cstdint>
#include <array>



namespace GBCPP
{

	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	using s8 = int8_t;
	using s16 = int16_t;
	using mem_t = std::array<u8, 0x10000>;

	static u8 null = 0;
	static u16 null16 = 0;

	struct OpDef
	{
		int cycles[2];
		int length;
	};

	class Cpu;
	class MemMgr;
	class Ppu;

	extern bool DEBUG;

	extern MemMgr* mem;
	extern Ppu* ppu;
	extern Cpu* cpu;
}
#endif