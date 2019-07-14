#pragma once

#include "Service.h"
#include "Worker.h"

namespace z {
namespace sched {

class Scheduler {
public:
	Scheduler(Worker* worker) : mWorker(worker) {
	}

protected:
	Worker* mWorker{ nullptr };
};

class StrandScheduler : public Scheduler {
public:
	StrandScheduler(Worker* worker) :
		Scheduler(worker), 
		mTaskQueue(mWorker->GetService()) {
	}
	void PostStrand(Task&& task) {
		mTaskQueue.Post(std::forward<Task>(task));
	}

private:
	StrandTaskQueue mTaskQueue;
};


class ParallelScheduler : public Scheduler{
public:
	ParallelScheduler(Worker* worker) :
		Scheduler(worker),
		mTaskQueue(mWorker->GetService()) {
	}

	void PostPar(Task&& task) {
		mTaskQueue.Post(std::forward<Task>(task));
	}

protected:
	ParallelTaskQueue mTaskQueue;
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