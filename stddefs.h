#pragma once

#include "spdlog/spdlog.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short opc;
typedef unsigned long long cycles_t;

struct OpDef {
	int cycles[2];
	int length;
};