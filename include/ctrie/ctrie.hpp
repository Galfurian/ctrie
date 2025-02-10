/// @file ctrie.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief The ctrie main code.
#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#if __cplusplus >= 201103L
#include <mutex>
#endif

enum : unsigned char {
    CTRIE_MAJOR_VERSION = 1, ///< Major version of the library.
    CTRIE_MINOR_VERSION = 0, ///< Minor version of the library.
    CTRIE_MICRO_VERSION = 0  ///< Micro version of the library.
};

namespace ctrie
{

/// @brief The maximum number of keys.
#define MAX_KEYS 128

/// @brief The type of the key.
using key_t = char;

/// @brief A node of the prefix tree.
template <typename T>
class SNode
{
public:
    /// @brief Construct a new node.
    /// @param _value The value to store.
    explicit SNode(const T &_value)
        : value(_value)
    {
        // Nothing to do.
    }

    /// @brief Copy constructor.
    /// @param other The instance to copy from.
    SNode(const SNode &other) = delete;

    /// @brief Copy assignment operator.
    /// @param other The instance to copy from.
    /// @return Reference to the instance.
    auto operator=(const SNode &other) -> SNode & = delete;

    /// @brief Move constructor.
    /// @param other The instance to move from.
    SNode(SNode &&other) noexcept = delete;

    /// @brief Move assignment operator.
    /// @param other The instance to move from.
    /// @return Reference to the instance.
    auto operator=(SNode &&other) noexcept -> SNode & = delete;

    /// @brief Destruct the node.
    virtual ~SNode() = default;

    /// @brief Set the value of the node.
    /// @param _value The value to store.
    void setValue(const T &_value) { value = _value; }

    /// @brief Get the value of the node.
    /// @return The value of the node.
    auto getValue() const -> T { return value; }

private:
    /// The stored value.
    T value;
};

/// @brief A node of the prefix tree.
template <typename T>
class CNode : std::enable_shared_from_this<CNode<T>>
{
public:
    /// @brief Construct a new node.
    /// @param _parent The parent of the node.
    /// @param _key The key of the node.
    CNode(const std::shared_ptr<CNode<T>> &_parent, key_t _key)
        : parent(_parent)
        , key(_key)
        , snode()
        , children()
    {
        // Nothing to do.
    }

    /// @brief Construct a new node.
    /// @param _parent The parent of the node.
    /// @param _key The key of the node.
    /// @param _value The value of the node.
    CNode(std::shared_ptr<CNode<T>> _parent, key_t _key, const T &_value)
        : parent(_parent)
        , key(_key)
        , snode(std::make_shared<SNode<T>>(_value))
        , children()
    {
        // Nothing to do.
    }

    /// @brief Copy constructor.
    /// @param other The instance to copy from.
    CNode(const CNode &other) = delete;

    /// @brief Copy assignment operator.
    /// @param other The instance to copy from.
    /// @return Reference to the instance.
    auto operator=(const CNode &other) -> CNode & = delete;

    /// @brief Move constructor.
    /// @param other The instance to move from.
    CNode(CNode &&other) = delete;

    /// @brief Move assignment operator.
    /// @param other The instance to move from.
    /// @return Reference to the instance.
    auto operator=(CNode &&other) noexcept -> CNode & = delete;

    /// @brief Destruct the node.
    virtual ~CNode() = default;

    /// @brief Get the key of the node.
    /// @return The key of the node.
    auto getKey() const -> key_t { return key; }

    /// @brief Get the parent of the node.
    /// @return The parent of the node.
    auto getParent() const -> std::shared_ptr<CNode<T>> { return parent.lock(); }

    /// @brief Clear the stored value.
    void clearSNode() { snode.reset(); }

    /// @brief Set the stored value.
    /// @param _value The value to store.
    void setSNode(const T &_value) { snode = std::make_shared<SNode<T>>(_value); }

    /// @brief Get the stored value.
    /// @return The stored value.
    auto getSNode() const -> std::shared_ptr<SNode<T>> { return snode; }

    /// @brief Remove the child with the given key.
    /// @param c The key of the child to remove.
    /// @throws std::out_of_range if the key is out of bounds.
    void removeChild(key_t c)
    {
        // Check if the index is within the valid range of the children array.
        auto index = static_cast<std::size_t>(c);
        if (index >= children.size()) {
            throw std::out_of_range("removeChild: key out of bounds");
        }
        // Reset (remove) the child at the given index.
        children[index].reset();
    }

