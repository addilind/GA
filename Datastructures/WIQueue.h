//
// Created by amueller on 5/12/15.
//

#pragma once

#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace GA {
    namespace Datastructures {
        //WorkItem queue
        template<class T>
        class WIQueue {
            struct slot {
                enum STATE : unsigned char {
                    FREE = 0U, WRITING = 1U, READY = 2U, READING = 3U
                };

                STATE state;
                std::mutex mutex;
                std::condition_variable conVar;
                std::shared_ptr<T> value;

                slot() : state(FREE), mutex(), conVar(), value(nullptr) { }

                bool IsFree() { return state == STATE::FREE; }

                bool IsReady() { return state == STATE::READY; }
            };

            static const unsigned int LENGTH = 512U;
            static const unsigned int MASK = LENGTH - 1U;

            slot mData[LENGTH];
            std::atomic<unsigned int> mHead;
            std::atomic<unsigned int> mTail;
        public:
            WIQueue();

            WIQueue(const WIQueue &source) = delete; //Disable copy constructor

            void Push(const std::shared_ptr<T> &value);

            std::shared_ptr<T> Pop();

        };

        // ==== Implementations ====

        template<class T>
        WIQueue<T>::WIQueue() : mData(), mHead(0U), mTail(0U) {
        }

        template<class T>
        void WIQueue<T>::Push(const std::shared_ptr<T> &value) {
            unsigned int target = mHead.fetch_add(1, std::memory_order_acq_rel) % LENGTH;
            mHead.fetch_and(MASK); // => if(mHead == LENGTH) mHead = 0;

            if (mData[target].state != slot::STATE::FREE) { //slot is not ready yet
                std::unique_lock<std::mutex> lock(mData[target].mutex);
                mData[target].conVar.wait(lock, std::bind(&slot::IsFree, &mData[target]));
            }

            mData[target].state = slot::STATE::WRITING;

            mData[target].value = value;

            mData[target].state = slot::STATE::READY;
            mData[target].conVar.notify_one();
        }

        template<class T>
        std::shared_ptr<T> WIQueue<T>::Pop() {
            unsigned int target = mTail.fetch_add(1, std::memory_order_acq_rel) % LENGTH;
            mTail.fetch_and(MASK); // => if(mTail == LENGTH) mTail = 0;

            if (mData[target].state != slot::STATE::READY) { //slot is not ready yet
                std::unique_lock<std::mutex> lock(mData[target].mutex);
                mData[target].conVar.wait(lock, std::bind(&slot::IsReady, &mData[target]));
            }

            mData[target].state = slot::STATE::READING;

            auto value = mData[target].value;

            mData[target].state = slot::STATE::FREE;
            mData[target].conVar.notify_one();

            return value;
        }
    }
}