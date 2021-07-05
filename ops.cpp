#include "stddefs.h"
#include "spdlog/spdlog.h"
#include "cpuimpl.h"
#include <type_traits>

/* !!! FIXME: Fix this next */

using namespace GBCPP;

static u8 pre = 0;

void CpuImpl::ld16(u16 &t, u16 v) {
  spdlog::info("LD16 {:x},{:x}", t, v);
  t = v;
}

void CpuImpl::ld_i(u16 &t) {
  spdlog::info("LDI [{:x}],A({:x})", t, a);
	(*m)[t] = a;
}

void CpuImpl::ld(u8 &t, u8 v) {
  spdlog::info("LD {:x},{:x}", t, v);
  t = v;
}

void CpuImpl::inc16(u16 &t) {
  spdlog::info("INC16 {:x}", t);
  t += 1;
}

void CpuImpl::inc(u8& t) {
  spdlog::info("INC {:x}", t);
  half_carry_check_add(t, 1);
  t += 1;
  zero_check(t);
  clrf(N);
}

void CpuImpl::dec16(u16 &t) {
  spdlog::info("DEC16 {:x}", t);
  t -= 1;
}

void CpuImpl::dec(u8& t) {
  spdlog::info("DEC {:x}", t);
  half_carry_check_sub(t, 1);
  t -= 1;
  zero_check(t);
  setf(N);
}
void CpuImpl::add16(u16 v) {
  spdlog::info("ADD16 HL,{:x}", v);
  half_carry_check_add16(hl, v);
  carry_check_add16(hl, v);
  hl += v;
  clrf(N);
}

void CpuImpl::add(u8 v) {
  spdlog::info("ADD A,{:x}", v);
  half_carry_check_add(a, v);
  carry_check_add(a, v);
  a += v;
  zero_check(a);
  clrf(N);
}

void CpuImpl::adc(u8 v) {
  spdlog::info("ADC A,{:x}", v);
  half_carry_check_add(a, v + ((getf(C)) ? 1 : 0));
  carry_check_add(a, v + ((getf(C)) ? 1 : 0));
  a += v + ((getf(C)) ? 1 : 0);
  zero_check(a);
  clrf(N);
}

void CpuImpl::sub(u8 v) {
  spdlog::info("SUB A,{:x}", v);
  half_carry_check_sub(a, v);
  carry_check_sub(a, v);
  a -= v;
  zero_check(a);
  setf(N);
}

void CpuImpl::sbc(u8 v) {
  spdlog::info("SBC A,{:x}", v);
  half_carry_check_sub(a, v - ((getf(C)) ? 1 : 0));
  carry_check_sub(a, v - ((getf(C)) ? 1 : 0));
  a -= v - ((getf(C)) ? 1 : 0);
  zero_check(a);
  setf(N);
}

void CpuImpl::_and(u8 v) {
  spdlog::info("AND A,{:x}", v);
  a&= v;
  zero_check(a);
  clrf(N);
  setf(H);
  clrf(C);
}

void CpuImpl::_xor(u8 v) {
  spdlog::info("XOR A,{:x}", v);
  a^= v;
  zero_check(a);
  clrf(N);
  clrf(H);
  clrf(C);
}

void CpuImpl::_or(u8 v) {
  spdlog::info("OR A,{:x}", v);
  a|= v;
  zero_check(a);
  clrf(N);
  clrf(H);
  clrf(C);
}

void CpuImpl::cp(u8 v) {
  spdlog::info("CP A,{:x}", v);
  half_carry_check_sub(a, v);
  carry_check_sub(a, v);
  (a==v) ? setf(Z) : clrf(Z);
  setf(N);
}

void CpuImpl::daa() {
  pre = a;
  if (!getf(N)) {
	if (getf(C) || a> 0x99) {
	  a+= 0x60;
	  setf(C);
	}
	if (getf(H) || (a& 0x0F) > 0x09) {
	  a+= 0x6;
	}
  } else {
	if (getf(C)) {
	  a-= 0x60;
	}
	if (getf(H)) {
	  a-= 0x6;
	}
  }
  zero_check(a);
  clrf(H);
  spdlog::info("DAA ({:x}) = {:x}", pre, a);
}

void CpuImpl::ccf() {
  spdlog::info("CCF");
  clrf(N);
  (!getf(C)) ? setf(C) : clrf(C);
}

void CpuImpl::jr(bool cond) {
  spdlog::info("JR {0:d} : OldPC {1:x} : NewPC {2:x}", cond, pc, pc + ((cond) ? (signed)get8(pc+1) : 0));
  pc += ((cond) ? static_cast<s8>(get8(pc+1)) - 2 : 0);
}

void CpuImpl::jp(bool cond) {
  spdlog::info("JP {:d} : OldPC {:x} : NewPC {:x}", cond, pc, ((cond) ? get16(pc+1) : pc));
  pc = ((cond) ? get16(pc+1) - 3 : pc);
}

void CpuImpl::call(bool cond) {
  spdlog::info("CALL {:d} : OldPC {:x} : NewPC {:x}", cond, pc, get16(pc+1));
  if (cond) {
	push16(pc + 3);
	pc = get16(pc+1) - 3;
  }
}

