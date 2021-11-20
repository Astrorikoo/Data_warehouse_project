/**
 * LRU implementation
 */
#include "buffer/lru_replacer.h"
#include "page/page.h"

namespace scudb {

template <typename T> LRUReplacer<T>::LRUReplacer():capacity(6),size(0) {
    dummyHead = new ListNode(0);
    dummyTail = new ListNode(0);

    dummyHead->next = dummyTail;
    dummyHead->prev = nullptr;

    dummyTail->next = nullptr;
    dummyTail->prev = dummyHead;

}

template <typename T> LRUReplacer<T>::~LRUReplacer() {

    ListNode *next;
    ListNode *cur = dummyHead;

    for(int i=0;i<size+2;i++){
        next = cur->next;
        delete cur;
        cur = next;
    }
}

/*
 * Insert value into LRU
 */
template <typename T> void LRUReplacer<T>::Insert(const T &value) {
    std::lock_guard<std::recursive_mutex> guard(mutex);
    if(!cache.count(value)){
        //if value not in cache
        auto *node = new ListNode(value);
        cache[value] = node;

        node->prev = dummyHead;
        node->next = dummyHead->next;

        dummyHead->next->prev = node;
        dummyHead->next = node;

        size++;
    }else{

        //if in then put the value to the head
        auto *node = cache[value];

        node->prev->next = node->next;
        node->next->prev = node->prev;

        node->prev = dummyHead;
        node->next = dummyHead->next;

        dummyHead->next->prev = node;
        dummyHead->next = node;
    }
}

/* If LRU is non-empty, pop the head member from LRU to argument "value", and
 * return true. If LRU is empty, return false
 */
template <typename T> bool LRUReplacer<T>::Victim(T &value) {

    std::lock_guard<std::recursive_mutex> guard(mutex);
    if(size !=0){
        value = dummyTail->prev->farme_id;
        auto *p = dummyTail->prev;
        p->prev->next = dummyTail;
        dummyTail->prev = p->prev;

        delete p;
        size--;
        cache.erase(value);
        return true;
    }

    return false;
}

/*
 * Remove value from LRU. If removal is successful, return true, otherwise
 * return false
 */
template <typename T> bool LRUReplacer<T>::Erase(const T &value) {
    std::lock_guard<std::recursive_mutex> guard(mutex);
    if(cache.count(value)){
        auto *node = cache[value];
        // think the condition of head or tail

        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->prev = nullptr;
        node->next = nullptr;

        cache.erase(value);
        size--;

        return true;
    }

    return false;
}

template <typename T> size_t LRUReplacer<T>::Size() { return size; }

template class LRUReplacer<Page *>;
// test only
template class LRUReplacer<int>;

} // namespace scudb
