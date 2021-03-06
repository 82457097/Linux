#include<iostream>
#include<vector>
#include<cassert>
#include<ctime>
#include"StackAlloc.h"
#include"MemoryPool.h"

#define ELEMS 100000
#define REPS 100

using namespace std;

int main() {
	clock_t start;

	StackAlloc<int, allocator<int>> stackDefault;
	start = clock();
	for(int i = 0; i < REPS; ++i) {
		assert(stackDefault.Empty());
		for(int j = 0; j < ELEMS; ++j)
			stackDefault.Push(j);
		for(int j = 0; j < ELEMS; ++j)
			stackDefault.Pop();
	}
	cout << "Default Allocator Time: ";
	cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl;

	StackAlloc<int, MemoryPool<int>> stackPool;
	start = clock();
	for(int i = 0; i < REPS; ++i) {
		assert(stackPool.Empty());
		for(int j = 0; j < ELEMS; ++j)
			stackPool.Push(j);
		for(int j = 0; j < ELEMS; ++j)
			stackPool.Pop();
	}
	cout << "MemoryPool Allocator Time: ";
	cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl;
	
	return 0;
}

