
/*!
 * \file    lib/EHash.h
 * \date    2025-09-22
 * \author  elijw
 * \license MIT
 *
 * \brief   simple hashmap using separate chaining with std::list.
 *
 * \note    hi rival
 * \todo    error handling (optional at the moment)
 */

#pragma once
#include <vector>
#include <list>
#include <functional>

/*!
 * \brief   hashmap implementation.
 *
 * \tparam  K key type.
 * \tparam  V value type.
 *
 * \note    uses std::hash internally; separate chaining with std::list.
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

    std::vector<std::list<Pair>> buckets; //!< array of buckets
    size_t numElements = 0;               //!< number of elements
    float maxLoad = 0.75f;                //!< load factor threshold

    /*!
     * \brief   compute hash index for a key.
     */
    size_t hashKey(const K& key) const
    {
        return std::hash<K>{}(key) % buckets.size();
    }

    /*!
     * \brief   double bucket size and rehash all elements.
     */
    void rehash()
    {
        std::vector<std::list<Pair>> old = buckets;
        buckets.clear();
        buckets.resize(old.size() * 2);

        for (auto& bucket : old)
        {
            for (auto& pair : bucket)
            {
                insert(pair.key, pair.value);
            }
        }
    }

  public:
    explicit EHash(size_t size = 8) : buckets(size) {}

    void insert(const K& key, const V& value)
    {
        if ((float)numElements / buckets.size() > maxLoad)
        {
            rehash();
        }

        size_t idx = hashKey(key);
        for (auto& pair : buckets[idx])
        {
            if (pair.key == key)
            {
                pair.value = value;
                return;
            }
        }

        buckets[idx].push_front({key, value});
        numElements++;
    }

    V* find(const K& key)
    {
        size_t idx = hashKey(key);
        for (auto& pair : buckets[idx])
        {
            if (pair.key == key) return &pair.value;
        }
        return nullptr;
    }

    bool remove(const K& key)
    {
        size_t idx = hashKey(key);
        auto& bucket = buckets[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it)
        {
            if (it->key == key)
            {
                bucket.erase(it);
                numElements--;
                return true;
            }
        }
        return false;
    }
};

;
