#include "ctrie/ctrie.hpp"

int main()
{
    ctrie::CTrie<std::string> trie;

    trie.insert("hello", "world");
    trie.insert("hi", "there");

    std::string serialized = trie.toString();
    std::cout << "Serialized Trie: " << serialized << std::endl;

    return 0;
}
