#pragma once

#include <functional>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <mutex>

#include <Core/Common/Common.h>


namespace z {
namespace sched {

// Task
typedef std::function<void()> Task;

// TaskQueue Interface
class ITaskQueue {
public:

	virtual void Post(Task&& task) = 0;
	virtual void OnSched() = 0;

	virtual bool ShouldPopWhenSched() = 0;
	virtual bool SingleTheradSched() = 0;
private:

};


// Service
class Service {
public:
	Service();
	void Run();
	void AddWaitingTaskQueue(ITaskQueue* q);

private:
	std::list<ITaskQueue*> mWaitQueue;
	std::mutex mWaitQueueMutex;
	std::condition_variable mWaitQueueCond;

	Service(Service const&) = delete;
	void operator =(Service const&) = delete;
};


// TaskQueue
class TaskQueue : public ITaskQueue {
public:
	TaskQueue(Service& service) : mService(service) {}
protected:
	Service& mService;
};


// StrandTaskQueue
class StrandTaskQueue : public TaskQueue {
private:
	struct Node {
		Task call;
		Node* next;
		Node(Task&& call) : call(call), next(nullptr) {}
	};

public:
	StrandTaskQueue(Service& service) : TaskQueue(service) {}

	bool ShouldPopWhenSched() override {
		return true;
	}

	bool SingleTheradSched() override {
		return true;
	}

	void Post(Task&& task) override;
	void OnSched();

private:
	Node* ReverseListOrder(Node* cur, Node** head);

	std::atomic<int> mAccu{ 0 };
	std::atomic<Node*> mTasksHead{ nullptr };

	std::atomic<bool> mInSched{ false };
};


// ParallelTaskQueue
class ParallelTaskQueue : public TaskQueue {

public:
	ParallelTaskQueue(Service& service) : TaskQueue(service) {}

	bool ShouldPopWhenSched() override {
		return mTasksNum == 0;
	}

	bool SingleTheradSched() override {
		return false;
	}

	void Post(Task&& task) override;
	void OnSched() override;

private:
	std::list<Task> mTasksList;
	std::mutex mTasksListMutex;
	std::atomic<int> mTasksNum{0};
};


} // namespace sched
} // namespace z
