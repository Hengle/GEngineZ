#pragma once

#include <thread>
#include <vector>

#include "Service.h"

namespace z {
namespace sched {

class Worker {
public:
	Worker() {
	}

	virtual void Run() {
		mService.Run();
	}

	virtual void Stop() {
	
	}
	
	Service& GetService() {
		return mService;
	}

protected:
	Service mService;
};


class ThreadWorker : public Worker {
public:
	ThreadWorker(int n) : mThreadNum(n) {
		mThreads.resize(n);
	}

	void Run() override {
		for (int i = 0; i < mThreadNum; i++) {
			mThreads[i] = std::thread([this]() {
				this->GetService().Run();

			});
		}
	}

	void Stop() override {
		for (int i = 0; i < mThreadNum; i++) {
			mThreads[i].join();
		}
	}

private:
	int mThreadNum;
	std::vector<std::thread> mThreads;
};

}	// namespace sched
}	// namespace z