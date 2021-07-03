#pragma once
#include "spdlog/pattern_formatter.h"
#include "cpu.h"
#include <cstdio>

class opcode_flag : public spdlog::custom_flag_formatter {
public:
	void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override {
		char buffer[5];
		sprintf_s(buffer, "0x%02X", cpu.getlastop() & 0xFF);
		std::string str = buffer;
		dest.append(str.data(), str.data() + str.size());
	}

	std::unique_ptr<custom_flag_formatter> clone() const override {
		return spdlog::details::make_unique<opcode_flag>();
	}
};

class registers_flag : public spdlog::custom_flag_formatter {
public:
	void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override {
		char buffer[100];
		CpuState state = cpu.getstate();
		sprintf_s(buffer, "PC:%04X AF:%04X BC:%04X DE:%04X HL:%04X SP:%04X", state.pc, state.af, state.bc, state.de, state.hl, state.sp);
		std::string str = buffer;
		dest.append(str.data(), str.data() + str.size());
	}

	std::unique_ptr<custom_flag_formatter> clone() const override {
		return spdlog::details::make_unique<registers_flag>();
	}
};

class stack_flag : public spdlog::custom_flag_formatter {
public:
    void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override {
        char buffer[100];
        CpuState state = cpu.getstate();
        sprintf_s(buffer, "STACK %04X %04X", mm[state.sp], mm[state.sp + 1]);
        std::string str = buffer;
        dest.append(str.data(), str.data() + str.size());
    }

    std::unique_ptr<custom_flag_formatter> clone() const override {
        return spdlog::details::make_unique<stack_flag>();
    }
};
