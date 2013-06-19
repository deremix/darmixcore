/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef __SQLOPERATIONS_H
#define __SQLOPERATIONS_H

#include "Common.h"

#include "ace/Thread_Mutex.h"
#include "ace/Method_Request.h"
#include "LockedQueue.h"
#include <queue>
#include "Utilities/Callback.h"
#include "QueryResult.h"

// BASE

class Database;
class SqlDelayThread;

class SqlOperation
{
    public:
        virtual void OnRemove() { delete this; }
        virtual void Execute(Database *db) = 0;
        virtual ~SqlOperation() {}
};

// ASYNC STATEMENTS / TRANSACTIONS

class SqlStatement : public SqlOperation
{
    private:
        const char *m_sql;
    public:
        SqlStatement(const char *sql) : m_sql(strdup(sql)){}
        ~SqlStatement() { void* tofree = const_cast<char*>(m_sql); free(tofree); }
        void Execute(Database *db);
};

class SqlTransaction : public SqlOperation
{
    private:
        std::queue<const char*> m_queue;
        ACE_Thread_Mutex m_Mutex;
    public:
        SqlTransaction() {}
        void DelayExecute(const char *sql)
        {
            m_Mutex.acquire();
            char* _sql = strdup(sql);
            if (_sql)
                m_queue.push(_sql);
            m_Mutex.release();
        }
        void Execute(Database *db);
};

// ASYNC QUERIES

class SqlQuery;                                             // contains a single async query
class QueryResult;                                          // the result of one
class SqlResultQueue;                                       // queue for thread sync
class SqlQueryHolder;                                       // groups several async quries
class SqlQueryHolderEx;                                     // points to a holder, added to the delay thread

class SqlResultQueue : public ACE_Based::LockedQueue<BlizzLike::IQueryCallback*, ACE_Thread_Mutex>
{
    public:
        SqlResultQueue() {}
        void Update();
};

class SqlQuery : public SqlOperation
{
    private:
        const char *m_sql;
        BlizzLike::IQueryCallback * m_callback;
        SqlResultQueue * m_queue;
    public:
        SqlQuery(const char *sql, BlizzLike::IQueryCallback * callback, SqlResultQueue * queue)
            : m_sql(strdup(sql)), m_callback(callback), m_queue(queue) {}
        ~SqlQuery() { void* tofree = const_cast<char*>(m_sql); free(tofree); }
        void Execute(Database *db);
};

class SqlQueryHolder
{
    friend class SqlQueryHolderEx;
    private:
        typedef std::pair<const char*, QueryResult_AutoPtr> SqlResultPair;
        std::vector<SqlResultPair> m_queries;
    public:
        SqlQueryHolder() {}
        ~SqlQueryHolder();
        bool SetQuery(size_t index, const char *sql);
        bool SetPQuery(size_t index, const char *format, ...) ATTR_PRINTF(3,4);
        void SetSize(size_t size);
        QueryResult_AutoPtr GetResult(size_t index);
        void SetResult(size_t index, QueryResult_AutoPtr result);
        bool Execute(BlizzLike::IQueryCallback * callback, SqlDelayThread *thread, SqlResultQueue *queue);
};

class SqlQueryHolderEx : public SqlOperation
{
    private:
        SqlQueryHolder * m_holder;
        BlizzLike::IQueryCallback * m_callback;
        SqlResultQueue * m_queue;
    public:
        SqlQueryHolderEx(SqlQueryHolder *holder, BlizzLike::IQueryCallback * callback, SqlResultQueue * queue)
            : m_holder(holder), m_callback(callback), m_queue(queue) {}
        void Execute(Database *db);
};

class SqlAsyncTask : public ACE_Method_Request
{
public:
    SqlAsyncTask(Database * db, SqlOperation * op) : m_db(db), m_op(op){}
    ~SqlAsyncTask()
    {
        if (!m_op)
            return;

        delete m_op;
        m_op = NULL;
    }

    int call()
    {
        if (m_db == NULL || m_op == NULL)
        return -1;

        try
        {
            m_op->Execute(m_db);
        }
        catch(...)
        {
            return -1;
        }

        return 0;
    }

private:
    Database * m_db;
    SqlOperation * m_op;
};
#endif                                                      //__SQLOPERATIONS_H

