#pragma once

#include <functional>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <mutex>

#include <core/common/common.h>


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
	std::list<ITaskQueue*> waitq_;
	std::mutex waitq_mutex_;
	std::condition_variable waitq_cond_;

	Service(Service const&) = delete;
	void operator =(Service const&) = delete;
};


// TaskQueue
class TaskQueue : public ITaskQueue {
public:
	TaskQueue(Service& service) : service_(service) {}
protected:
	Service& service_;
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

	std::atomic<int> accu_{ 0 };
	std::atomic<Node*> tasks_head_{ nullptr };

	std::atomic<bool> in_sched_{ false };
};


// ParallelTaskQueue
class ParallelTaskQueue : public TaskQueue {

public:
	ParallelTaskQueue(Service& service) : TaskQueue(service) {}

	bool ShouldPopWhenSched() override {
		return tasks_num_ == 0;
	}

	bool SingleTheradSched() override {
		return false;
	}

	void Post(Task&& task) override;
	void OnSched() override;

private:
	std::list<Task> tasks_list_;
	std::mutex tasks_list_mtx_;
	std::atomic<int> tasks_num_{0};
};


} // namespace sched
} // namespace z
