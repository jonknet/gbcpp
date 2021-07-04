//
// Created by Jon on 7/3/2021.
//

#include "stddefs.h"
#include "helpers.h"

using namespace CpuNS;

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

