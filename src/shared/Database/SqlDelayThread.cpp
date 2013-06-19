/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "Database/SqlDelayThread.h"
#include "Database/SqlOperations.h"
#include "DatabaseEnv.h"

SqlDelayThread::SqlDelayThread(Database* db) : m_dbEngine(db), m_running(true)
{
}

void SqlDelayThread::run()
{
    mysql_thread_init();

    SqlAsyncTask * s = NULL;

    ACE_Time_Value _time(2);
    while (m_running)
    {
        // if the running state gets turned off while sleeping
        // empty the queue before exiting
        s = dynamic_cast<SqlAsyncTask*> (m_sqlQueue.dequeue());
        if (s)
        {
            s->call();
            delete s;
        }
    }

    mysql_thread_end();
}

void SqlDelayThread::Stop()
{
    m_running = false;
    m_sqlQueue.queue()->deactivate();
}

bool SqlDelayThread::Delay(SqlOperation* sql)
{
    int res = m_sqlQueue.enqueue(new SqlAsyncTask(m_dbEngine, sql));
    return (res != -1);
}

