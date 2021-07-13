#pragma once
#include <cstdint>
#include <optional>

namespace ModernBoy
{
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	using s8 = int8_t;
	using s16 = int16_t;

	template<typename T> using func_t = T(*)();
	template<typename T = void,typename U = void> using Tfunc = T(*)(U const &);
}
