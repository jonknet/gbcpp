//
// Created by Jon on 7/3/2021.
//

#pragma once

#ifndef __HELPERS__H
#define __HELPERS__H

#include <stddefs.h>
#include <cpu.h>
#include "mm.h"
#include <array>

namespace HelpersNS
{

	template<typename T> using BitHelper = T(u8& flagptr, CpuNS::Flags flag);
	template<class T = u8> using CheckHelper = void(T value1, T value2, u8& flagreg);
	template<class T = u8> using MemoryHelper = T(MemNS::MemMgr& memory,u16& regptr);
	template<class T = u8> using PushHelper = void(T val, MemNS::MemMgr& memory, u16& ptr);

	MemoryHelper<u8> 	get8,pop8,peek8;
	MemoryHelper<u16> 	get16,pop16,peek16;
	PushHelper<u8> push8;
	PushHelper<u16> push16;
	BitHelper<void> set,clr;
	BitHelper<bool> get;



	static void zero_check(u16 val, u8& flag);
	static CheckHelper<> halfcy_check_add;
	static CheckHelper<> halfcy_check_sub;
	static CheckHelper<u16> halfcy16_check_add;
	static CheckHelper<u16> halfcy16_check_sub;
	static CheckHelper<> carry_check_add;
	static CheckHelper<> carry_check_sub;
	static CheckHelper<u16> carry16_check_add;
	static CheckHelper<u16> carry16_check_sub;

}

#endif