void CpuImpl::ret(bool cond) {
  spdlog::info("RET {:d} : OldPC {:x} : NewPC {:x}", cond, pc, peek16());
  pc = ((cond) ? pop16() - 1 : 0);
}

void CpuImpl::rst(u8 v) {
  spdlog::info("RST {:x}", v);
  pc = v - 1;
}

void CpuImpl::pop(u16 &t) {
  spdlog::info("POP16 = {:x}", peek16());
  t = pop16();
}

void CpuImpl::push(u16 v) {
  spdlog::info("PUSH16 ({:x})", v);
  push16(v);
}

void CpuImpl::rlc(u8& t) {
  pre = t;
  (isbitset(t, 7)) ? setf(C) : clrf(C);
  t <<= 1;
  t |= (getf(C)) ? 1 : 0;
  zero_check(t);
  clrf(N);
  clrf(H);
  spdlog::info("RLC ({:x}) = {:x}", pre, t);
}

void CpuImpl::rrc(u8& t) {
  pre = t;
  (isbitset(t, 0)) ? setf(C) : clrf(C);
  t >>= 1;
  t |= (getf(C)) ? 0x80 : 0;
  zero_check(t);
  clrf(N);
  clrf(H);
  spdlog::info("RRC ({:x}) = {:x}", pre, t);
}

void CpuImpl::rlca() {
  pre = a;
  (isbitset(a,7)) ? setf(C) : clrf(C);
  a<<= 1;
  a|= (getf(C)) ? 1 : 0;
  clrf(N);
  clrf(H);
  clrf(Z);
  spdlog::info("RLCA ({:x}) = {:x}", pre, a);
}

void CpuImpl::rrca() {
  pre = a;
  (a& 1) ? setf(C) : clrf(C);
  a>>= 1;
  a|= (getf(C)) ? 0x80 : 0;
  clrf(H);
  clrf(N);
  clrf(Z);
  spdlog::info("RRCA ({:x}) = {:x}", pre, a);
}

void CpuImpl::rla() {
  pre = a;
  bool cy;
  (getf(C)) ? cy = true : cy = false;
  (a& 0x80) ? setf(C) : clrf(C);
  a<<= 1;
  (cy) ? a|= 1 : NULL;
  clrf(Z);
  clrf(N);
  clrf(H);
  spdlog::info("RLA ({:x}) = {:x}", pre, a);
}

void CpuImpl::rra() {
  pre = a;
  bool cy;
  (getf(C)) ? cy = true : cy = false;
  (a& 1) ? setf(C) : clrf(C);
  a>>= 1;
  (cy) ? a|= 0x80 : NULL;
  clrf(Z);
  clrf(N);
  clrf(H);
  spdlog::info("RRA ({:x}) = {:x}", pre, a);
}

void CpuImpl::rl(u8& t) {
  pre = t;
  bool cy = getf(C);
  (isbitset(t, 7)) ? setf(C) : clrf(C);
  t <<= 1;
  t |= (cy) ? 1 : 0;
  zero_check(t);
  clrf(N);
  clrf(H);
  spdlog::info("RL ({:x}) = {:x}", pre, t);
}

void CpuImpl::rr(u8& t) {
  pre = t;
  bool cy = getf(C);
  (isbitset(t, 0)) ? setf(C) : clrf(C);
  t >>= 1;
  t |= (cy) ? 0x80 : 0;
  zero_check(t);
  clrf(N);
  clrf(H);
  spdlog::info("RR ({:x}) = {:x}", pre, t);
}

void CpuImpl::sla(u8& t) {
  pre = t;
  (isbitset(t, 7)) ? setf(C) : clrf(C);
  t <<= 1;
  zero_check(t);
  clrf(N);
  clrf(H);
  spdlog::info("SLA ({:x}) = {:x}", pre, t);
}

void CpuImpl::sra(u8& t) {
  pre = t;
  (isbitset(t, 0)) ? setf(C) : clrf(C);
  t >>= 1;
  t |= (t & 0x40) ? 0x80 : 0;
  zero_check(t);
  clrf(N);
  clrf(H);
  spdlog::info("SRA ({:x}) = {:x}", pre, t);
}

void CpuImpl::swap(u8& t) {
  pre = t;
  t &= 0xFF | (t << 4) | (t >> 4);
  zero_check(t);
  clrf(N);
  clrf(H);
  clrf(C);
  spdlog::info("SWAP ({:x}) = {:x}", pre, t);
}

void CpuImpl::srl(u8& t) {
  pre = t;
  (isbitset(t, 0)) ? setf(C) : clrf(C);
  t >>= 1;
  zero_check(t);
  clrf(N);
  clrf(H);
  spdlog::info("SRL ({:x}) = {:x}", pre, t);
}

void CpuImpl::bit(u8 v, int bit_num) {
  spdlog::info("BIT ({:x}),{:d} = {}", v, bit_num, (isbitset(v,bit_num)) ? "true" : "false");
	(isbitset(v,bit_num)) ? setf(Z) : clrf(Z);
  clrf(N);
  setf(H);
}

void CpuImpl::res(u8 &t, int b) {
  spdlog::info("RES ({:x}),{:d}", t, b);
  t &= ~(1 << b);
}

void CpuImpl::set(u8 &t, int b) {
  spdlog::info("SET ({:x}),{:d}", t, b);
  t |= (1 << b);
}