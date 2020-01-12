#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include<vector>
#include<queue>
#include<memory>		
#include<stdexcept>				//runtime_error
#include<thread>				//std::thread
#include<mutex>					//std::mutex, std::unique_lock
#include<condition_variable>	//std::condition_variable
#include<future>				//std::future, std::packaged_task
#include<functional>			//std::function, std::bind
#include<utility>				//std::move, std::forward

using namespace std;

class ThreadPool {
public:
	inline ThreadPool(size_t threads) : stop(false) {
		for(size_t i = 0; i < threads; ++i) {
			workers.emplace_back([this] {
				for(;;) {
					function<void()> task;
					{
						unique_lock<mutex> lock(this->queue_mutex);
						this->condition.wait(lock, 
							[this] {
								return this->stop || !this->tasks.empty();
							});
						if(this->stop && this->tasks.empty())
							return;
						task = std::move(this->tasks.front());
						this->tasks.pop();
					}
					task();
				}
			});
		}
	}

	template<typename F, typename... Args>
	auto enqueue(F&& f, Args&&... args)
		->future<typename result_of<F(Args...)>::type> {
		using return_type = typename result_of<F(Args...)>::type;
		auto task = make_shared<packaged_task<return_type()>>(
			bind(forward<F>(f), forward<Args>(args)...)
		);
		future<return_type> res = task->get_future();
		{
			unique_lock<mutex> lock(queue_mutex);
			if(stop) {
				throw runtime_error("enqueue on stopped ThreadPool");
			}
			tasks.emplace([task] { (*task)(); });
		}
		condition.notify_one();
		return res;
	}

	inline ~ThreadPool() {
		{
			unique_lock<mutex> lock(queue_mutex);
			stop = true;
		}
		condition.notify_all();
		for(thread &worker: workers) {
			worker.join();
		}
	}

private:
	vector<thread> workers;
	queue<function<void()>> tasks;

	mutex queue_mutex;
	condition_variable condition;

	bool stop;

};

#endif