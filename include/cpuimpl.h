//
// Created by Jon on 7/4/2021.
//
#pragma once
#ifndef GBCPPCLION_CPUIMPL_H
#define GBCPPCLION_CPUIMPL_H

#include "stddefs.h"
#include "cpu.h"
#include "mm.h"

namespace GBCPP {
struct StateType;
class Registers;

class CpuImpl {
private:
  friend class Cpu;
  Registers *reg;
  StateType *state;
  static GBCPP::MemMgr &m;

public:
  explicit CpuImpl(GBCPP::MemMgr &memmgr);

private:

  u8 &a, &b, &c, &d, &e, &h, &l, &f;
  u16 &af, &bc, &de, &hl, &sp, &pc;

  // Memory Helpers
  static inline u8 get8(u16 address) {
	return m[address];
  }

  static inline u16 get16(u16 address) {
	return ((m[address + 1] << 8) | (m[address]));
  }

  // Stack Helpers

  inline u8 pop8() {
	return m[sp++];
  }

  [[nodiscard]] inline u8 peek8() const {
	return m[sp];
  }

  inline void push8(u8 val) {
	m[--sp] = val;
  }

  [[nodiscard]] inline u16 peek16() const {
	return (m[sp] | (m[sp + 1] << 8));
  }

  inline u16 pop16() {
	u16 out = peek16();
	sp += 2;
	return out;
  }

  inline void push16(u16 val) {
	m[--sp] = (val & 0xFF00) >> 8;
	m[--sp] = val & 0xFF;
  }

  // Bit Flag Helpers

  inline void setf(Flags flags) {
	f |= flags;
  };

  inline void clrf(Flags flags) {
	f &= ~flags;
  }

  [[nodiscard]] inline bool getf(Flags flags) const {
	return (f & flags)!=0;
  }

  // Flag Checker Helpers

  inline void zero_check(u16 val) {
	if (!val) {
	  setf(Z);
	} else {
	  clrf(Z);
	}
  }

  inline void half_carry_check_add(u8 op1, u8 op2) {
	if (((op1 & 0xF) + (op2 & 0xF) & 0x10)==0x10) {
	  setf(H);
	} else {
	  clrf(H);
	}
  }

  inline void half_carry_check_sub(u8 op1, u8 op2) {
	if ((op1 & 0xF) < op2) {
	  setf(H);
	} else {
	  clrf(H);
	}
  }

  inline void half_carry_check_add16(u16 op1, u16 op2) {
	if ((((op1 & 0xFF) + (op2 & 0xFF)) & 0x100)==0x100) {
	  setf(H);
	} else {
	  clrf(H);
	}
  }

  inline void half_carry_check_sub16(u16 op1, u16 op2) {
	if ((op1 & 0xFF) < op2) {
	  setf(H);
	} else {
	  clrf(H);
	}
  }

  inline void carry_check_add(u16 op1, u16 op2) {
	if (((op1 & 0xFF) + (op2 & 0xFF)) > 0xFF) {
	  setf(C);
	} else {
	  clrf(C);
	}
  }

  inline void carry_check_sub(u8 op1, u8 op2) {
	if ((op1 & 0xF) < (op2 & 0xF)) {
	  setf(C);
	} else {
	  clrf(C);
	}
  }

  inline void carry_check_add16(u32 op1, u32 op2) {
	u32 val = ((op1 & 0xFFFF) + (op2 & 0xFFFF));
	if ((val > 0xFFFF) || (val < op1)) {
	  setf(C);
	} else {
	  clrf(C);
	}
  }

  // Member Definitions
  void lookup_and_execute();

  // Microcode Definitions
  void ld16(u16 &t, u16 v);
  void ld_i(u16 &t);
  void ld(u8 &t, u8 v);
  void inc16(u16 &t);
  void inc(u8 &t);
  void dec16(u16 &t);
  void dec(u8 &t);
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
  void pop(u16 &t);
  void push(u16 v);
  void rlc(u8 &t);
  void rrc(u8 &t);
  void rl(u8 &t);
  void rlca();
  void rrca();
  void rla();
  void rra();
  void rr(u8 &t);
  void sla(u8 &t);
  void sra(u8 &t);
  void swap(u8 &t);
  void srl(u8 &t);
  void bit(u8 v, int b);
  void res(u8 &t, int b);
  void set(u8 &t, int b);
};

}

#endif //GBCPPCLION_CPUIMPL_H
