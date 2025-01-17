#include "ctrie/ctrie.hpp"

int main()
{
    ctrie::CTrie<std::string> trie;

    trie.insert("cat", "A small domesticated animal");
    trie.insert("dog", "A loyal animal");

    std::string value;
    if (trie.find("cat", value)) {
        std::cout << "Found 'cat': " << value << std::endl;
    } else {
        std::cout << "'cat' not found!" << std::endl;
    }

    if (trie.find("fish", value)) {
        std::cout << "Found 'fish': " << value << std::endl;
    } else {
        std::cout << "'fish' not found!" << std::endl;
    }

    return 0;
}
