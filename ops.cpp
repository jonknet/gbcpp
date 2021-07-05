#include "stddefs.h"
#include "spdlog/spdlog.h"
#include "cpuimpl.h"
#include <type_traits>

/* !!! FIXME: Fix this next */

using namespace GBCPP;

static u8 pre = 0;

void CpuImpl::ld16(u16& t, u16 v) {
	spdlog::info("LD16 {:x},{:x}", t, v);
	t = v;
}

void CpuImpl::ld_i(u16& t) {
	spdlog::info("LDI [{:x}],A({:x})", t, a);
    m[t] = a;
}

void CpuImpl::ld(u8& t, u8 v) {
	spdlog::info("LD {:x},{:x}", t, v);
	t = v;
}

void CpuImpl::inc16(u16& t) {
	spdlog::info("INC16 {:x}", *t);
	*t += 1;
}

void CpuImpl::inc(u8* t) {
	spdlog::info("INC {:x}", *t);
	HCHKADD(*t, 1);
	*t += 1;
	ZCHK(*t);
	CLR(_f, NBIT);
}

void CpuImpl::dec16(u16& t) {
	spdlog::info("DEC16 {:x}", *t);
	*t -= 1;
}

void CpuImpl::dec(u8* t) {
	spdlog::info("DEC {:x}", *t);
	HCHKSUB(*t, 1);
	*t -= 1;
	ZCHK(*t);
	SET(_f, NBIT);
}
void CpuImpl::add16(u16 v) {
	spdlog::info("ADD16 HL,{:x}", v);
	HCHKADD16(_hl, v);
	CYCHKADD16(_hl, v);
	_hl += v;
	CLR(_f, NBIT);
}

void CpuImpl::add(u8 v) {
	spdlog::info("ADD A,{:x}", v);
	HCHKADD(_a, v);
	CYCHKADD(_a, v);
	_a += v;
	ZCHK(_a);
	CLR(_f, NBIT);
}

void CpuImpl::adc(u8 v) {
	spdlog::info("ADC A,{:x}", v);
	HCHKADD(_a, v + (GET(_f, CBIT)) ? 1 : 0);
	CYCHKADD(_a, v + (GET(_f, CBIT)) ? 1 : 0);
	_a += v + (GET(_f, CBIT)) ? 1 : 0;
	ZCHK(_a);
	CLR(_f, NBIT);
}

void CpuImpl::sub(u8 v) {
	spdlog::info("SUB A,{:x}", v);
	HCHKSUB(_a, v);
	CYCHKSUB(_a, v);
	_a -= v;
	ZCHK(_a);
	SET(_f, NBIT);
}

void CpuImpl::sbc(u8 v) {
	spdlog::info("SBC A,{:x}", v);
	HCHKSUB(_a, v - (GET(_f,CBIT)) ? 1 : 0);
	CYCHKSUB(_a, v - (GET(_f, CBIT)) ? 1 : 0);
	_a -= v - (GET(_f, CBIT)) ? 1 : 0;
	ZCHK(_a);
	SET(_f, NBIT);
}

void CpuImpl::_and(u8 v) {
	spdlog::info("AND A,{:x}", v);
	_a &= v;
	ZCHK(_a);
	CLR(_f, NBIT);
	SET(_f, HBIT);
	CLR(_f, CBIT);
}

void CpuImpl::_xor(u8 v) {
	spdlog::info("XOR A,{:x}", v);
	_a ^= v;
	ZCHK(_a);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	CLR(_f, CBIT);
}

void CpuImpl::_or(u8 v) {
	spdlog::info("OR A,{:x}", v);
	_a |= v;
	ZCHK(_a);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	CLR(_f, CBIT);
}

void CpuImpl::cp(u8 v) {
	spdlog::info("CP A,{:x}", v);
	HCHKSUB(_a, v);
	CYCHKSUB(_a, v);
	(_a == v) ? SET(_f, ZBIT) : CLR(_f, ZBIT);
	SET(_f, NBIT);
}

void CpuImpl::daa() {
	pre = _a;
	if (!GET(_f, NBIT)) {
		if (GET(_f, CBIT) || _a > 0x99) { _a += 0x60; SET(_f, CBIT); }
		if (GET(_f, HBIT) || (_a & 0x0F) > 0x09) { _a += 0x6; }
	}
	else {
		if (GET(_f, CBIT)) { _a -= 0x60; }
		if (GET(_f, HBIT)) { _a -= 0x6; }
	}
	ZCHK(_a);
	CLR(_f, HBIT);
	spdlog::info("DAA ({:x}) = {:x}", pre, _a);
}

void CpuImpl::ccf() {
	spdlog::info("CCF");
	CLR(_f, NBIT);
	(!GET(_f, CBIT)) ? SET(_f, CBIT) : CLR(_f, CBIT);
}

void CpuImpl::jr(bool c) {
	spdlog::info("JR {0:d} : OldPC {1:x} : NewPC {2:x}", c, _pc, _pc + (c) ? (signed)GET8 : 0);
	_pc += ((c) ? static_cast<s8>(GET8) - 2 : 0);
}

void CpuImpl::jp(bool c) {
	spdlog::info("JP {:d} : OldPC {:x} : NewPC {:x}", c, _pc, (c) ? GET16 : _pc);
	_pc = ((c) ? GET16 - 3 : _pc);
}

void CpuImpl::call(bool c) {
	spdlog::info("CALL {:d} : OldPC {:x} : NewPC {:x}", c, _pc, GET16);
	if (c) {
		PUSH16(_pc + 3);
		_pc = GET16 - 3;
	}
}

