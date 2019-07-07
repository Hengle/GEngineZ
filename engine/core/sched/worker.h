#pragma once

#include <thread>
#include <vector>

#include "service.h"

namespace z {
namespace sched {

class Worker {
public:
	Worker() {
	}

	virtual void Run() {
		service_.Run();
	}

	virtual void Stop() {
	
	}
	
	Service& GetService() {
		return service_;
	}

protected:
	Service service_;
};


class ThreadWorker : public Worker {
public:
	ThreadWorker(int n) : thread_num_(n) {
		ts_.resize(n);
	}

	void Run() override {
		for (int i = 0; i < thread_num_; i++) {
			ts_[i] = std::thread([this]() {
				this->GetService().Run();

			});
		}
	}

	void Stop() override {
		for (int i = 0; i < thread_num_; i++) {
			ts_[i].join();
		}
	}

private:
	int thread_num_;
	std::vector<std::thread> ts_;
};

}	// namespace sched
}	// namespace z