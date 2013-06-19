/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef LOCKEDQUEUE_H
#define LOCKEDQUEUE_H

#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <deque>
#include <assert.h>
#include "Errors.h"

namespace ACE_Based
{
    template <class T, class LockType, typename StorageType=std::deque<T> >
        class LockedQueue
    {
        // Lock access to the queue.
        LockType _lock;

        // Storage backing the queue.
        StorageType _queue;

        // Cancellation flag.
        volatile bool _canceled;

        public:

            // Create a LockedQueue.
            LockedQueue()
                : _canceled(false)
            {
            }

            // Destroy a LockedQueue.
            virtual ~LockedQueue()
            {
            }

            // Adds an item to the queue.
            void add(const T& item)
            {
                lock();

                //ASSERT(!this->_canceled);
                //throw Cancellation_Exception();

                _queue.push_back(item);

                unlock();
            }

            // Gets the next result in the queue, if any.
            bool next(T& result)
            {
               // ACE_Guard<LockType> g(this->_lock);
                ACE_GUARD_RETURN (LockType, g, this->_lock, false);

                if (_queue.empty())
                    return false;

                //ASSERT (!_queue.empty() || !this->_canceled);
                //throw Cancellation_Exception();
                result = _queue.front();
                _queue.pop_front();

                return true;
            }

            // Peeks at the top of the queue. Remember to unlock after use.
            T& peek()
            {
                lock();

                T& result = _queue.front();

                return result;
            }

            // Cancels the queue.
            void cancel()
            {
                lock();

                _canceled = true;

                unlock();
            }

            // Checks if the queue is cancelled.
            bool cancelled()
            {
                ACE_Guard<LockType> g(this->_lock);
                return _canceled;
            }

            // Locks the queue for access.
            void lock()
            {
                this->_lock.acquire();
            }

            // Unlocks the queue.
            void unlock()
            {
                this->_lock.release();
            }

            // Calls pop_front of the queue
            void pop_front()
            {
                ACE_GUARD (LockType, g, this->_lock);
                _queue.pop_front();
            }

            // Checks if we're empty or not with locks held
            bool empty()
            {
                ACE_GUARD_RETURN (LockType, g, this->_lock, false);
                return _queue.empty();
            }
    };
}
#endif

