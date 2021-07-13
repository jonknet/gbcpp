#pragma once
#include "../StdDefs.h"
#include "ITickable.h"
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

namespace ModernBoy {

  class ISynchronized {
   public:
	void signal();
   protected:
    explicit ISynchronized(ITickable& member);
   private:
    static std::vector<ITickable&> members_;
    ~ISynchronized();
  };

  auto ISynchronized::members_ = std::vector<ITickable&>();

  ISynchronized::ISynchronized(ITickable& member) {
    members_.push_back(member);
  }

  void ISynchronized::signal() {
	std::for_each(members_.begin(),members_.end(),[](ITickable& e){ e.tick(); });
  }

  ISynchronized::~ISynchronized() {
	for(auto it = std::begin(members_); it != std::end(members_); ++it){
	  if(this == &(*it)) { members_.erase(it); }
	}
	if(members_.empty()){ delete members_; return; }
  }

}