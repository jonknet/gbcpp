#include "stddefs.h"
#include "spdlog/spdlog.h"
#include "mm.h"
#include <cassert>
#include <fstream>
#include <filesystem>

using namespace MemNS;

MemMgr::MemMgr() : mem{0}{
    mem[TIMA] = 0;
    mem[TMA] = 0;
    mem[TAC] = 0;
    mem[NR10] = 0x80;
    mem[NR11] = 0xBF;
    mem[NR12] = 0xF3;
    mem[NR14] = 0xBF;
    mem[NR21] = 0x3F;
    mem[NR22] = 0;
    mem[NR24] = 0xBF;
    mem[NR30] = 0x7F;
    mem[NR31] = 0xFF;
    mem[NR32] = 0x9F;
    mem[NR34] = 0xBF;
    mem[NR41] = 0xFF;
    mem[NR42] = 0;
    mem[NR43] = 0;
    mem[NR44] = 0xBF;
    mem[NR50] = 0x77;
    mem[NR51] = 0xF3;
    mem[NR52] = 0xF1;
    mem[LCDC] = 0x91;
    mem[SCY] = 0;
    mem[SCX] = 0;
    mem[LYC] = 0;
    mem[BGP] = 0xFC;
    mem[OBP0] = 0xFF;
    mem[OBP1] = 0xFF;
    mem[WY] = 0;
    mem[WX] = 0;
    mem[IE] = 0;
}

void MemMgr::init() {

}

u8 &MemMgr::operator[](std::size_t i) {
    assert(i >= 0 && i <= 0xFFFF);
    if (i < 256 && mem[BOOT] == 0) {
        return const_cast<u8 &>(BOOT_ROM[i]);
    }
    return mem[i];
}

std::array<u8, 0x10000> &MemMgr::data() {
    return mem;
}

int MemMgr::load_rom(const std::string &filename, MemMgr *mgr) {
    std::ifstream rom{filename, std::ios::in | std::ios::binary};
    auto &arr = mgr->data();
    if (rom.is_open()) {
        rom.read(reinterpret_cast<char *>(arr.data()),
                 static_cast<long long>(std::filesystem::file_size(filename)));
        spdlog::info("Rom file loaded");
        rom.close();
        return 0;
    }
    spdlog::error("Can't open rom file {}", filename);
    return -1;
}


