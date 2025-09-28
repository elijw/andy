/*!
 * \file    tests/test_ehash.cpp
 * \date    2025-09-26
 * \author  elijw
 * \license MIT
 *
 * \brief   unit tests and benchmarks for EHash,
 *          compared against std::unordered_map.
 */

#include "../lib/EHash.h"
#include <cassert>
#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include <unordered_map>

/*!
 * \brief   basic unit tests for correctness for EHash
 *
 * \note    will abort if any test fails.
 */
void unit_tests()
{
    EHash<std::string, int> emap(10);
    emap.insert("apple", 1);
    emap.insert("banana", 2);
    emap.insert("cherry", 3);

    // lookup elements
    assert(emap.find("apple") && *emap.find("apple") == 1);
    assert(emap.find("banana") && *emap.find("banana") == 2);
    assert(emap.find("cherry") && *emap.find("cherry") == 3);

    // overwrite existing key
    emap.insert("banana", 42);
    assert(emap.find("banana") && *emap.find("banana") == 42);

    // remove an element
    emap.remove("banana");
    assert(emap.find("banana") == nullptr);

    // remove a non-existent key (should not throw)
    emap.remove("does_not_exist");

    std::cout << "[TEST] all EHash unit tests passed!\n";
}

/*!
 * \brief   mixed workload benchmark on EHash.
 *
 * \param N number of operations to run
 * \param rng_seed deterministic seed for reproducibility
 * \return checksum (to prevent compiler optimizing away calls)
 */
long long bench_ehash(size_t N, uint64_t rng_seed)
{
    // start with half as many buckets as elements to force collisions
    EHash<int, int> emap(N / 2);

    std::mt19937_64 rng(rng_seed);
    std::uniform_int_distribution<int> dist(1, static_cast<int>(N * 10));

    long long checksum = 0;

    auto start = std::chrono::high_resolution_clock::now();

    /* 50% inserts, 40% finds, 10% removals */
    for (size_t i = 0; i < N; ++i)
    {
        int op = rng() % 100;
        int key = dist(rng);

        if (op < 50) /* insert */
        {
            emap.insert(key, key * 2);
        }
        else if (op < 90) /* find */
        {
            auto val = emap.find(key);
            if (val) checksum += *val;
        }
        else /* remove */
        {
            emap.remove(key);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    double total_time = std::chrono::duration<double>(end - start).count();
    double avg_ns =
        std::chrono::duration<double, std::nano>(end - start).count() / N;

    std::cout << "[esda::uo_ehash]\n";
    std::cout << "   ├─ total time: " << total_time << "s\n";
    std::cout << "   ├─ avg per operation: " << avg_ns << " ns\n";
    std::cout << "   └─ checksum: " << checksum << "\n";

    return checksum;
}

/*!
 * \brief   run a mixed workload benchmark on std::unordered_map.
 *
 * \param N number of operations to run
 * \param rng_seed deterministic seed for reproducibility
 * \return checksum (to prevent compiler optimizing away calls)
 */
long long bench_stdmap(size_t N, uint64_t rng_seed)
{
    std::unordered_map<int, int> smap;
    smap.reserve(N / 2); // pre-allocate buckets, guarantee collisions again

    std::mt19937_64 rng(rng_seed);
    std::uniform_int_distribution<int> dist(1, static_cast<int>(N * 10));

    long long checksum = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < N; ++i)
    {
        int op = rng() % 100;
        int key = dist(rng);

        if (op < 50) /* insert */
        {
            smap[key] = key * 2;
        }
        else if (op < 90) /* find */
        {
            auto it = smap.find(key);
            if (it != smap.end()) checksum += it->second;
        }
        else /* remove */
        {
            smap.erase(key);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    double total_time = std::chrono::duration<double>(end - start).count();
    double avg_ns =
        std::chrono::duration<double, std::nano>(end - start).count() / N;

    std::cout << "[std::unordered_map]\n";
    std::cout << "   ├─ total time: " << total_time << "s\n";
    std::cout << "   ├─ avg per operation: " << avg_ns << " ns\n";
    std::cout << "   └─ checksum: " << checksum << "\n";

    return checksum;
}

/*!
 * \brief   benchmark EHash and std::unordered_map side by side at multiple
 * scales.
 */
void benchmark()
{
    std::vector<size_t> scales = {100'000, 10'000'000, 50'000'000};

    for (auto N : scales)
    {
        std::cout << "\n[BENCH] scale: " << N << " elements\n";

        long long c1 = bench_ehash(N, 12345);
        long long c2 = bench_stdmap(N, 12345);

        /* sanity: checksums should be identical if both behaved the same */
        if (c1 != c2)
        {
            std::cerr << "[WARN] checksum mismatch between EHash and "
                         "std::unordered_map\n";
        }
    }
}

int main()
{
    unit_tests();
    benchmark();
    return 0;
}
