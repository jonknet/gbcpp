#pragma once

#include "stddefs.h"
#include "ops.h"



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


namespace Cpu
{

	enum Flags
	{
		Z = 0x80,
		N = 0x40,
		H = 0x20,
		C = 0x10
	};

	enum CheckOp
	{
		none = 0,
		add = 1,
		sub = 2,
	};

	enum HelperType
	{
		out = 1,
		in = 2
	};

	typedef struct
	{
		cycles_t cycles;
		bool ime;
		bool running;
		bool halted;
		bool stopped;
		s16 lastop;
	} State;





	void _zero_chk(u16 v1, u16 v2 = 0){
		if(v1) { return false; }
		else { return true; }
	}



	class Registers
	{
	private:
		u16 af_, bc_, de_, hl_, pc_, sp_ = { 0 };
	public:
		u16* af, * bc, * de, * hl, * pc, * sp;
		u8* a, * f, * b, * c, * d, * e, * h, * l;

		Registers()
		{
			af = &af_;
			bc = &bc_;
			de = &de_;
			hl = &hl_;
			pc = &pc_;
			sp = &sp_;
			a = (u8*)(af);
			f = (u8*)(af + 1);
			b = (u8*)(bc);
			c = (u8*)(bc + 1);
			d = (u8*)(de);
			e = (u8*)(de + 1);
			h = (u8*)(hl);
			l = (u8*)(hl + 1);
		}
	};

	class Cpu
	{
	private:
		Registers r;
		State s;
		friend Opcodes;
		void init();
	public:
		Cpu();
		void exec();
		void run();
		void pause();
		void sethalt(bool h);
		void setstop(bool s);
		void reset();
		s16 getlastop();
		const Registers& getregisters();
	};

	extern Cpu* cpu;

}


