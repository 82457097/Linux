#include<iostream>
#include<cassert>
#include<ctime>
#include<vector>

#include"MemoryPool.hpp"
#include"StackAlloc.hpp"

#define ELEMS 10000000
#define REPS 100

using namespace std;

int main() {
    clock_t start;

    StackAlloc<int,allocator<int>> stackDefault;
    start = clock();
    for(int j = 0; j < REPS; ++j) {
        assert(stackDefault.empty());
        for(int i = 0; i < ELEMS; ++i)
            stackDefault.push(i);
        for(int i = 0; i < ELEMS; ++i)
            stackDefault.pop();
    }
    cout << "Default Allocator Time: ";
    cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

    StackAlloc<int, MemoryPool<int>> stackPool;
    start = clock();
    for(int j = 0; j < REPS; ++j) {
        assert(stackPool.empty());
        for(int i = 0; i < ELEMS; ++i)
            stackPool.push(i);
        for(int i = 0; i < ELEMS; ++i)
            stackPool.pop();
    }
    cout << "MemoryPool Allocator Time: ";
    cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

    return 0;
}
