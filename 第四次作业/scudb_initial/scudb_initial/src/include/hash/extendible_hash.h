/*
 * extendible_hash.h : implementation of in-memory hash table using extendible
 * hashing
 *
 * Functionality: The buffer pool manager must maintain a page table to be able
 * to quickly map a PageId to its corresponding memory location; or alternately
 * report that the PageId does not match any currently-buffered page.
 */

#pragma once

#include <cstdlib>
#include <unordered_map>

#include "hash/hash_table.h"
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace scudb {

    template <typename K, typename V>
    class ExtendibleHash : public HashTable<K, V> {

        struct Bucket {
            int localDepth;
            size_t maxSize{};
            std::unordered_map<K, V> vals;

            // constructor
            Bucket(int d, int s) : localDepth(d), maxSize(s){};

            bool find(K key);
            bool remove(K key);
            bool insert(K key, V value);

            bool isFull();
            bool isEmpty();
            int getDepth();
            void clear();
        };

        using Buckets = std::vector<std::shared_ptr<Bucket>>;

    public:
        // constructor
        explicit ExtendibleHash(size_t bucketSize = 3);
        // helper function to generate hash addressing
        size_t HashKey(const K &key) const;
        // helper function to get global & local depth
        int GetGlobalDepth() const;
        int GetLocalDepth(int bucket_id) const;
        // helper function to get the number of elements in the hash map
        int getSize() const;
        int GetNumBuckets() const;
        // lookup and modifier
        bool Find(const K &key, V &value) override;
        bool Remove(const K &key) override;
        void Insert(const K &key, const V &value) override;


    private:
        int globalDepth;
        Buckets cache;
        size_t bucketMaxSize;
        size_t size{};
        std::recursive_mutex mutex;

        void split(const std::shared_ptr<Bucket> &targetBucket);

    };

} // namespace scudb