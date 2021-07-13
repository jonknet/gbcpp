#pragma once
#include "stddefs.h"
#include "Mmu.h"
#include "ops.h"
#include "spdlog/spdlog.h"
#include "Cpu.h"

namespace GBCPP {

enum class OpState {
  RUNNING,
  IDLE
};

struct CpuState {
  u64 cycles;
  bool ime;
  bool running;
  bool halted;
  bool stopped;
  s16 lastop;
  bool lastop_cb;
  OpState current_state;
  int remaining_cycles;
  CpuState() :  cycles(0),
                ime(true),
                running(false),
                halted(false),
                stopped(false),
                lastop(0),
                lastop_cb(false),
                current_state(OpState::IDLE),
                remaining_cycles(0) {};
};

enum Flags {
  Z = 0x80, N = 0x40, H = 0x20, C = 0x10
};



class Cpu::CpuImpl {
 public:
  explicit CpuImpl(MemMgr *mem_mgr)
      : m(mem_mgr) {
  }

 protected:

  // Memory Helpers

  inline u8 get8(u16 address) {
    return (*m)[address];
  }

  inline u16 get16(u16 address) {
    return (((*m)[address + 1] << 8) | ((*m)[address]));
  }

  // Stack Helpers

  [[nodiscard]] inline u16 peek16() const {
    return ((*m)[r.sp] | ((*m)[r.sp + 1] << 8));
  }

  inline u16 pop16() {
    u16 out = peek16();
    r.sp += 2;
    return out;
  }

  inline void push16(u16 val) {
    (*m)[--r.sp] = (val & 0xFF00) >> 8;
    (*m)[--r.sp] = val & 0xFF;
  }

  // Bit Flag Helpers

  static bool isbitset(u16 field, int bit_num) {
    assert(bit_num < 8 && bit_num >= 0);
    if (field & (1 << bit_num)) {
      return true;
    } else {
      return false;
    }
  }

  inline void setf(Flags flags) const {
    *r.f |= flags;
  }

  inline void clrf(Flags flags) const {
    *r.f &= ~flags;
  }

  [[nodiscard]] inline bool getf(Flags flags) const {
    return (*r.f & flags) != 0;
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
    if (((op1 & 0xF) + (op2 & 0xF) & 0x10) == 0x10) {
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
    if ((((op1 & 0xFF) + (op2 & 0xFF)) & 0x100) == 0x100) {
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

  static void ld16(u16 *t, u16 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:<6s} t:{:04x},v:{:04x}", __func__, *t, v)) + log_buffer;
    *t = v;
  }

  void ld_i(u16 t) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} t:{:x})", __func__, t)) + log_buffer;
    (*m)[t] = *r.a;
  }

