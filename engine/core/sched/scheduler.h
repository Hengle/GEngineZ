#pragma once

#include "service.h"
#include "worker.h"

namespace z {
namespace sched {

class Scheduler {
public:
	Scheduler(Worker* worker) : worker_(worker) {
	}

protected:
	Worker* worker_{ nullptr };
};

class StrandScheduler : public Scheduler {
public:
	StrandScheduler(Worker* worker) :
		Scheduler(worker), 
		taskq_(worker_->GetService()) {
	}
	void PostStrand(Task&& task) {
		taskq_.Post(std::forward<Task>(task));
	}

private:
	StrandTaskQueue taskq_;
};


class ParallelScheduler : public Scheduler{
public:
	ParallelScheduler(Worker* worker) :
		Scheduler(worker),
		taskq_(worker_->GetService()) {
	}

	void PostPar(Task&& task) {
		taskq_.Post(std::forward<Task>(task));
	}

protected:
	ParallelTaskQueue taskq_;
};



class ComplexScheduler:
	public ParallelScheduler, public StrandScheduler {
public:
	ComplexScheduler(Worker* worker) :
		ParallelScheduler(worker), StrandScheduler(worker) {
	}
};


}	// namespace sched
}	// namespace z