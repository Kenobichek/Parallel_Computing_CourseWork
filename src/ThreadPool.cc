#include "ThreadPool.h"

ThreadPool::ThreadPool(const int thread_count) {
	write_lock _(rw_lock);

	if (initialized || terminated) {
		return;
	}

	threads.reserve(thread_count);

	for (int i = 0; i < thread_count; i++) {
		threads.emplace_back(&ThreadPool::Routine, this);
	}

	initialized = !threads.empty();
}

ThreadPool::~ThreadPool() {
	Terminate();
}

void ThreadPool::Routine()
{
	while (true)
	{
		std::function<void ()>  task;
		{
			write_lock _(rw_lock);
			task_waiter.wait(rw_lock, [this] {
				return terminated || !tasks_queue.empty(); 
			});

			if (terminated && tasks_queue.empty()) {
				return;
			}

			task = tasks_queue.front();
			tasks_queue.pop();
		}
		task();
	}
}

void ThreadPool::AddTask(std::function<void()> fun) {
	read_lock _(rw_lock);
	if (!WorkingUnsafe()) {
		return;
	}

	if (tasks_queue.emplace(fun)) {
		task_waiter.notify_one();
	}
}

void ThreadPool::Terminate(const bool should_finish_tasks) {
	{
		write_lock _(rw_lock);

		if (!should_finish_tasks) {
			threads.clear();
		}
		if (WorkingUnsafe()) {
			terminated = true;
		}
		else {
			threads.clear();
			terminated = false;
			initialized = false;
			return;
		}
	}

	task_waiter.notify_all();

	for (std::thread& thread : threads) {
		thread.join();
	}

	threads.clear();
	terminated = false;
	initialized = false;
}

bool ThreadPool::WorkingUnsafe() const
{
	return initialized && !terminated;
}
