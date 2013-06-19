/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_WORLDLOG_H
#define BLIZZLIKE_WORLDLOG_H

#include "Common.h"
#include "Policies/Singleton.h"
#include "Errors.h"

#include <stdarg.h>

// Log packets to a file
class WorldLog : public BlizzLike::Singleton<WorldLog, BlizzLike::ClassLevelLockable<WorldLog, ACE_Thread_Mutex> >
{
    friend class BlizzLike::OperatorNew<WorldLog>;
    WorldLog();
    WorldLog(const WorldLog &);
    WorldLog& operator=(const WorldLog &);
    typedef BlizzLike::ClassLevelLockable<WorldLog, ACE_Thread_Mutex>::Lock Guard;

    // Close the file in destructor
    ~WorldLog();

    public:
        void Initialize();
        // Is the world logger active?
        bool LogWorld(void) const { return (i_file != NULL); }
        // Log to the file
        void outLog(char const *fmt, ...);
        void outTimestampLog(char const *fmt, ...);

    private:
        FILE *i_file;

        bool m_dbWorld;
};

#define sWorldLog WorldLog::Instance()
#endif

