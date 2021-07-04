#pragma once
#ifndef __STDDEFS__H
#define __STDDEFS__H

#include <array>

	typedef unsigned char u8;

	typedef unsigned short u16;

	typedef unsigned int u32;

	typedef signed char s8;

	typedef signed short s16;

	typedef unsigned short opc;

	typedef unsigned long long cycles_t;

	using gb_memory = std::array<u8, 0x10000>;

	static u8 null = 0;

	static u16 null16 = 0;

	struct OpDef {
		int cycles[2];
		int length;
	};

#endif