    /// @brief Insert a child with the given key.
    /// @param c The key of the child to insert.
    /// @param child The child to insert.
    /// @throws std::out_of_range if the key is out of bounds.
    void insertChild(key_t c, const std::shared_ptr<CNode<T>> &child)
    {
        // Check if the index is within the valid range of the children array.
        auto index = static_cast<std::size_t>(c);
        if (index >= children.size()) {
            throw std::out_of_range("insertChild: key out of bounds");
        }
        // Insert the child.
        children[index] = child;
    }

    /// @brief Get the child with the given key.
    /// @param c The key of the child to get.
    /// @return The child with the given key.
    /// @throws std::out_of_range if the key is out of bounds.
    auto at(key_t c) -> std::shared_ptr<CNode<T>>
    {
        // Check if the index is within the valid range of the children array.
        auto index = static_cast<std::size_t>(c);
        if (index >= children.size()) {
            throw std::out_of_range("at: key out of bounds");
        }
        // Return the child.
        return children[index];
    }

    /// @brief Get the child with the given key.
    /// @param c The key of the child to get.
    /// @return The child with the given key.
    /// @throws std::out_of_range if the key is out of bounds.
    auto at(key_t c) const -> std::shared_ptr<CNode<T>>
    {
        // Check if the index is within the valid range of the children array.
        auto index = static_cast<std::size_t>(c);
        if (index >= children.size()) {
            throw std::out_of_range("at (const): key out of bounds");
        }
        // Return the child.
        return children[index];
    }

    /// @brief Check if the node has children.
    /// @return true if the node has children, false otherwise.
    auto hasChildren() const -> bool
    {
        return std::any_of(children.begin(), children.end(), [](const auto &child) { return child != nullptr; });
    }

    /// @brief Get the string representation of the node.
    /// @param prefix The prefix to prepend to each line of the output.
    /// @param isLast Whether this node is the last child in a sequence.
    /// @return A string representing the node.
    auto toString(const std::string &prefix = "", bool isLast = true) const -> std::string
    {
        std::stringstream ss;
        // Print the current node with its prefix, except for the root.
        if (parent.lock()) {
            ss << prefix;
            ss << (isLast ? "└─" : "├─");
        }
        ss << key;
        if (snode) {
            ss << " : " << snode->getValue();
        }
        ss << "\n";
        // Compute the new prefix for children.
        std::string childPrefix = prefix + (isLast ? "  " : "│ ");
        // Iterate over children.
        for (std::size_t i = 0; i < children.size(); ++i) {
            if (children[i]) {
                // Determine if this child is the last one.
                ss << children[i]->toString(childPrefix, this->isLastChild(i));
            }
        }
        return ss.str();
    }

private:
    /// @brief Checks if there are more children after the given index.
    /// @param index The index to check.
    /// @return true if there are no children after the given index, false otherwise.
    auto isLastChild(std::size_t index) const -> bool
    {
        // Iterate through the children starting from the next index.
        for (std::size_t i = index + 1; i < children.size(); ++i) {
            // If a child exists at any position after the given index, return false.
            if (children[i]) {
                return false;
            }
        }
        // No children found after the given index, return true.
        return true;
    }

    /// A pointer to the parent.
    std::weak_ptr<CNode<T>> parent;
    /// The key associated with the node.
    key_t key;
    /// The stored value.
    std::shared_ptr<SNode<T>> snode;
    /// The childrens of the node.
    std::array<std::shared_ptr<CNode<T>>, MAX_KEYS> children;
};

/// @brief A prefix tree.
template <typename T>
class CTrie
{
public:
    /// @brief Construct a new ctrie.
    CTrie() = default;

    /// @brief Destroy the CTrie object.
    virtual ~CTrie() { _root.reset(); }

    /// @brief Copy constructor.
    CTrie(const CTrie &other) = delete;

    /// @brief Copy assignment operator.
    auto operator=(const CTrie &other) -> CTrie & = delete;

    /// @brief Move constructor.
    CTrie(CTrie &&other) noexcept = delete;

