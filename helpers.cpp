//
// Created by Jon on 7/3/2021.
//

#include "stddef.h"
#include "helpers.h"
#include "cpu.h"

using namespace Cpu;



bool set(Flags flag, u8& value){
	value |= flag;
};

void clr(Flags flag, u8& value){
	value &= ~flag;
}

bool get(Flags flag, u8 value)
{
	if (!value)
	{ return false; }
	if (value &= flag > 0)
	{ return true; }
}

