/*!
 * \file    lib/EHash.h
 * \date    2025-09-22
 * \author  elijw
 * \license MIT
 *
 * \brief   simple hashmap using separate chaining with linked lists.
 *
 * \note    hi rival; header-only template.
 * \todo    error handling (optional at the moment)
 */

#pragma once
#include <vector>
#include <functional>
#include "ELinkedList.h"

/*!
 * \brief   hashmap implementation.
 *
 * \tparam K key type.
 * \tparam V value type.
 *
 * \note    uses std::hash internally; separate chaining with ELinkedList.
 */
template<typename K, typename V> class EHash
{
    /*!
     * \brief   internal key-value pair.
     */
    struct Pair
    {
        K key;   //!< the key
        V value; //!< associated value
    };

    std::vector<ELinkedList<Pair>> buckets; //!< array of buckets
    size_t numElements = 0;                 //!< number of elements
    float maxLoad = 0.75f;                  //!< load factor threshold

    /*!
     * \brief   compute hash index for a key.
     *
     * \param key key to hash
     * \return bucket index
     *
     * \note    uses std::hash<K>; modulo buckets.size().
     */
    size_t hashKey(const K& key) const
    {
        return std::hash<K>{}(key) % buckets.size();
    }

    /*!
     * \brief   double bucket size and rehash all elements.
     *
     * \note    called automatically when load factor > maxLoad.
     *          simple O(n) operation; old nodes are re-inserted.
     */
    void rehash()
    {
        std::vector<ELinkedList<Pair>> old = buckets;
        buckets.clear();
        buckets.resize(old.size() * 2);

        for (auto& list : old)
        {
            ENode<Pair>* node = list.head;
            while (node)
            {
                insert(node->value.key, node->value.value);
                node = node->next;
            }
        }
    }

  public:
    /*!
     * \brief   construct new hash map.
     *
     * \param size initial number of buckets (default: 8)
     */
    explicit EHash(size_t size = 8) : buckets(size) {}

    /*!
     * \brief   insert or update a key-value pair.
     *
     * \param key key to insert
     * \param value value to insert
     *
     * \note    if key exists, value is overwritten.
     */
    void insert(const K& key, const V& value)
    {
        /* rehash if load factor exceeded */
        if ((float)numElements / buckets.size() > maxLoad)
        {
            rehash();
        }

        size_t idx = hashKey(key);

        /* find existing key */
        Pair* found =
            buckets[idx].find([&](const Pair& p) { return p.key == key; });
        if (found)
        {
            found->value = value;
            return;
        }

        /* insert new pair at bucket head */
        buckets[idx].insert({key, value});
        numElements++;
    }

    /*!
     * \brief   find value by key.
     *
     * \param key key to search
     * \return pointer to value if found, nullptr otherwise
     *
     * \note    nullptr return is intentional; no exception thrown.
     */
    V* find(const K& key)
    {
        size_t idx = hashKey(key);
        Pair* found =
            buckets[idx].find([&](const Pair& p) { return p.key == key; });
        return found ? &found->value : nullptr;
    }

    /*!
     * \brief   remove value by key.
     *
     * \param key key to remove
     * \return true if removed, false otherwise
     *
     * \note    returns false if key not found; does not throw.
     */
    bool remove(const K& key)
    {
        size_t idx = hashKey(key);
        if (buckets[idx].remove([&](const Pair& p) { return p.key == key; }))
        {
            numElements--;
            return true;
        }
        return false;
    }
};

;
