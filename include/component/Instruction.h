//
// Created by Jon on 7/11/2021.
//

#pragma once

#include "Runnable.h"
#include "Registers.h"
#include "stddefs.h"
#include <functional>
#include <vector>
#include <variant>
#include <bitset>

namespace ModernBoy {

using operand = std::variant<uint16_t &,uint16_t ,RegisterEnum,std::nullptr_t>;

struct Parameters {
  operand dest;
  operand src;
};

using op_func = std::function<void(Parameters&)>;

enum class ParamAddressMode {
  IMMEDIATE,	// op d8
  IMM_EXTENDED, // op d16
  RELATIVE, // jr s8
  DIRECT, // op (a16)
  REGISTER,
  REG_INDIRECT, // 1
  IMPLIED,	// xor, add, sub, etc
  BIT,
  SPECIAL_1, // 1 byte
  SPECIAL_2,
  SPECIAL_3,
  NONE = 0
};

enum class CpuFlags {
  ZERO = (1 << 7),
  SUBTRACT = (1 << 6),
  HALF_CARRY = (1 << 5),
  CARRY = (1 << 4)
};

enum class OpFlagChecks {
  HALF_CARRY_ADD = 1,
  HALF_CARRY_SUB = (1 << 2),
  CARRY_ADD = (1 << 3),
  CARRY_SUB = (1 << 4),
  ZERO = (1 << 5),
  SUBTRACTION = (1 << 6),
  NONE = 0
};

enum class OpType : int {
  NORMAL = 0,
  PREFIXED = 1
};

const auto ParamAddressModeSize[] = { 2, 3, 2, 3, 1, 1, 1, 1, 1, 2, 3 };



struct Opcode {
  std::byte op = 0;
  ParamAddressMode param_address_mode = ParamAddressMode::NONE;
  OpType type = OpType::NORMAL;
  struct {
	uint8_t std = 4;
	uint8_t jmp = 0;
  } cycles;
};

class Instruction : private Runnable {
 public:
  static Instruction& _op_func_tbl[2][256] {nullptr};
  explicit Instruction(Opcode op){
	if(_op_func_tbl[op.type][op.op] == nullptr){
	  data_ = op;
	  _op_func_tbl[op.type][op.op] = &this;
	}
  }
 protected:
  struct Opcode data_={};
  struct Parameters params_={};
};

}

