//
// Created by amueller on 5/12/15.
//

#pragma once

#include <stddef.h>
#include <mutex>
#include <cmath>

namespace GA {
    namespace Datastructures {
        template<class T, int LevelCount>
        class SkipList<T, LevelCount> {
            class Node {
                size_t mID;
                Node *mNext[LevelCount];
                T mValue;
                std::mutex mMutex;
            public:
                Node(const size_t &id, const T &value) : mID(id), mValue(value), mMutex() {
                    for (auto i = 0U; i < LevelCount; ++i)
                        mNext[i] = nullptr;
                };

                Node(const Node &source) = delete;

                size_t GetID() { return mID; };

                void SetNext(size_t level, Node *target) { mNext[level] = target; };

                Node *GetNext(size_t level) { return mNext[level]; };

                T *GetValue() { return &mValue; };
            };

            Node *mFirst;
            size_t mSize;
            std::mutex mInsertMutex;
        public:
            SkipList();

            SkipList(const SkipList &source) = delete; //Disable copy constructor
            ~SkipList();

            size_t Insert(const T &value);

            T *Get(const size_t &id);

        private:
            Node *GetNode(const size_t &id);
        };

        template<class T, int LevelCount>
        SkipList<T, LevelCount>::SkipList()
                : mFirst(new Node(0, T())), mSize(1U), mInsertMutex() { }

        template<class T, int LevelCount>
        SkipList<T, LevelCount>::~SkipList() {
            Node *next = mFirst;
            while (next != nullptr) {
                Node *tmp = next->GetNext(0U);
                delete next;
                next = tmp;
            }
        }

        template<class T, int LevelCount>
        size_t SkipList<T, LevelCount>::Insert(const T &value) {
            std::unique_lock lock(mInsertMutex);
            auto id = mSize++;

            Node *newnode = new Node(id, value);
            int levelDist = 1;
            for (size_t i = 0; i < LevelCount; ++i) {
                int prevID = static_cast<int>(id) - levelDist;
                if (prevID < 0)
                    break;
                GetNode(prevID)->SetNext(i, newnode);
                levelDist *= 2;
            }

            return id;
        }

        template<class T, int LevelCount>
        T *SkipList<T, LevelCount>::Get(const size_t &id) {
            if (id == 0U)
                throw std::runtime_error("SkipList: Tried to access ID 0!");
            return GetNode(id)->GetValue();
        }

        template<class T, int LevelCount>
        SkipList<T, LevelCount>::Node *SkipList<T, LevelCount>::GetNode(const size_t &id) {
            Node *current = mFirst;
            size_t currentID = 0U;
            while (currentID != id) {
                size_t nextJump = 0U;
                size_t nextJumpLength = 1U;
                while (nextJump < LevelCount && currentID + nextJumpLength < id) {
                    nextJump += 1U;
                    nextJumpLength *= 2U;
                }

                current = current->GetNext(nextJump);
                if (current == nullptr) //This should never happen (TM)
                    throw std::logic_error("SkipList: Tried to jump across a nullptr");
                currentID = current->GetID();
            }
            return current;
        }
    }
}
