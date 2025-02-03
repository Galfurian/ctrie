#include "ctrie/ctrie.hpp"

int main()
{
    ctrie::CTrie<int> trie;

    trie.insert("one", 1);
    trie.insert("two", 2);
    trie.insert("three", 3);

    std::cout << "Before removal:\n" << trie << std::endl;

    trie.remove("two");

    std::cout << "After removing 'two':\n" << trie << std::endl;

    return 0;
}
