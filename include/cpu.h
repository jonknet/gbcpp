#ifndef __CPU__H
#define __CPU__H

#include "stddefs.h"
#include "mm.h"
#include "cpuimpl.h"

namespace GBCPP
{

	class CpuImpl;

	class Registers;

	struct StateType;

	class Cpu
	{
	private:
		CpuImpl* pImpl;
	public:
		explicit Cpu(MemMgr& mm);
		void exec();
		void run();
		void pause();
		void sethalt(bool h);
		void setstop(bool s);
		void reset();
		StateType getstate();
		Registers getregisters();
	};
}


#endif