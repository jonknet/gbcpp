//
// Created by Jon on 7/3/2021.
//

#ifndef GBCPPCLION_HELPERS_H
#define GBCPPCLION_HELPERS_H

#include <stddefs.h>
#include <array>

using namespace CpuNS;

    template<typename T = u8> using BitHelper = bool (Flags flag, T &value);
    template<typename T = u8> using CheckHelper = void (T value1, T value2);
    template<typename T = u8> using MemoryHelper = T (std::array<u8, 0x10000> &array);

    extern MemoryHelper<> get8,
            pop8,
            peek8,
            push8;

    extern MemoryHelper<u16> get16,
            pop16,
            peek16,
            push16;

    extern BitHelper<> set, clr, get;

    CheckHelper<> zero_check;
    CheckHelper<> halfcy_check_add;
    CheckHelper<> halfcy_check_sub;
    CheckHelper<u16> halfcy16_check_add;
    CheckHelper<u16> halfcy16_check_sub;
    CheckHelper<> carry_check_add;
    CheckHelper<> carry_check_sub;
    CheckHelper<u16> carry16_check_add;
    CheckHelper<u16> carry16_check_sub;

#endif //GBCPPCLION_HELPERS_H


