#include "ctrie/ctrie.hpp"

int main()
{
    ctrie::CTrie<std::string> trie;

    trie.insert("apple", "A fruit");
    trie.insert("app", "Abbreviation for application");
    trie.insert("banana", "Another fruit");

    std::cout << "Trie contents:\n"
              << trie << std::endl;

    return 0;
}
