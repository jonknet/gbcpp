#pragma once
#include <cstdint>
#include <array>
#include <string>

namespace GBCPP
{

	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	using s8 = int8_t;
	using s16 = int16_t;
	template<auto S> using mem_t = std::array<u8, S>;
	using gb_mem = mem_t<0x10000>;

	static u8 null = 0;
	static u16 null16 = 0;

	extern bool DEBUG;
	extern std::string log_buffer;
}
