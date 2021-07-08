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