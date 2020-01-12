#include<iostream>	
#include<vector>
#include<string>
#include<future>	//std::future
#include<chrono>	//std::chrono::seconds
#include"ThreadPool.h"

using namespace std;

int main() {
	ThreadPool pool(4);
	
	vector<future<string>> results;

	for(int i = 0; i < 8; ++i) {
		results.emplace_back(
			pool.enqueue([i] {
				cout << "Hello " << i << endl;
				this_thread::sleep_for(chrono::seconds(1));
				cout << "world " << i <<endl;
				return string("---thread ") + to_string(i) + string("  finished.---");
			})
		);
	}
	for(auto && result : results) {
		cout << result.get() << ' ';
	}
	cout << endl;

	return 0;
}