void CpuImpl::ret(bool c) {
	spdlog::info("RET {:d} : OldPC {:x} : NewPC {:x}", c, _pc, PEEK16);
	_pc = ((c) ? POP16 - 1 : 0);
}

void CpuImpl::rst(u8 v) {
	spdlog::info("RST {:x}", v);
	_pc = v - 1;
}

void CpuImpl::pop(u16& t) {
	spdlog::info("POP16 = {:x}", PEEK16);
	*t = POP16;
}

void CpuImpl::push(u16 v) {
	spdlog::info("PUSH16 ({:x})", v);
	PUSH16(v);
}

void CpuImpl::rlc(u8* t) {
	pre = *t;
	(GET(*t, 7)) ? SET(_f, CBIT) : CLR(_f, CBIT);
	*t <<= 1;
	*t |= (GET(_f, CBIT)) ? 1 : 0;
	ZCHK(*t);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	spdlog::info("RLC ({:x}) = {:x}", pre, *t);
}

void CpuImpl::rrc(u8* t) {
	pre = *t;
	(GET(*t, 0)) ? SET(_f, CBIT) : CLR(_f, CBIT);
	*t >>= 1;
	*t |= (GET(_f, CBIT)) ? 0x80 : 0;
	ZCHK(*t);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	spdlog::info("RRC ({:x}) = {:x}", pre, *t);
}

void CpuImpl::rlca() {
	pre = _a;
	(_a & 0x80) ? SET(_f, CBIT) : CLR(_f, CBIT);
	_a <<= 1;
	_a |= (GET(_f, CBIT)) ? 1 : 0;
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	CLR(_f, ZBIT);
	spdlog::info("RLCA ({:x}) = {:x}", pre, _a);
}

void CpuImpl::rrca() {
	pre = _a;
	(_a & 1) ? SET(_f, CBIT) : CLR(_f, CBIT);
	_a >>= 1;
	_a |= (GET(_f, CBIT)) ? 0x80 : 0;
	CLR(_f, HBIT);
	CLR(_f, NBIT);
	CLR(_f, ZBIT);
	spdlog::info("RRCA ({:x}) = {:x}", pre, _a);
}

void CpuImpl::rla() {
	pre = _a;
	bool cy;
	(GET(_f, CBIT)) ? cy = true : cy = false;
	(_a & 0x80) ? SET(_f, CBIT) : CLR(_f, CBIT);
	_a <<= 1;
	(cy) ? _a |= 1 : NULL;
	CLR(_f, ZBIT);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	spdlog::info("RLA ({:x}) = {:x}", pre, _a);
}

void CpuImpl::rra() {
	pre = _a;
	bool cy;
	(GET(_f, CBIT)) ? cy = true : cy = false;
	(_a & 1) ? SET(_f, CBIT) : CLR(_f, CBIT);
	_a >>= 1;
	(cy) ? _a |= 0x80 : NULL;
	CLR(_f, ZBIT);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	spdlog::info("RRA ({:x}) = {:x}", pre, _a);
}

void CpuImpl::rl(u8* t) {
	pre = *t;
	bool cy = GET(_f, CBIT);
	(GET(*t, 7)) ? SET(_f, CBIT) : CLR(_f, CBIT);
	*t <<= 1;
	*t |= (cy) ? 1 : 0;
	ZCHK(*t);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	spdlog::info("RL ({:x}) = {:x}", pre, *t);
}

void CpuImpl::rr(u8* t) {
	pre = *t;
	bool cy = GET(_f, CBIT);
	(GET(*t, 0)) ? SET(_f, CBIT) : CLR(_f, CBIT);
	*t >>= 1;
	*t |= (cy) ? 0x80 : 0;
	ZCHK(*t);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	spdlog::info("RR ({:x}) = {:x}", pre, *t);
}

void CpuImpl::sla(u8* t) {
	pre = *t;
	(GET(*t, 7)) ? SET(_f, CBIT) : CLR(_f, CBIT);
	*t <<= 1;
	ZCHK(*t);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	spdlog::info("SLA ({:x}) = {:x}", pre, *t);
}

void CpuImpl::sra(u8* t) {
	pre = *t;
	(GET(*t, 0)) ? SET(_f, CBIT) : CLR(_f, CBIT);
	*t >>= 1;
	*t |= (*t & 0x40) ? 0x80 : 0;
	ZCHK(*t);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	spdlog::info("SRA ({:x}) = {:x}", pre, *t);
}

void CpuImpl::swap(u8* t) {
	pre = *t;
	*t &= 0xFF | (*t << 4) | (*t >> 4);
	ZCHK(*t);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	CLR(_f, CBIT);
	spdlog::info("SWAP ({:x}) = {:x}", pre, *t);
}

void CpuImpl::srl(u8* t) {
	pre = *t;
	(GET(*t, 0)) ? SET(_f, CBIT) : CLR(_f, CBIT);
	*t >>= 1;
	ZCHK(*t);
	CLR(_f, NBIT);
	CLR(_f, HBIT);
	spdlog::info("SRL ({:x}) = {:x}", pre, *t);
}

void CpuImpl::bit(u8 v, int b) {
	spdlog::info("BIT ({:x}),{:d} = {}", v, b, (v & (1 << b)) ? "true" : "false");
	(v & (1 << b)) ? SET(_f, ZBIT) : CLR(_f, ZBIT);
	CLR(_f, NBIT);
	SET(_f, HBIT);
}

void CpuImpl::res(u8& t, int b) {
	spdlog::info("RES ({:x}),{:d}", t, b);
	t &= ~(1 << b);
}

void CpuImpl::set(u8& t, int b) {
	spdlog::info("SET ({:x}),{:d}", t, b);
	t |= (1 << b);
}