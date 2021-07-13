
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once

namespace GBCPP
{
	class MemMgr;
	class Registers;
	struct CpuState;

	class Cpu
	{
	 public:
	  explicit Cpu(MemMgr& mm);
	  void tick();
	  void exec();
	  void run();
	  void pause();
	  void sethalt(bool h);
	  void setstop(bool s);
	  void reset();
	  CpuState getstate();
	  Registers getregisters();
	  class CpuImpl;
	protected:
		CpuImpl* pImpl;
	};
}