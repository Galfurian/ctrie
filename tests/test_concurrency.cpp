#include "ctrie/ctrie.hpp"

#if __cplusplus < 201103L

int main(int, char *[])
{
    return 0;
}
#else

#include <condition_variable>
#include <iostream>
#include <thread>

ctrie::CTrie<int> trie; // (3)

#define KEY "test"

std::mutex mutex_;
std::condition_variable condVar1;
std::condition_variable condVar2;

int counter    = 0;
int COUNTLIMIT = 50;

void setTrue() // (1)
{
    int value;
    while (counter < COUNTLIMIT) { // (7)
        std::unique_lock<std::mutex> lck(mutex_);
        condVar1.wait(lck, [&] {
            trie.find(KEY, value);
            return (value % 2) == false;
        }); // (4)

        trie.insert(KEY, value + 1);

        ++counter; // (5)

        condVar2.notify_one(); // (6)
    }
}

void setFalse() // (2)
{
    int value;
    while (counter < COUNTLIMIT) { // (8)
        std::unique_lock<std::mutex> lck(mutex_);
        condVar2.wait(lck, [&] {
            trie.find(KEY, value);
            return (value % 2) == true;
        });
        trie.insert(KEY, value + 1);

        condVar1.notify_one();
    }
}

int main(int, char *[])
{
    trie.insert(KEY, 0);

    int value;
    trie.find(KEY, value);
    if (value != 0)
        return 1;

    std::thread t1(setTrue);
    std::thread t2(setFalse);

    t1.join();
    t2.join();

    trie.find(KEY, value);
    if (value != 100)
        return 1;
    return 0;
}
#endif
