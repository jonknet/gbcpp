#pragma once

namespace GBCPP
{
	class MemMgr;
	class Registers;
	struct StateType;

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
	  StateType getstate();
	  Registers getregisters();
	  class CpuImpl;
	protected:
		CpuImpl* pImpl;
	};
}