#include "mm.h"
#include <assert.h>
#include <fstream>

using namespace std;

MemMgr::MemMgr() {
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

	memcpy_s(bootrom, 256, DMG_BOOT_ROM, 256);
}

void MemMgr::set(int i, u8 val) {
	assert(i >= 0 && i <= 0xFFFF);
	mem[i] = val;
}

u8& MemMgr::operator[](int i) {
	assert(i >= 0 && i <= 0xFFFF);
	if (i < 256 && mem[BOOT] == 0) {
		return bootrom[i];
	}
	return mem[i];
}

int MemMgr::loadrom(string filename) {
	fstream romfile;
	romfile.open(filename,ios::in | ios::binary);
	if (romfile.is_open()) {
		int i = 0;
		while (!romfile.eof()) {
			set(i++, romfile.get());
		}
		romfile.close();
		spdlog::info("Romfile loaded");
		return 0;
	}
	spdlog::error("Can't open rom file {}", filename);
	return -1;
}

MemMgr MM;