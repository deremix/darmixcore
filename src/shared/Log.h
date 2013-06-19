/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKECORE_LOG_H
#define BLIZZLIKECORE_LOG_H

#include "Common.h"
#include "Policies/Singleton.h"
#include "Database/DatabaseEnv.h"

class Config;

enum LogFilters
{
    LOG_FILTER_TRANSPORT_MOVES     = 1,
    LOG_FILTER_CREATURE_MOVES      = 2,
    LOG_FILTER_VISIBILITY_CHANGES  = 4,
};

enum LogTypes
{
    LOG_TYPE_STRING = 0,
    LOG_TYPE_ERROR  = 1,
    LOG_TYPE_BASIC  = 2,
    LOG_TYPE_DETAIL = 3,
    LOG_TYPE_DEBUG  = 4,
    LOG_TYPE_CHAR   = 5,
    LOG_TYPE_WORLD  = 6,
    LOG_TYPE_RA     = 7,
    LOG_TYPE_GM     = 8,
    LOG_TYPE_CRASH  = 9,
    LOG_TYPE_CHAT   = 10,
    MAX_LOG_TYPES
};

enum LogLevel
{
    LOGL_NORMAL = 0,
    LOGL_BASIC,
    LOGL_DETAIL,
    LOGL_DEBUG
};

const int LogLevels = int(LOGL_DEBUG)+1;

enum ColorTypes
{
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    MAGENTA,
    CYAN,
    GREY,
    YELLOW,
    LRED,
    LGREEN,
    LBLUE,
    LMAGENTA,
    LCYAN,
    WHITE
};

const int Colors = int(WHITE)+1;

class Log : public BlizzLike::Singleton<Log, BlizzLike::ClassLevelLockable<Log, ACE_Thread_Mutex> >
{
    friend class BlizzLike::OperatorNew<Log>;
    Log();
    ~Log();

    public:
        void Initialize();

        void InitColors(const std::string& init_str);
        void SetColor(bool stdout_stream, ColorTypes color);
        void ResetColor(bool stdout_stream);

        void outDB( LogTypes type, const char * str );
        void outString( const char * str, ... )                 ATTR_PRINTF(2,3);
        void outString( );
        void outStringInLine( const char * str, ... )           ATTR_PRINTF(2,3);
        void outError( const char * err, ... )                  ATTR_PRINTF(2,3);
        void outCrash( const char * err, ... )                  ATTR_PRINTF(2,3);
        void outBasic( const char * str, ... )                  ATTR_PRINTF(2,3);
        void outDetail( const char * str, ... )                 ATTR_PRINTF(2,3);
        void outDebug( const char * str, ... )                  ATTR_PRINTF(2,3);
        void outStaticDebug( const char * str, ... )            ATTR_PRINTF(2,3);
        void outDebugInLine( const char * str, ... )            ATTR_PRINTF(2,3);
        void outErrorDb( const char * str, ... )                ATTR_PRINTF(2,3);
        void outChar( const char * str, ... )                   ATTR_PRINTF(2,3);
        void outCommand( uint32 account, const char * str, ...) ATTR_PRINTF(3,4);
        void outRemote( const char * str, ... )                 ATTR_PRINTF(2,3);
        void outChat( const char * str, ... )                   ATTR_PRINTF(2,3);
        void outWarden( const char * str, ... )                 ATTR_PRINTF(2,3);
        void outArena( const char * str, ... )                  ATTR_PRINTF(2,3);
        void outCharDump( const char * str, uint32 account_id, uint32 guid, const char * name );

        static void outTimestamp(FILE* file);
        static std::string GetTimestampStr();

        void SetLogLevel(char * Level);
        void SetLogFileLevel(char * Level);
        void SetDBLogLevel(char * Level);
        void SetRealmID(uint32 id) { realm = id; }

        uint32 getLogFilter() const { return m_logFilter; }
        bool IsOutDebug() const { return m_logLevel > 2 || (m_logFileLevel > 2 && logfile); }
        bool IsOutCharDump() const { return m_charLog_Dump; }

        bool GetLogDB() { return m_enableLogDB; }
        bool GetLogDBLater() { return m_enableLogDBLater; }
        void SetLogDB(bool enable) { m_enableLogDB = enable; }
        void SetLogDBLater(bool value) { m_enableLogDBLater = value; }
    private:
        FILE* openLogFile(char const* configFileName,char const* configTimeStampFlag, char const* mode);
        FILE* openGmlogPerAccount(uint32 account);

        FILE* raLogfile;
        FILE* logfile;
        FILE* gmLogfile;
        FILE* charLogfile;
        FILE* dberLogfile;
        FILE* chatLogfile;
        FILE* arenaLogFile;
        FILE* wardenLogFile;

        // cache values for after initilization use (like gm log per account case)
        std::string m_logsDir;
        std::string m_logsTimestamp;

        // gm log control
        bool m_gmlog_per_account;
        std::string m_gmlog_filename_format;

        bool m_enableLogDBLater;
        bool m_enableLogDB;
        uint32 realm;

        // log coloring
        bool m_colored;
        ColorTypes m_colors[4];

        // log levels:
        // 0 minimum/string, 1 basic/error, 2 detail, 3 full/debug
        uint8 m_dbLogLevel;
        uint8 m_logLevel;
        uint8 m_logFileLevel;
        uint8 m_logFilter;
        bool m_dbChar;
        bool m_dbRA;
        bool m_dbGM;
        bool m_dbChat;
        bool m_charLog_Dump;
};

#define sLog BlizzLike::Singleton<Log>::Instance()

#ifdef BLIZZLIKE_DEBUG
#define DEBUG_LOG BlizzLike::Singleton<Log>::Instance().outDebug
#else
#define DEBUG_LOG
#endif

// primary for script library
#define outstring_log BlizzLike::Singleton<Log>::Instance().outString
#define detail_log BlizzLike::Singleton<Log>::Instance().outDetail
#define debug_log BlizzLike::Singleton<Log>::Instance().outDebug
#define error_log BlizzLike::Singleton<Log>::Instance().outError
#define error_db_log BlizzLike::Singleton<Log>::Instance().outErrorDb
#endif
