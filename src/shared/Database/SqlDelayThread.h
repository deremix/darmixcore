/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef __SQLDELAYTHREAD_H
#define __SQLDELAYTHREAD_H

#include "ace/Thread_Mutex.h"
#include "ace/Activation_Queue.h"
#include "Threading.h"

class Database;
class SqlOperation;

class SqlDelayThread : public ACE_Based::Runnable
{
    typedef ACE_Activation_Queue SqlQueue;

    private:
        SqlQueue m_sqlQueue;                                // Queue of SQL statements
        Database* m_dbEngine;                               // Pointer to used Database engine
        volatile bool m_running;

        SqlDelayThread();
    public:
        SqlDelayThread(Database* db);

        // Put sql statement to delay queue
        bool Delay(SqlOperation* sql);

        void Stop();                                // Stop event
        virtual void run();                                 // Main Thread loop
};
#endif                                                      //__SQLDELAYTHREAD_H

