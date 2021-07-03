//
// Created by Jon on 7/3/2021.
//

#ifndef GBCPPCLION_HELPERS_H
#define GBCPPCLION_HELPERS_H

#include <array>
#include <stddefs.h>
#include <cpu.h>

using BitHelper = bool ()(Cpu::Flags flag, u8& value);
template<typename T> using CheckHelper = void ()(T value1, T value2);
template<typename T> using MemoryHelper = T ()(std::array<T, 0x10000>& array);

extern MemoryHelper<u8> 	get8,
		pop8,
		peek8,
		push8;
extern MemoryHelper<u16> 	get16,
		pop16,
		peek16,
		push16;

extern BitHelper set,clr,get;

CheckHelper<u16> zero_check;
CheckHelper<u8> halfcy_check_add;
CheckHelper<u8> halfcy_check_sub;
CheckHelper<u16> halfcy16_check_add;
CheckHelper<u16> halfcy16_check_sub;
CheckHelper<u8> carry_check_add;
CheckHelper<u8> carry_check_sub;
CheckHelper<u16> carry16_check_add;
CheckHelper<u16> carry16_check_sub;

#endif //GBCPPCLION_HELPERS_H
