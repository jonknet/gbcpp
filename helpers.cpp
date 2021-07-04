//
// Created by Jon on 7/3/2021.
//

#include <cpu.h>
#include "stddefs.h"
#include "helpers.h"


/*
#define ZCHK(num) ((num == 0) ? SET(*R.f,ZBIT) : CLR(*R.f,ZBIT))
#define HCHKADD(num, op) (((num & 0xF) + op > 0xF) ? SET(*R.f,HBIT) : CLR(*R.f,HBIT))
#define HCHKSUB(num, op) (((num & 0xF) < op) ? SET(*R.f,HBIT) : CLR(*R.f,HBIT))
#define HCHKADD16(num, op) (((((u16)num & 0xFF) + (u16)op) > 0xFF) ? SET(*R.f,HBIT) : CLR(*R.f,HBIT))
#define HCHKSUB16(num, op) (((num & 0xFF) < op) ? SET(*R.f,HBIT) : CLR(*R.f,HBIT))
#define CYCHKADD(num, op) (((u16)num + (u16)op > 0xFF) ? SET(*R.f,CBIT) : CLR(*R.f,CBIT))
#define CYCHKSUB(num, op) ((num < op) ? SET(*R.f,CBIT) : CLR(*R.f,CBIT))
#define CYCHKADD16(num, op) (((u32)num + (u32)op > 0xFFFF) ? SET(*R.f,CBIT) : CLR(*R.f,CBIT))

#define GET16 ((mm[(*R.pc)+2] << 8) | (mm[(*R.pc)+1]))
#define GET8 ((mm[(*R.pc)+1]))
#define POP8 (MM[(*R.sp)++])
#define POP16 (mm[(*R.sp)++] | (mm[(*R.sp)++] << 8))
#define PEEK8 (MM[(*R.sp)])
#define PEEK16 (mm[(*R.sp)]) | (mm[(*R.sp)+1] << 8)
#define PUSH8(val) (MM.set(--(*R.sp),val))
#define PUSH16(val) { mm.set(--(*R.sp), (val & 0xFF00) >> 8); mm.set(--(*R.sp), val & 0xFF); }
*/


using namespace HelpersNS;

u8 get8(MemNS::MemMgr& memory,u16& ptr){
	return memory[ptr+1];
}

u8 pop8(MemNS::MemMgr& memory,u16& stackptr){
	return memory[stackptr++];
}

u8 peek8(MemNS::MemMgr& memory,u16& stackptr){
	return memory[stackptr];
}

u16 get16(MemNS::MemMgr& memory,u16& ptr){
	return ((memory[ptr+2] << 8) | (memory[ptr+1]));
}

u16 pop16(MemNS::MemMgr& memory,u16& ptr){
	return (memory[(ptr)++] | (memory[(ptr)++] << 8));
}

u16 peek16(MemNS::MemMgr& memory,u16& ptr){
	return (memory[ptr] | (memory[ptr+1] << 8));
}

void set8(CpuNS::Flags flag){

}

bool set(u8& flagptr, CpuNS::Flags flag){
	flagptr |= flag;
};

void clr(u8& flagptr, CpuNS::Flags flag){
	flagptr &= ~flag;
}

bool get(u8& flagptr, CpuNS::Flags flag)
{
	return (flagptr & flag) ? true : false;
}

