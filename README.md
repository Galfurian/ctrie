# CTrie

[![Ubuntu](https://github.com/Galfurian/ctrie/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/Galfurian/ctrie/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/Galfurian/ctrie/actions/workflows/windows.yml/badge.svg)](https://github.com/Galfurian/ctrie/actions/workflows/windows.yml)
[![MacOS](https://github.com/Galfurian/ctrie/actions/workflows/macos.yml/badge.svg)](https://github.com/Galfurian/ctrie/actions/workflows/macos.yml)
[![Documentation](https://github.com/Galfurian/ctrie/actions/workflows/documentation.yml/badge.svg)](https://github.com/Galfurian/ctrie/actions/workflows/documentation.yml)

CTrie is a **concurrent prefix tree** (trie) implementation in C++. It provides
an efficient, hierarchical data structure for storing key-value pairs where the
keys are strings. The library supports thread-safe operations (when compiled
with C++11 or later) and is suitable for use cases like autocomplete, dictionary
storage, or hierarchical data management.

## Features

- **Efficient Storage**: Stores keys hierarchically, minimizing redundancy for common prefixes.
- **Thread Safety**: Supports thread-safe operations when compiled with C++11 or later.
- **Key-Value Storage**: Associates string keys with arbitrary values.
- **Customizable**: Fully templated to store values of any type.
- **Pretty Printing**: Provides a string representation of the trie structure.

## Requirements

- **C++11 or later** (for thread safety and modern features)
- A standard C++ compiler

## Installation

Clone the repository:

```bash
git clone https://github.com/yourusername/ctrie.git
```

Include the ctrie.hpp header file in your project:

```cpp
#include "ctrie/ctrie.hpp"
```

That's it! There's no additional setup or dependencies.

## API Reference

`CTrie`

Member Functions:

- `bool insert(const std::string &key, T value)` Inserts a key-value pair into the trie.
- `bool find(const std::string &key, T &value)` const Finds the value associated with a key.
- `bool remove(const std::string &key)` Removes a key-value pair from the trie.
- `std::string toString() const` Returns a string representation of the trie.

## Contributing

Contributions are welcome! Feel free to submit a pull request or open an issue
if you have suggestions, bug reports, or improvements.

## License

This project is licensed under the MIT License - see the `LICENSE.md` file for details.

## Acknowledgments

Author: Enrico Fraccaroli (<enry.frak@gmail.com>)
