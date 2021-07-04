#pragma once
#ifndef __CPU__H
#define __CPU__H

#include "stddefs.h"
#include "ops.h"
#include "mm.h"
#include "cpuimpl.h"

namespace CpuNS
{

	typedef struct
	{
		cycles_t cycles;
		bool ime;
		bool running;
		bool halted;
		bool stopped;
		s16 lastop;
	} StateType;

	enum Flags
	{
		Z = 0x80, N = 0x40, H = 0x20, C = 0x10
	};
	enum CheckOp
	{
		none = 0, add = 1, sub = 2,
	};
	enum HelperType
	{
		out = 1, in = 2
	};




	class Registers
	{
	private:
		u16 af_, bc_, de_, hl_, pc_, sp_;
	public:
		u16* af, * bc, * de, * hl, * pc, * sp;
		u8* a, * f, * b, * c, * d, * e, * h, * l;

		Registers()
				:af_{ 0 }, bc_{ 0 }, de_{ 0 }, hl_{ 0 }, pc_{ 0 }, sp_{ 0 }
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
		CpuImplNS::CpuImpl* pCpuImpl {};
	public:
		Cpu();
		explicit Cpu(MemNS::MemMgr* mm);
		void exec();
		void run();
		void pause();
		void sethalt(bool h);
		void setstop(bool s);
		void reset();
		StateType& getstate();
		Registers& getregisters();
	};

	extern Cpu* cpu;

}

#endif