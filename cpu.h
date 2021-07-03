#pragma once

#include "stddefs.h"

#define _a *R.a
#define _f *R.f
#define _b *R.b
#define _c *R.c
#define _d *R.d
#define _e *R.e
#define _h *R.h
#define _l *R.l
#define _af *R.af
#define _bc *R.bc
#define _de *R.de
#define _hl *R.hl
#define _pc *R.pc
#define _sp *R.sp

#define SET(num,bit) (num |= (1 << bit))
#define CLR(num,bit) (num &= ~(1 << bit))
#define GET(num,bit) (num & (1 << bit))

#define ZCHK(num) ((num == 0) ? SET(*R.f,ZBIT) : CLR(*R.f,ZBIT))
#define HCHKADD(num,op) (((num & 0xF) + op > 0xF) ? SET(*R.f,HBIT) : CLR(*R.f,HBIT))
#define HCHKSUB(num,op) (((num & 0xF) < op) ? SET(*R.f,HBIT) : CLR(*R.f,HBIT))
#define HCHKADD16(num,op) (((((u16)num & 0xFF) + (u16)op) > 0xFF) ? SET(*R.f,HBIT) : CLR(*R.f,HBIT))
#define HCHKSUB16(num,op) (((num & 0xFF) < op) ? SET(*R.f,HBIT) : CLR(*R.f,HBIT))
#define CYCHKADD(num,op) (((u16)num + (u16)op > 0xFF) ? SET(*R.f,CBIT) : CLR(*R.f,CBIT))
#define CYCHKSUB(num,op) ((num < op) ? SET(*R.f,CBIT) : CLR(*R.f,CBIT))
#define CYCHKADD16(num,op) (((u32)num + (u32)op > 0xFFFF) ? SET(*R.f,CBIT) : CLR(*R.f,CBIT))

#define GET16 ((MM[(*R.pc)+2] << 8) | (MM[(*R.pc)+1]))
#define GET8 ((MM[(*R.pc)+1]))

#define POP8 (MM[(*R.sp)++])
#define POP16 (MM[(*R.sp)++] | (MM[(*R.sp)++] << 8))
#define PEEK8 (MM[(*R.sp)])
#define PEEK16 (MM[(*R.sp)]) | (MM[(*R.sp)+1] << 8)
#define PUSH8(val) (MM.set(--(*R.sp),val))
#define PUSH16(val) { MM.set(--(*R.sp), (val & 0xFF00) >> 8); MM.set(--(*R.sp), val & 0xFF); }

#define ZBIT 7
#define NBIT 6
#define HBIT 5
#define CBIT 4

class R {
private:
	u16 af_ = 0, bc_ = 0, de_ = 0, hl_ = 0, pc_ = 0, sp_ = 0;
public:
	u16* af, * bc, * de, * hl, * pc, * sp;
	u8* a, * f, * b, * c, * d, * e, * h, * l;
	R() {
		af_ = 0,
		bc_ = 0,
		de_ = 0, 
		hl_ = 0, 
		pc_ = 0, 
		sp_ = 0;
		af = &af_;
		bc = &bc_;
		de = &de_;
		hl = &hl_;
		pc = &pc_;
		sp = &sp_;
		a = (u8*)(af);
		f = (u8*)(af+1);
		b = (u8*)(bc);
		c = (u8*)(bc + 1);
		d = (u8*)(de);
		e = (u8*)(de+1);
		h = (u8*)(hl);
		l = (u8*)(hl+1);
	}
};

struct CpuState {
	u16 af, bc, de, hl, pc, sp;
};

class Cpu
{
private:
	R R;
	cycles_t cycles;
	bool ime;
	bool running;
	bool halted;
	bool stopped;
	void init();

	s16 lastop;

	void ld16(u16* t,u16 v);
	void ld_i(u16* t); 
	void ld(u8* t,u8 v); 
	void inc16(u16* t); 
	void inc(u8* t);  
	void dec16(u16* t); 
	void dec(u8* t); 
	void add16(u16 v);
	void add(u8 v); 
	void adc(u8 v); 
	void sub(u8 v); 
	void sbc(u8 v); 
	void _and(u8 v);
	void _xor(u8 v);
	void _or(u8 v);
	void cp(u8 v); 
	void daa();
	void ccf();
	void jr(bool c); 
	void jp(bool c); 
	void call(bool c); 
	void ret(bool c); 
	void rst(u8 v); 
	void pop(u16* t); 
	void push(u16 v); 
	void rlc(u8* t); 
	void rrc(u8* t); 
	void rl(u8* t);
	void rlca();
	void rrca();
	void rla();
	void rra();
	void rr(u8* t); 
	void sla(u8* t); 
	void sra(u8* t); 
	void swap(u8* t); 
	void srl(u8* t); 
	void bit(u8 v,int b); 
	void res(u8* t,int b); 
	void set(u8* t,int b);

public:

	Cpu();
	void exec();
	void run();
	void pause();
	void sethalt(bool h);
	void setstop(bool s);
	void reset();

	s16 getlastop();
	CpuState getstate();

};

extern Cpu cpu;