  static void ld(u8 *t, u16 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} t:{:x},v:{:x}", __func__, *t, v)) + log_buffer;
    *t = v;
  }

  static void inc16(u16 *t) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} t:{:x}", __func__, *t)) + log_buffer;
    *t += 1;
  }

  void inc(u8 *t) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} t:{:x}", __func__, *t)) + log_buffer;
    half_carry_check_add(*t, 1);
    *t += 1;
    zero_check(*t);
    clrf(N);
  }

  static void dec16(u16 *t) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} t:{:x}", __func__, *t)) + log_buffer;
    *t -= 1;
  }

  void dec(u8 *t) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} t:{:x}", __func__, *t)) + log_buffer;
    half_carry_check_sub(*t, 1);
    *t -= 1;
    zero_check(*t);
    setf(N);
  }
  void add16(u16 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} HL,{:x}", __func__, v)) + log_buffer;
    half_carry_check_add16(r.hl, v);
    carry_check_add16(r.hl, v);
    r.hl += v;
    clrf(N);
  }

  void add(u8 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} A,{:x}", __func__, v)) + log_buffer;
    half_carry_check_add(*r.a, v);
    carry_check_add(*r.a, v);
    *r.a += v;
    zero_check(*r.a);
    clrf(N);
  }

  void adc(u8 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} A,{:x}", __func__, v)) + log_buffer;
    half_carry_check_add(*r.a, v + ((getf(C)) ? 1 : 0));
    carry_check_add(*r.a, v + ((getf(C)) ? 1 : 0));
    *r.a += v + ((getf(C)) ? 1 : 0);
    zero_check(*r.a);
    clrf(N);
  }

  void sub(u8 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} A,{:x}", __func__, v)) + log_buffer;
    half_carry_check_sub(*r.a, v);
    carry_check_sub(*r.a, v);
    *r.a -= v;
    zero_check(*r.a);
    setf(N);
  }

  void sbc(u8 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} A,{:x}", __func__, v)) + log_buffer;
    half_carry_check_sub(*r.a, v - ((getf(C)) ? 1 : 0));
    carry_check_sub(*r.a, v - ((getf(C)) ? 1 : 0));
    *r.a -= v - ((getf(C)) ? 1 : 0);
    zero_check(*r.a);
    setf(N);
  }

  void _and(u8 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} A,{:x}", __func__, v)) + log_buffer;
    *r.a &= v;
    zero_check(*r.a);
    clrf(N);
    setf(H);
    clrf(C);
  }

  void _xor(u8 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} A,{:x}", __func__, v)) + log_buffer;
    *r.a ^= v;
    zero_check(*r.a);
    clrf(N);
    clrf(H);
    clrf(C);
  }

  void _or(u8 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} A,{:x}", __func__, v)) + log_buffer;
    *r.a |= v;
    zero_check(*r.a);
    clrf(N);
    clrf(H);
    clrf(C);
  }

  void cp(u8 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} A,{:x}", __func__, v)) + log_buffer;
    half_carry_check_sub(*r.a, v);
    carry_check_sub(*r.a, v);
    (*r.a == v) ? setf(Z) : clrf(Z);
    setf(N);
  }

  void daa() {
    auto pre = *r.a;
    if (!getf(N)) {
      if (getf(C) || *r.a > 0x99) {
        *r.a += 0x60;
        setf(C);
      }
      if (getf(H) || (*r.a & 0x0F) > 0x09) {
        *r.a += 0x6;
      }
    } else {
      if (getf(C)) {
        *r.a -= 0x60;
      }
      if (getf(H)) {
        *r.a -= 0x6;
      }
    }
    zero_check(*r.a);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x})", __func__, *r.a)) + log_buffer;
  }

  void ccf() {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s}", __func__)) + log_buffer;
    clrf(N);
    (!getf(C)) ? setf(C) : clrf(C);
  }

  void jr(bool cond) {
    log_buffer = fmt::format("{:64s}",
                             fmt::format("{:s} {:d} : Oldr.pc {:x} : Newr.pc {:x}",
                                         __func__,
                                         cond,
                                         r.pc,
                                         r.pc + ((cond) ? (s8) get8(r.pc + 1) : 0) + 2)) + log_buffer;
    r.pc += ((cond) ? (static_cast<s8>(get8(r.pc + 1))) : 0);
  }

  void jp(bool cond) {
    log_buffer = fmt::format("{:64s}",
                             fmt::format("{:s} {:d} : Oldr.pc {:x} : Newr.pc {:x}",
                                         __func__,
                                         cond,
                                         r.pc,
                                         ((cond) ? get16(r.pc + 1) : r.pc))) + log_buffer;
    r.pc = ((cond) ? (get16(r.pc + 1) - 3) : r.pc);
  }

  void call(bool cond) {
    log_buffer = fmt::format("{:64s}",
                             fmt::format("{:s} {:d} : Oldr.pc {:x} : Newr.pc {:x}",
                                         __func__,
                                         cond,
                                         r.pc,
                                         get16(r.pc + 1))) + log_buffer;
    if (cond) {
      push16(r.pc + 3);
      r.pc = get16(r.pc + 1) - 3;
    }
  }

  void ret(bool cond) {
    log_buffer =
        fmt::format("{:64s}", fmt::format("{:s} {:d} : Oldr.pc {:x} : Newr.pc {:x}", __func__, cond, r.pc, peek16()))
            + log_buffer;
    r.pc = ((cond) ? pop16() - 1 : r.pc);
  }

  void rst(u8 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} {:x}", __func__, v)) + log_buffer;
    r.pc = v - 1;
  }

  void pop(u16 *t) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} = {:x}", __func__, peek16())) + log_buffer;
    *t = pop16();
  }

  void push(u16 v) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x})", __func__, v)) + log_buffer;
    push16(v);
  }

  void rlc(u8 *t) {
    auto pre = *t;
    (isbitset(*t, 7)) ? setf(C) : clrf(C);
    *t <<= 1;
    *t |= (getf(C)) ? 1 : 0;
    zero_check(*t);
    clrf(N);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *t)) + log_buffer;
  }

  void rrc(u8 *t) {
    auto pre = *t;
    (isbitset(*t, 0)) ? setf(C) : clrf(C);
    *t >>= 1;
    *t |= (getf(C)) ? 0x80 : 0;
    zero_check(*t);
    clrf(N);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *t)) + log_buffer;
  }

  void rlca() {
    auto pre = *r.a;
    (isbitset(*r.a, 7)) ? setf(C) : clrf(C);
    *r.a <<= 1;
    *r.a |= (getf(C)) ? 1 : 0;
    clrf(N);
    clrf(H);
    clrf(Z);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *r.a)) + log_buffer;
  }

  void rrca() {
    auto pre = *r.a;
    (*r.a & 1) ? setf(C) : clrf(C);
    *r.a >>= 1;
    *r.a |= (getf(C)) ? 0x80 : 0;
    clrf(H);
    clrf(N);
    clrf(Z);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *r.a)) + log_buffer;
  }

  void rla() {
    auto pre = *r.a;
    bool cy;
    (getf(C)) ? cy = true : cy = false;
    (*r.a & 0x80) ? setf(C) : clrf(C);
    *r.a <<= 1;
    *r.a |= (cy) ? 1 : 0;
    clrf(Z);
    clrf(N);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *r.a)) + log_buffer;
  }

  void rra() {
    auto pre = *r.a;
    bool cy;
    (getf(C)) ? cy = true : cy = false;
    (*r.a & 1) ? setf(C) : clrf(C);
    *r.a >>= 1;
    *r.a |= (cy) ? 0x80 : 0;
    clrf(Z);
    clrf(N);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *r.a)) + log_buffer;
  }

  void rl(u8 *t) {
    auto pre = *t;
    bool cy = getf(C);
    (isbitset(*t, 7)) ? setf(C) : clrf(C);
    *t <<= 1;
    *t |= (cy) ? 1 : 0;
    zero_check(*t);
    clrf(N);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *t)) + log_buffer;
  }

  void rr(u8 *t) {
    auto pre = *t;
    bool cy = getf(C);
    (isbitset(*t, 0)) ? setf(C) : clrf(C);
    *t >>= 1;
    *t |= (cy) ? 0x80 : 0;
    zero_check(*t);
    clrf(N);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *t)) + log_buffer;
  }

  void sla(u8 *t) {
    auto pre = *t;
    (isbitset(*t, 7)) ? setf(C) : clrf(C);
    *t <<= 1;
    zero_check(*t);
    clrf(N);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *t)) + log_buffer;
  }

  void sra(u8 *t) {
    auto pre = *t;
    (isbitset(*t, 0)) ? setf(C) : clrf(C);
    *t >>= 1;
    *t |= ((*t & 0x40) ? 0x80 : 0);
    zero_check(*t);
    clrf(N);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *t)) + log_buffer;
  }

  void swap(u8 *t) {
    auto pre = *t;
    *t &= 0xFF | (*t << 4) | (*t >> 4);
    zero_check(*t);
    clrf(N);
    clrf(H);
    clrf(C);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *t)) + log_buffer;
  }

  void srl(u8 *t) {
    auto pre = *t;
    (isbitset(*t, 0)) ? setf(C) : clrf(C);
    *t >>= 1;
    zero_check(*t);
    clrf(N);
    clrf(H);
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} ({:x}) = {:x}", __func__, pre, *t)) + log_buffer;
  }

  void bit(u16 v, int bit_num) {
    log_buffer = fmt::format("{:64s}",
                             fmt::format("{:s} ({:x}),{:d} = {}",
                                         __func__,
                                         v,
                                         bit_num,
                                         (isbitset(v, bit_num)) ? "true" : "false")) + log_buffer;
    (isbitset(v, bit_num)) ? clrf(Z) : setf(Z);
    clrf(N);
    setf(H);
  }

  static void res(u8 *t, int b) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} t:{:x},b:{:d}", __func__, *t, b)) + log_buffer;
    *t &= ~(1 << b);
  }

  static void set(u8 *t, int b) {
    log_buffer = fmt::format("{:64s}", fmt::format("{:s} t:{:x},b:{:d}", __func__, *t, b)) + log_buffer;
    *t |= (1 << b);
  }

 public:
  void lookup_and_execute() {
    log_buffer.clear();
    log_buffer = fmt::format(
        "\t\t[PC:{:04x} A:{:02x} B:{:02x} C:{:02x} D:{:02x} E:{:02x} H:{:02x} L:{:02x} SP:{:04x} [{:04x} {:04x}]]",
        r.pc,
        *r.a,
        *r.b,
        *r.c,
        *r.d,
        *r.e,
        *r.h,
        *r.l,
        r.sp,
        (*m)[r.sp],
        (*m)[r.sp + 1]);
    s.lastop = (*m)[r.pc];
    s.lastop_cb = ((*m)[r.pc] == 0xCB);
    assert(s.lastop >= 0 && s.lastop <= 256);
    switch (s.lastop) {
      case 0x00:break;
      case 0x01:ld16(&r.bc, get16(r.pc + 1));
        break;
      case 0x02:ld_i(r.bc);
        break;
      case 0x03:inc16(&r.bc);
        break;
      case 0x04:inc(r.b);
        break;
      case 0x05:dec(r.b);
        break;
      case 0x06:ld(r.b, get8(r.pc + 1));
        break;
      case 0x07:rlca();
        break;
      case 0x08:(*m)[get16(r.pc + 1)] = (r.sp & 0xFF);
        (*m)[get16(r.pc + 2)] = (r.sp & 0xFF00) >> 8;
        break;
      case 0x09:add16(r.bc);
        break;
      case 0x0A:ld(r.a, (*m)[r.bc]);
        break;
      case 0x0B:dec16(&r.bc);
        break;
      case 0x0C:inc(r.c);
        break;
      case 0x0D:dec(r.c);
        break;
      case 0x0E:ld(r.c, get8(r.pc + 1));
        break;
      case 0x0F:rrca();
        break;
      case 0x10:s.stopped = true;
        break;
      case 0x11:ld16(&r.de, get16(r.pc + 1));
        break;
      case 0x12:ld_i(r.de);
        break;
      case 0x13:inc16(&r.de);
        break;
      case 0x14:inc(r.d);
        break;
      case 0x15:dec(r.d);
        break;
      case 0x16:ld(r.d, get8(r.pc + 1));
        break;
      case 0x17:rla();
        break;
      case 0x18:jr(true);
        break;
      case 0x19:add16(r.de);
        break;
      case 0x1A:ld(r.a, (*m)[r.de]);
        break;
      case 0x1B:dec16(&r.de);
        break;
      case 0x1C:inc(r.e);
        break;
      case 0x1D:dec(r.e);
        break;
      case 0x1E:ld(r.e, get8(r.pc + 1));
        break;
      case 0x1F:rra();
        break;
      case 0x20:jr(!getf(Z));
        break;
      case 0x21:ld16(&r.hl, get16(r.pc + 1));
        break;
      case 0x22:ld_i(r.hl);
        r.hl++;
        break;
      case 0x23:inc16(&r.hl);
        break;
      case 0x24:inc(r.h);
        break;
      case 0x25:dec(r.h);
        break;
      case 0x26:ld(r.h, get8(r.pc + 1));
        break;
      case 0x27:daa();
        break;
      case 0x28:jr(getf(Z));
        break;
      case 0x29:add16(r.hl);
        break;
      case 0x2A:ld(r.a, r.hl);
        (r.hl)++;
        break;
      case 0x2B:dec16(&r.hl);
        break;
      case 0x2C:inc(r.l);
        break;
      case 0x2D:dec(r.l);
        break;
      case 0x2E:ld(r.l, get8(r.pc + 1));
        break;
      case 0x2F:*r.a = ~*r.a;
        setf(N);
        setf(H);
        break;
      case 0x30:jr(!getf(C));
        break;
      case 0x31:ld16(&r.sp, get16(r.pc + 1));
        break;
      case 0x32:ld_i(r.hl);
        (r.hl)--;
        break;
      case 0x33:inc16(&r.sp);
        break;
      case 0x34:inc(&(*m)[r.hl]);
        break;
      case 0x35:dec(&(*m)[r.hl]);
        break;
      case 0x36:ld(&(*m)[r.hl], get8(r.pc + 1));
        break;
      case 0x37:setf(C);
        clrf(N);
        clrf(H);
        break;
      case 0x38:jr(getf(C));
        break;
      case 0x39:add16(r.sp);
        break;
      case 0x3A:ld(r.a, (*m)[r.hl]);
        (r.hl)--;
        break;
      case 0x3B:dec16(&r.sp);
        break;
      case 0x3C:inc(r.a);
        break;
      case 0x3D:dec(r.a);
        break;
      case 0x3E:ld(r.a, get8(r.pc + 1));
        break;
      case 0x3F:ccf();
        break;
      case 0x40:ld(r.b, *r.b);
        break;
      case 0x41:ld(r.b, *r.c);
        break;
      case 0x42:ld(r.b, *r.d);
        break;
      case 0x43:ld(r.b, *r.e);
        break;
      case 0x44:ld(r.b, *r.h);
        break;
      case 0x45:ld(r.b, *r.l);
        break;
      case 0x46:ld(r.b, (*m)[r.hl]);
        break;
      case 0x47:ld(r.b, *r.a);
        break;
      case 0x48:ld(r.c, *r.b);
        break;
      case 0x49:ld(r.c, *r.c);
        break;
      case 0x4A:ld(r.c, *r.d);
        break;
      case 0x4B:ld(r.c, *r.e);
        break;
      case 0x4C:ld(r.c, *r.h);
        break;
      case 0x4D:ld(r.c, *r.l);
        break;
      case 0x4E:ld(r.c, (*m)[r.hl]);
        break;
      case 0x4F:ld(r.c, *r.a);
        break;
      case 0x50:ld(r.d, *r.b);
        break;
      case 0x51:ld(r.d, *r.c);
        break;
      case 0x52:ld(r.d, *r.d);
        break;
      case 0x53:ld(r.d, *r.e);
        break;
      case 0x54:ld(r.d, *r.h);
        break;
      case 0x55:ld(r.d, *r.l);
        break;
      case 0x56:ld(r.d, (*m)[r.hl]);
        break;
      case 0x57:ld(r.d, *r.a);
        break;
      case 0x58:ld(r.e, *r.b);
        break;
      case 0x59:ld(r.e, *r.c);
        break;
      case 0x5A:ld(r.e, *r.d);
        break;
      case 0x5B:ld(r.e, *r.e);
        break;
      case 0x5C:ld(r.e, *r.h);
        break;
      case 0x5D:ld(r.e, *r.l);
        break;
      case 0x5E:ld(r.e, (*m)[r.hl]);
        break;
      case 0x5F:ld(r.e, *r.a);
        break;
      case 0x60:ld(r.h, *r.b);
        break;
      case 0x61:ld(r.h, *r.c);
        break;
      case 0x62:ld(r.h, *r.d);
        break;
      case 0x63:ld(r.h, *r.e);
        break;
      case 0x64:ld(r.h, *r.h);
        break;
      case 0x65:ld(r.h, *r.l);
        break;
      case 0x66:ld(r.h, (*m)[r.hl]);
        break;
      case 0x67:ld(r.h, *r.a);
        break;
      case 0x68:ld(r.l, *r.b);
        break;
      case 0x69:ld(r.l, *r.c);
        break;
      case 0x6A:ld(r.l, *r.d);
        break;
      case 0x6B:ld(r.l, *r.e);
        break;
      case 0x6C:ld(r.l, *r.h);
        break;
      case 0x6D:ld(r.l, *r.l);
        break;
      case 0x6E:ld(r.l, (*m)[r.hl]);
        break;
      case 0x6F:ld(r.l, *r.a);
        break;
      case 0x70:ld(&(*m)[r.hl], *r.b);
        break;
      case 0x71:ld(&(*m)[r.hl], *r.c);
        break;
      case 0x72:ld(&(*m)[r.hl], *r.d);
        break;
      case 0x73:ld(&(*m)[r.hl], *r.e);
        break;
      case 0x74:ld(&(*m)[r.hl], *r.h);
        break;
      case 0x75:ld(&(*m)[r.hl], *r.l);
        break;
      case 0x76: s.halted = true;
        break;
      case 0x77:ld(&(*m)[r.hl], *r.a);
        break;
      case 0x78:ld(r.a, *r.b);
        break;
      case 0x79:ld(r.a, *r.c);
        break;
      case 0x7A:ld(r.a, *r.d);
        break;
      case 0x7B:ld(r.a, *r.e);
        break;
      case 0x7C:ld(r.a, *r.h);
        break;
      case 0x7D:ld(r.a, *r.l);
        break;
      case 0x7E:ld(r.a, (*m)[r.hl]);
        break;
      case 0x7F:ld(r.a, *r.a);
        break;
      case 0x80:add(*r.b);
        break;
      case 0x81:add(*r.c);
        break;
      case 0x82:add(*r.d);
        break;
      case 0x83:add(*r.e);
        break;
      case 0x84:add(*r.h);
        break;
      case 0x85:add(*r.l);
        break;
      case 0x86:add((*m)[r.hl]);
        break;
      case 0x87:add(*r.a);
        break;
      case 0x88:adc(*r.b);
        break;
      case 0x89:adc(*r.c);
        break;
      case 0x8A:adc(*r.d);
        break;
      case 0x8B:adc(*r.e);
        break;
      case 0x8C:adc(*r.h);
        break;
      case 0x8D:adc(*r.l);
        break;
      case 0x8E:adc((*m)[r.hl]);
        break;
      case 0x8F:adc(*r.a);
        break;
      case 0x90:sub(*r.b);
        break;
      case 0x91:sub(*r.c);
        break;
      case 0x92:sub(*r.d);
        break;
      case 0x93:sub(*r.e);
        break;
      case 0x94:sub(*r.h);
        break;
      case 0x95:sub(*r.l);
        break;
      case 0x96:sub((*m)[r.hl]);
        break;
      case 0x97:sub(*r.a);
        break;
      case 0x98:sbc(*r.b);
        break;
      case 0x99:sbc(*r.c);
        break;
      case 0x9A:sbc(*r.d);
        break;
      case 0x9B:sbc(*r.e);
        break;
      case 0x9C:sbc(*r.h);
        break;
      case 0x9D:sbc(*r.l);
        break;
      case 0x9E:sbc((*m)[r.hl]);
        break;
      case 0x9F:sbc(*r.a);
        break;
      case 0xA0:_and(*r.b);
        break;
      case 0xA1:_and(*r.c);
        break;
      case 0xA2:_and(*r.d);
        break;
      case 0xA3:_and(*r.e);
        break;
      case 0xA4:_and(*r.h);
        break;
      case 0xA5:_and(*r.l);
        break;
      case 0xA6:_and((*m)[r.hl]);
        break;
      case 0xA7:_and(*r.a);
        break;
      case 0xA8:_xor(*r.b);
        break;
      case 0xA9:_xor(*r.c);
        break;
      case 0xAA:_xor(*r.d);
        break;
      case 0xAB:_xor(*r.e);
        break;
      case 0xAC:_xor(*r.h);
        break;
      case 0xAD:_xor(*r.l);
        break;
      case 0xAE:_xor((*m)[r.hl]);
        break;
      case 0xAF:_xor(*r.a);
        break;
      case 0xB0:_or(*r.b);
        break;
      case 0xB1:_or(*r.c);
        break;
      case 0xB2:_or(*r.d);
        break;
      case 0xB3:_or(*r.e);
        break;
      case 0xB4:_or(*r.h);
        break;
      case 0xB5:_or(*r.l);
        break;
      case 0xB6:_or((*m)[r.hl]);
        break;
      case 0xB7:_or(*r.a);
        break;
      case 0xB8:cp(*r.b);
        break;
      case 0xB9:cp(*r.c);
        break;
      case 0xBA:cp(*r.d);
        break;
      case 0xBB:cp(*r.e);
        break;
      case 0xBC:cp(*r.h);
        break;
      case 0xBD:cp(*r.l);
        break;
      case 0xBE:cp((*m)[r.hl]);
        break;
      case 0xBF:cp(*r.a);
        break;
      case 0xC0:ret(!getf(Z));
        break;
      case 0xC1:pop(&r.bc);
        break;
      case 0xC2:jp(!(getf(Z)));
        break;
      case 0xC3:jp(true);
        break;
      case 0xC4:call(!getf(Z));
        break;
      case 0xC5:push(r.bc);
        break;
      case 0xC6:add(get8(r.pc + 1));
        break;
      case 0xC7:rst(0x00);
        break;
      case 0xC8:ret(getf(Z));
        break;
      case 0xC9:ret(true);
        break;
      case 0xCA:jp(getf(Z));
        break;
      case 0xCB: s.lastop = (*m)[r.pc + 1];
        switch ((*m)[r.pc + 1]) {
          case 0x00:rlc(r.b);
            break;
          case 0x01:rlc(r.c);
            break;
          case 0x02:rlc(r.d);
            break;
          case 0x03:rlc(r.e);
            break;
          case 0x04:rlc(r.h);
            break;
          case 0x05:rlc(r.l);
            break;
          case 0x06:rlc(&(*m).data()[r.hl]);
            break;
          case 0x07:rlc(r.a);
            break;
          case 0x08:rrc(r.b);
            break;
          case 0x09:rrc(r.c);
            break;
          case 0x0A:rrc(r.d);
            break;
          case 0x0B:rrc(r.e);
            break;
          case 0x0C:rrc(r.h);
            break;
          case 0x0D:rrc(r.l);
            break;
          case 0x0E:rrc(&(*m).data()[r.hl]);
            break;
          case 0x0F:rrc(r.a);
            break;
          case 0x10:rl(r.b);
            break;
          case 0x11:rl(r.c);
            break;
          case 0x12:rl(r.d);
            break;
          case 0x13:rl(r.e);
            break;
          case 0x14:rl(r.h);
            break;
          case 0x15:rl(r.l);
            break;
          case 0x16:rl(&(*m).data()[r.hl]);
            break;
          case 0x17:rl(r.a);
            break;
          case 0x18:rr(r.b);
            break;
          case 0x19:rr(r.c);
            break;
          case 0x1A:rr(r.d);
            break;
          case 0x1B:rr(r.e);
            break;
          case 0x1C:rr(r.h);
            break;
          case 0x1D:rr(r.l);
            break;
          case 0x1E:rr(&(*m).data()[r.hl]);
            break;
          case 0x1F:rr(r.a);
            break;
          case 0x20:sla(r.b);
            break;
          case 0x21:sla(r.c);
            break;
          case 0x22:sla(r.d);
            break;
          case 0x23:sla(r.e);
            break;
          case 0x24:sla(r.h);
            break;
          case 0x25:sla(r.l);
            break;
          case 0x26:sla(&(*m).data()[r.hl]);
            break;
          case 0x27:sla(r.a);
            break;
          case 0x28:sra(r.b);
            break;
          case 0x29:sra(r.c);
            break;
          case 0x2A:sra(r.d);
            break;
          case 0x2B:sra(r.e);
            break;
          case 0x2C:sra(r.h);
            break;
          case 0x2D:sra(r.l);
            break;
          case 0x2E:sra(&(*m).data()[r.hl]);
            break;
          case 0x2F:sra(r.a);
            break;
          case 0x30:swap(r.b);
            break;
          case 0x31:swap(r.c);
            break;
          case 0x32:swap(r.d);
            break;
          case 0x33:swap(r.e);
            break;
          case 0x34:swap(r.h);
            break;
          case 0x35:swap(r.l);
            break;
          case 0x36:swap(&(*m).data()[r.hl]);
            break;
          case 0x37:swap(r.a);
            break;
          case 0x38:srl(r.b);
            break;
          case 0x39:srl(r.c);
            break;
          case 0x3A:srl(r.d);
            break;
          case 0x3B:srl(r.e);
            break;
          case 0x3C:srl(r.h);
            break;
          case 0x3D:srl(r.l);
            break;
          case 0x3E:srl(&(*m).data()[r.hl]);
            break;
          case 0x3F:srl(r.a);
            break;
          case 0x40:bit(*r.b, 0);
            break;
          case 0x41:bit(*r.c, 0);
            break;
          case 0x42:bit(*r.d, 0);
            break;
          case 0x43:bit(*r.e, 0);
            break;
          case 0x44:bit(*r.h, 0);
            break;
          case 0x45:bit(*r.l, 0);
            break;
          case 0x46:bit((*m).data()[r.hl], 0);
            break;
          case 0x47:bit(*r.a, 0);
            break;
          case 0x48:bit(*r.b, 1);
            break;
          case 0x49:bit(*r.c, 1);
            break;
          case 0x4A:bit(*r.d, 1);
            break;
          case 0x4B:bit(*r.e, 1);
            break;
          case 0x4C:bit(*r.h, 1);
            break;
          case 0x4D:bit(*r.l, 1);
            break;
          case 0x4E:bit((*m).data()[r.hl], 1);
            break;
          case 0x4F:bit(*r.a, 1);
            break;
          case 0x50:bit(*r.b, 2);
            break;
          case 0x51:bit(*r.c, 2);
            break;
          case 0x52:bit(*r.d, 2);
            break;
          case 0x53:bit(*r.e, 2);
            break;
          case 0x54:bit(*r.h, 2);
            break;
          case 0x55:bit(*r.l, 2);
            break;
          case 0x56:bit((*m).data()[r.hl], 2);
            break;
          case 0x57:bit(*r.a, 2);
            break;
          case 0x58:bit(*r.b, 3);
            break;
          case 0x59:bit(*r.c, 3);
            break;
          case 0x5A:bit(*r.d, 3);
            break;
          case 0x5B:bit(*r.e, 3);
            break;
          case 0x5C:bit(*r.h, 3);
            break;
          case 0x5D:bit(*r.l, 3);
            break;
          case 0x5E:bit((*m).data()[r.hl], 3);
            break;
          case 0x5F:bit(*r.a, 3);
            break;
          case 0x60:bit(*r.b, 4);
            break;
          case 0x61:bit(*r.c, 4);
            break;
          case 0x62:bit(*r.d, 4);
            break;
          case 0x63:bit(*r.e, 4);
            break;
          case 0x64:bit(*r.h, 4);
            break;
          case 0x65:bit(*r.l, 4);
            break;
          case 0x66:bit((*m).data()[r.hl], 4);
            break;
          case 0x67:bit(*r.a, 4);
            break;
          case 0x68:bit(*r.b, 5);
            break;
          case 0x69:bit(*r.c, 5);
            break;
          case 0x6A:bit(*r.d, 5);
            break;
          case 0x6B:bit(*r.e, 5);
            break;
          case 0x6C:bit(*r.h, 5);
            break;
          case 0x6D:bit(*r.l, 5);
            break;
          case 0x6E:bit((*m).data()[r.hl], 5);
            break;
          case 0x6F:bit(*r.a, 5);
            break;
          case 0x70:bit(*r.b, 6);
            break;
          case 0x71:bit(*r.c, 6);
            break;
          case 0x72:bit(*r.d, 6);
            break;
          case 0x73:bit(*r.e, 6);
            break;
          case 0x74:bit(*r.h, 6);
            break;
          case 0x75:bit(*r.l, 6);
            break;
          case 0x76:bit((*m).data()[r.hl], 6);
            break;
          case 0x77:bit(*r.a, 6);
            break;
          case 0x78:bit(*r.b, 7);
            break;
          case 0x79:bit(*r.c, 7);
            break;
          case 0x7A:bit(*r.d, 7);
            break;
          case 0x7B:bit(*r.e, 7);
            break;
          case 0x7C:bit(*r.h, 7);
            break;
          case 0x7D:bit(*r.l, 7);
            break;
          case 0x7E:bit((*m).data()[r.hl], 7);
            break;
          case 0x7F:bit(*r.a, 7);
            break;
          case 0x80:res(r.b, 0);
            break;
          case 0x81:res(r.c, 0);
            break;
          case 0x82:res(r.d, 0);
            break;
          case 0x83:res(r.e, 0);
            break;
          case 0x84:res(r.h, 0);
            break;
          case 0x85:res(r.l, 0);
            break;
          case 0x86:res(&(*m).data()[r.hl], 0);
            break;
          case 0x87:res(r.a, 0);
            break;
          case 0x88:res(r.b, 1);
            break;
          case 0x89:res(r.c, 1);
            break;
          case 0x8A:res(r.d, 1);
            break;
          case 0x8B:res(r.e, 1);
            break;
          case 0x8C:res(r.h, 1);
            break;
          case 0x8D:res(r.l, 1);
            break;
          case 0x8E:res(&(*m).data()[r.hl], 1);
            break;
          case 0x8F:res(r.a, 1);
            break;
          case 0x90:res(r.b, 2);
            break;
          case 0x91:res(r.c, 2);
            break;
          case 0x92:res(r.d, 2);
            break;
          case 0x93:res(r.e, 2);
            break;
          case 0x94:res(r.h, 2);
            break;
          case 0x95:res(r.l, 2);
            break;
          case 0x96:res(&(*m).data()[r.hl], 2);
            break;
          case 0x97:res(r.a, 2);
            break;
          case 0x98:res(r.b, 3);
            break;
          case 0x99:res(r.c, 3);
            break;
          case 0x9A:res(r.d, 3);
            break;
          case 0x9B:res(r.e, 3);
            break;
          case 0x9C:res(r.h, 3);
            break;
          case 0x9D:res(r.l, 3);
            break;
          case 0x9E:res(&(*m).data()[r.hl], 3);
            break;
          case 0x9F:res(r.a, 3);
            break;
          case 0xA0:res(r.b, 4);
            break;
          case 0xA1:res(r.c, 4);
            break;
          case 0xA2:res(r.d, 4);
            break;
          case 0xA3:res(r.e, 4);
            break;
          case 0xA4:res(r.h, 4);
            break;
          case 0xA5:res(r.l, 4);
            break;
          case 0xA6:res(&(*m).data()[r.hl], 4);
            break;
          case 0xA7:res(r.a, 4);
            break;
          case 0xA8:res(r.b, 5);
            break;
          case 0xA9:res(r.c, 5);
            break;
          case 0xAA:res(r.d, 5);
            break;
          case 0xAB:res(r.e, 5);
            break;
          case 0xAC:res(r.h, 5);
            break;
          case 0xAD:res(r.l, 5);
            break;
          case 0xAE:res(&(*m).data()[r.hl], 5);
            break;
          case 0xAF:res(r.a, 5);
            break;
          case 0xB0:res(r.b, 6);
            break;
          case 0xB1:res(r.c, 6);
            break;
          case 0xB2:res(r.d, 6);
            break;
          case 0xB3:res(r.e, 6);
            break;
          case 0xB4:res(r.h, 6);
            break;
          case 0xB5:res(r.l, 6);
            break;
          case 0xB6:res(&(*m).data()[r.hl], 6);
            break;
          case 0xB7:res(r.a, 6);
            break;
          case 0xB8:res(r.b, 7);
            break;
          case 0xB9:res(r.c, 7);
            break;
          case 0xBA:res(r.d, 7);
            break;
          case 0xBB:res(r.e, 7);
            break;
          case 0xBC:res(r.h, 7);
            break;
          case 0xBD:res(r.l, 7);
            break;
          case 0xBE:res(&(*m).data()[r.hl], 7);
            break;
          case 0xBF:res(r.a, 7);
            break;
          case 0xC0:set(r.b, 0);
            break;
          case 0xC1:set(r.c, 0);
            break;
          case 0xC2:set(r.d, 0);
            break;
          case 0xC3:set(r.e, 0);
            break;
          case 0xC4:set(r.h, 0);
            break;
          case 0xC5:set(r.l, 0);
            break;
          case 0xC6:set(&(*m).data()[r.hl], 0);
            break;
          case 0xC7:set(r.a, 0);
            break;
          case 0xC8:set(r.b, 1);
            break;
          case 0xC9:set(r.c, 1);
            break;
          case 0xCA:set(r.d, 1);
            break;
          case 0xCB:set(r.e, 1);
            break;
          case 0xCC:set(r.h, 1);
            break;
          case 0xCD:set(r.l, 1);
            break;
          case 0xCE:set(&(*m).data()[r.hl], 1);
            break;
          case 0xCF:set(r.a, 1);
            break;
          case 0xD0:set(r.b, 2);
            break;
          case 0xD1:set(r.c, 2);
            break;
          case 0xD2:set(r.d, 2);
            break;
          case 0xD3:set(r.e, 2);
            break;
          case 0xD4:set(r.h, 2);
            break;
          case 0xD5:set(r.l, 2);
            break;
          case 0xD6:set(&(*m).data()[r.hl], 2);
            break;
          case 0xD7:set(r.a, 2);
            break;
          case 0xD8:set(r.b, 3);
            break;
          case 0xD9:set(r.c, 3);
            break;
          case 0xDA:set(r.d, 3);
            break;
          case 0xDB:set(r.e, 3);
            break;
          case 0xDC:set(r.h, 3);
            break;
          case 0xDD:set(r.l, 3);
            break;
          case 0xDE:set(&(*m).data()[r.hl], 3);
            break;
          case 0xDF:set(r.a, 3);
            break;
          case 0xE0:set(r.b, 4);
            break;
          case 0xE1:set(r.c, 4);
            break;
          case 0xE2:set(r.d, 4);
            break;
          case 0xE3:set(r.e, 4);
            break;
          case 0xE4:set(r.h, 4);
            break;
          case 0xE5:set(r.l, 4);
            break;
          case 0xE6:set(&(*m).data()[r.hl], 4);
            break;
          case 0xE7:set(r.a, 4);
            break;
          case 0xE8:set(r.b, 5);
            break;
          case 0xE9:set(r.c, 5);
            break;
          case 0xEA:set(r.d, 5);
            break;
          case 0xEB:set(r.e, 5);
            break;
          case 0xEC:set(r.h, 5);
            break;
          case 0xED:set(r.l, 5);
            break;
          case 0xEE:set(&(*m).data()[r.hl], 5);
            break;
          case 0xEF:set(r.a, 5);
            break;
          case 0xF0:set(r.b, 6);
            break;
          case 0xF1:set(r.c, 6);
            break;
          case 0xF2:set(r.d, 6);
            break;
          case 0xF3:set(r.e, 6);
            break;
          case 0xF4:set(r.h, 6);
            break;
          case 0xF5:set(r.l, 6);
            break;
          case 0xF6:set(&(*m).data()[r.hl], 6);
            break;
          case 0xF7:set(r.a, 6);
            break;
          case 0xF8:set(r.b, 7);
            break;
          case 0xF9:set(r.c, 7);
            break;
          case 0xFA:set(r.d, 7);
            break;
          case 0xFB:set(r.e, 7);
            break;
          case 0xFC:set(r.h, 7);
            break;
          case 0xFD:set(r.l, 7);
            break;
          case 0xFE:set(&(*m).data()[r.hl], 7);
            break;
          case 0xFF:set(r.a, 7);
            break;
          default:spdlog::warn("Unknown CB opcode {0:x}", (*m)[r.pc + 1]);
            assert(0);
        }
        break;
      case 0xCC:call(getf(Z));
        break;
      case 0xCD:call(true);
        break;
      case 0xCE:adc(get8(r.pc + 1));
        break;
      case 0xCF:rst(0x08);
        break;
      case 0xD0:ret(!getf(C));
        break;
      case 0xD1:pop(&r.de);
        break;
      case 0xD2:jp(!(getf(C)));
        break;
      case 0xD4:call(!getf(C));
        break;
      case 0xD5:push(r.de);
        break;
      case 0xD6:sub(get8(r.pc + 1));
        break;
      case 0xD7:rst(0x10);
        break;
      case 0xD8:ret(getf(C));
        break;
      case 0xD9:spdlog::info("RETI");
        r.pc = pop16() - 1;
// Handle end of interrupt
        break;
      case 0xDA:jp(getf(C));
        break;
      case 0xDC:call(getf(C));
        break;
      case 0xDE:sbc(get8(r.pc + 1));
        break;
      case 0xDF:rst(0x18);
        break;
      case 0xE0:ld(&(*m).data()[get8(r.pc + 1) + 0xFF00], *r.a);
        break;
      case 0xE1:pop(&r.hl);
        break;
      case 0xE2:ld(&(*m).data()[*r.c], *r.a);
        break;
      case 0xE5:push(r.hl);
        break;
      case 0xE6:_and(get8(r.pc + 1));
        break;
      case 0xE7:rst(0x20);
        break;
      case 0xE8:half_carry_check_add16(r.sp, (signed) get8(r.pc + 1));
        carry_check_add16(r.sp, (signed) get8(r.pc + 1));
        r.sp += (signed) get8(r.pc + 1);
        zero_check(r.sp);
        clrf(N);
        break;
      case 0xE9:jp((*m)[r.hl]);
        break;
      case 0xEA:ld(&(*m)[get16(r.pc + 1)], *r.a);
        break;
      case 0xEE:_xor(get8(r.pc + 1));
        break;
      case 0xEF:rst(0x28);
        break;
      case 0xF0:ld(r.a, (*m)[get8(r.pc + 1) + 0xFF00]);
        break;
      case 0xF1:pop(&r.af);
        break;
      case 0xF2:ld(r.a, (*m)[*r.c]);
        break;
      case 0xF3:s.ime = false;
        break;
      case 0xF5:push(r.af);
        break;
      case 0xF6:_or(get8(r.pc + 1));
        break;
      case 0xF7:rst(0x30);
        break;
      case 0xF8:half_carry_check_add16(r.sp, (signed) get8(r.pc + 1));
        carry_check_add(r.sp, (signed) get8(r.pc + 1));
        r.hl = r.sp + (signed) get8(r.pc + 1);
        clrf(Z);
        clrf(Z);
        break;
      case 0xF9:ld16(&r.sp, r.hl);
        break;
      case 0xFA:ld(r.a, (*m)[get16(r.pc + 1)]);
        break;
      case 0xFB:s.ime = true;
        break;
      case 0xFE:cp(get8(r.pc + 1));
        break;
      case 0xFF:rst(0x38);
        break;
      default:spdlog::error("Unknown opcode {0:x}", (*m)[r.pc]);
        assert(0);
    }

    spdlog::info(log_buffer);

    r.pc += ((!s.lastop_cb) ? GBCPP::opDefinesTbl[s.lastop].length : GBCPP::cbDefinesTbl[s.lastop].length);
    s.cycles +=
        ((!s.lastop_cb) ? GBCPP::opDefinesTbl[s.lastop].cycles[0] : cbDefinesTbl[s.lastop].cycles[0]);
  }
 private:
  friend class Cpu;
  Registers r;
  CpuState s;
  MemMgr *m;
};

}

