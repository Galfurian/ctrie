#include "ctrie/ctrie.hpp"
#include <thread>

void insertKeys(ctrie::CTrie<int> &trie, const std::string &prefix, int start, int end)
{
    for (int i = start; i < end; ++i) {
        trie.insert(prefix + std::to_string(i), i);
    }
}

int main()
{
    ctrie::CTrie<int> trie;

    std::thread t1(insertKeys, std::ref(trie), "thread1_", 0, 50);
    std::thread t2(insertKeys, std::ref(trie), "thread2_", 50, 100);

    t1.join();
    t2.join();

    std::cout << "Trie contents:\n"
              << trie << std::endl;

    return 0;
}
