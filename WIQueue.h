//
// Created by amueller on 5/12/15.
//

#pragma once

#include <memory>
#include <atomic>

namespace GA {
    //WorkItem queue
    template<class T>
    class WIQueue {
        static const unsigned int LENGTH = 512U;
        static const unsigned int MASK = LENGTH - 1U;
        enum SLOTSTATE : unsigned char { FREE = 0U, WRITING = 1U, READY = 2U, READING = 3U };

        std::pair<SLOTSTATE, std::shared_ptr<T>> mData[LENGTH];
        std::atomic<unsigned int> mHead;
        std::atomic<unsigned int> mHeadDone;
        std::atomic<unsigned int> mTail;
        std::atomic<unsigned int> mTailDone;
    public:
        WIQueue();
        WIQueue(const WIQueue& source) = delete; //Disable copy constructor

        void Push(const std::shared_ptr<T>& value );
        std::shared_ptr<T> Pop();

    };
}

// ==== Implementations ====

template<class T>
GA::WIQueue<T>::WIQueue() : mHead(0U), mTail(0U), mHeadDone(0U), mTailDone(0U) {
    for(unsigned long i = 0; i < LENGTH; ++i)
        mData[i] = std::pair(SLOTSTATE::FREE , nullptr);
}

template<class T>
void GA::WIQueue<T>::Push(const std::shared_ptr<T> & value) {
    unsigned int target = mHead.fetch_add(1, std::memory_order_acq_rel);

    unsigned int mHeadDone.
}

template<class T>
std::shared_ptr<T> GA::WIQueue<T>::Pop() {

}