//
// Created by Jon on 7/4/2021.
//
#pragma once
#ifndef GBCPPCLION_CPUIMPL_H
#define GBCPPCLION_CPUIMPL_H

#include "stddefs.h"
#include "cpu.h"
#include "mm.h"

namespace CpuImplNS
{

	class CpuImpl
	{
	private:
		CpuNS::Registers& r;
		CpuNS::StateType& s;
		static MemNS::MemMgr& m;
		struct
		{
			u8& a, & b, & c, & d, & e, & h, & l, & f;
			u16& af, & bc, & de, & hl, & sp, & pc;
		};

		void lookup_and_execute();

		void ld16(u16& t, u16 v);
		void ld_i(u16& t);
		void ld(u8& t, u8 v);
		void inc16(u16& t);
		void inc(u8& t);
		void dec16(u16& t);
		void dec(u8& t);
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
		void pop(u16& t);
		void push(u16 v);
		void rlc(u8& t);
		void rrc(u8& t);
		void rl(u8& t);
		void rlca();
		void rrca();
		void rla();
		void rra();
		void rr(u8& t);
		void sla(u8& t);
		void sra(u8& t);
		void swap(u8& t);
		void srl(u8& t);
		void bit(u8 v, int b);
		void res(u8& t, int b);
		void set(u8& t, int b);
	public:
		CpuImpl(CpuNS::Registers& r,CpuNS::StateType& s,MemNS::MemMgr& m);
	};

}



#endif //GBCPPCLION_CPUIMPL_H
