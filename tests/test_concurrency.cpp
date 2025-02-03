#include "ctrie/ctrie.hpp"

#if __cplusplus < 201103L

int main(int, char *[]) { return 0; }
#else

#include <condition_variable>
#include <iostream>
#include <thread>

ctrie::CTrie<int> trie; // (3)

#define KEY "test"

std::mutex mutex_;
std::condition_variable condVar1;
std::condition_variable condVar2;

const int COUNTLIMIT = 10;

void initialize() { trie.insert(KEY, 0); }

void increment_even() // (1)
{
    int value;
    while (true) { // (7)
        std::unique_lock<std::mutex> lck(mutex_);
        condVar1.wait(lck, [&] {
            trie.find(KEY, value);
            return ((value % 2) == 0) || (value >= COUNTLIMIT);
        }); // (4)
        if (value >= COUNTLIMIT) {
            break;
        }
        trie.insert(KEY, value + 1);
        condVar2.notify_one(); // (6)
    }
}

void increment_odd() // (2)
{
    int value;
    while (true) { // (8)
        std::unique_lock<std::mutex> lck(mutex_);
        condVar2.wait(lck, [&] {
            trie.find(KEY, value);
            return ((value % 2) != 0) || (value >= COUNTLIMIT);
        });
        if (value >= COUNTLIMIT) {
            break;
        }
        trie.insert(KEY, value + 1);
        condVar1.notify_one();
    }
}

int main(int, char *[])
{
    int value;
    for (unsigned i = 0; i < 1000; ++i) {
        initialize();

        std::thread t1(increment_even);
        std::thread t2(increment_odd);

        t1.join();
        t2.join();

        trie.find(KEY, value);

        if (value != COUNTLIMIT) {
            std::cerr << "During run " << i << " we found the wrong value " << value << "\n";
            return 1;
        }
    }
    return 0;
}
#endif