    /// @brief Move assignment operator.
    auto operator=(CTrie &&other) noexcept -> CTrie & = delete;

    /// @brief Inserts the key-value pair into the Trie.
    /// @param key The key to insert.
    /// @param value The value associated with the key.
    /// @return true if the insertion was successful, false otherwise.
    auto insert(const std::string &key, T value) -> bool
    {
        // Return false if the key is empty.
        if (key.empty()) {
            return false;
        }
#if __cplusplus >= 201103L
        // Automatically lock the mutex for thread safety.
        std::lock_guard<std::mutex> lock(_mutex);
#endif
        // Initialize the root node if it doesn't exist.
        if (!_root) {
            _root = std::make_shared<CNode<T>>(nullptr, 0);
        }
        // Start from the root node.
        auto node = _root;
        // Traverse the Trie, creating child nodes if they don't exist.
        for (const auto &ch : key) {
            auto child = node->at(ch);
            // Create a new child node if the current character doesn't exist.
            if (!child) {
                child = std::make_shared<CNode<T>>(node, ch);
                node->insertChild(ch, child);
            }
            // Move to the next child node.
            node = child;
        }
        // Set the value at the final node.
        node->setSNode(value);
        // Return true indicating successful insertion.
        return true;
    }

    /// @brief Find the value associated with the passed key.
    /// @param key the key to use for the search.
    /// @param value the output variable where the found value is stored.
    /// @return true if we have found the value, false otherwise.
    auto find(const std::string &key, T &value) const -> bool
    {
        // Return false if the key is empty or there's no root node.
        if (key.empty() || !_root) {
            return false;
        }
#if __cplusplus >= 201103L
        // Automatically lock and unlock the mutex.
        std::lock_guard<std::mutex> lock(_mutex);
#endif

        // Start from the root node.
        auto node = _root;
        // Traverse the trie using each character in the key.
        for (char ch : key) {
            // Move to the corresponding child node.
            node = node->at(ch);
            if (!node) {
                // Key path doesn't exist.
                return false;
            }
        }
        // If the node holds a value, assign it and return true.
        if (node->getSNode()) {
            value = node->getSNode()->getValue();
            return true;
        }
        // Key exists, but no associated value found.
        return false;
    }

    /// @brief Removes the key-value pair from the Trie.
    /// @param key The key to remove.
    /// @return true if the removal was successful, false otherwise.
    auto remove(const std::string &key) -> bool
    {
        // Return false if the key is empty or there's no root node.
        if (key.empty() || !_root) {
            return false;
        }
#if __cplusplus >= 201103L
        // Automatically lock and unlock the mutex.
        std::lock_guard<std::mutex> lock(_mutex);
#endif

        // Start from the root node.
        auto node = _root;
        // Traverse the Trie to find the node corresponding to the key.
        for (char ch : key) {
            node = node->at(ch);
            if (!node) {
                // Key path doesn't exist.
                return false;
            }
        }
        // If the node has an associated value, remove it.
        if (node->getSNode()) {
            // Clear the stored value.
            node->clearSNode();
            // Remove nodes up the parent chain if they meet removal conditions.
            while (node) {
                auto parent = node->getParent();
                if (!node->hasChildren() && parent && !node->getSNode()) {
                    parent->removeChild(node->getKey());
                    // Move to the parent node.
                    node = parent;
                } else {
                    // Stop if the current node shouldn't be removed.
                    break;
                }
            }
            // Key successfully removed.
            return true;
        }
        // Key exists but no value to remove.
        return false;
    }

    /// @brief Get the string representation of the tree.
    /// @return A string representing the tree.
    auto toString() const -> std::string
    {
        if (_root) {
            return _root->toString();
        }
        return std::string();
    }

private:
    /// The root of the tree.
    std::shared_ptr<CNode<T>> _root;
#if __cplusplus >= 201103L
    /// Internal mutex for thread safety.
    mutable std::mutex _mutex;
#endif
};

} // namespace ctrie

/// @brief Overload of the operator << for the CTrie class.
/// @tparam T The type of the value.
/// @param lhs the stream.
/// @param rhs the trie.
/// @return the stream.
template <typename T>
auto operator<<(std::ostream &lhs, const ctrie::CTrie<T> &rhs) -> std::ostream &
{
    lhs << rhs.toString();
    return lhs;
}
