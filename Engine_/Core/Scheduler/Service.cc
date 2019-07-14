#include "Service.h"

namespace z {
namespace sched {

Service::Service() {}

void Service::Run() {
	while (true) {
		ITaskQueue* q = nullptr;
		{
			std::unique_lock<std::mutex> lock(mWaitQueueMutex);
			while (mWaitQueue.size() == 0) {
				mWaitQueueCond.wait(lock);
			} 
			q = mWaitQueue.front();
			if (q->ShouldPopWhenSched()) {
				mWaitQueue.pop_front();
			}
		}

		if (q) q->OnSched();
	}
}

void Service::AddWaitingTaskQueue(ITaskQueue* q) {
	mWaitQueueMutex.lock();
	mWaitQueue.push_back(q);
	mWaitQueueMutex.unlock();

	q->SingleTheradSched() ?
		mWaitQueueCond.notify_one() :
		mWaitQueueCond.notify_all();
}


// === Starnd Task Queue === 
void StrandTaskQueue::Post(Task&& task) {
	// insert task node before head
	Node* new_node = new Node(std::forward<Task>(task));
	new_node->next = mTasksHead.load();
	// if task_head == new_node->next, task_head = new_node
	// if task_head != new_node->next, new_node->next = task_head 
	while (!mTasksHead.compare_exchange_weak(new_node->next, new_node));

	// add to service if accu == 0
	if (mAccu.fetch_add(1) == 0) {
		mService.AddWaitingTaskQueue(this);
	}
}

StrandTaskQueue::Node* StrandTaskQueue::ReverseListOrder(Node* cur, Node** head) {
	cur->next ? ReverseListOrder(cur->next, head)->next = cur : *head = cur;
	return cur;
}

void StrandTaskQueue::OnSched() {
	/* if post(task) in another thread when scheduler.
			post thread: add task --> accu.fetch_add()
			sched thread: accu.load() --> swap tasks --> run tasks --> accu.compare()

		1. accu.fetch_add() << accu.load() << accu.compare().
			the task already append to queue and will be called later, no need to enter wait queue again.
		2. accu.load() << accu.fetch_add() << accu.compare().
			compare fail and enter wait queue again here.
		3. accu.load() << accu.compare() << accu.fetch_add().
			compare ok and accu set to zero, enter wait queue in post thred.
	*/

	bool true_ = true, false_ = false;
	CHECK(mInSched.compare_exchange_strong(false_, true), "Strand enter sched failed.");

	int cur_accu = mAccu.load();
	Node* head = mTasksHead.exchange(nullptr);

	// call all tasks in reversed order
	if (head) {
		ReverseListOrder(head, &head)->next = nullptr;
		while (head) {
			Node* old_head = head;
			head = head->next;

			old_head->call();
			delete old_head;
		}
	}

	CHECK(mInSched.compare_exchange_strong(true_, false), "Strand leave sched failed.");

	// if accu == cur_accu, set accu == 0 and do nothing
	// if accu != cur_accu, add to service
	if (!mAccu.compare_exchange_strong(cur_accu, 0)) {
		mService.AddWaitingTaskQueue(this);
	}
};


// === Parallel Task Queue === 
void ParallelTaskQueue::Post(Task&& task) {
	std::unique_lock<std::mutex> lock(mTasksListMutex);
	mTasksList.emplace_back(task);
	if (++mTasksNum == 1) {
		mService.AddWaitingTaskQueue(this);
	}
}

void ParallelTaskQueue::OnSched() {
	while (true) {
		std::unique_lock<std::mutex> lock(mTasksListMutex);
		if (mTasksList.size() == 0) {
			return;
		}
		Task task = std::forward<Task>(mTasksList.front());
		mTasksList.pop_front();
		--mTasksNum;

		lock.unlock();

		task();
	}
};




}
}