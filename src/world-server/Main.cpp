/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "SystemConfig.h"

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Config/Config.h"

#include "Log.h"
#include "Master.h"
#include <ace/Version.h>
#include <ace/Get_Opt.h>

// Format is YYYYMMDD (change in the conf file)
#ifndef _DARMIX_WORLD_CONFVER
# define _DARMIX_WORLD_CONFVER 20130614
#endif //_DARMIX_WORLD_CONFVER

#ifndef _DARMIX_WORLD_CONFIG
# define _DARMIX_WORLD_CONFIG  "world-server.conf"
#endif //_DARMIX_WORLD_CONFIG

#ifdef _WIN32
#include "ServiceWin32.h"
char serviceName[] = "world-server";
char serviceLongName[] = "darmix-core world-server";
char serviceDescription[] = "world service";
/*
 * -1 - not in service mode
 *  0 - stopped
 *  1 - running
 *  2 - paused
 */
int m_ServiceStatus = -1;
#endif

#ifdef _WIN32
# define sleep(x) Sleep(x * 1000)
#endif

DatabaseType WorldDatabase;                                 ///< Accessor to the world database
DatabaseType CharacterDatabase;                             ///< Accessor to the character database
DatabaseType LoginDatabase;                                 ///< Accessor to the auth/login database

uint32 realmID;                                             ///< Id of the realm

// Print out the usage string for this program on the console.
void usage(const char *prog)
{
    sLog.outString("Usage: \n %s [<options>]\n"
        "    -v, --version            print version and exit\n\r"
        "    -c config_file           use config_file as configuration file\n\r"
        #ifdef _WIN32
        "    Running as service functions:\n\r"
        "    -s run                   run as service\n\r"
        "    -s install               install service\n\r"
        "    -s uninstall             uninstall service\n\r"
        #endif
        ,prog);
}

// Launch the world server
extern int main(int argc, char **argv)
{
    // Command line parsing
    char const* cfg_file = _DARMIX_WORLD_CONFIG;

#ifdef _WIN32
    char const *options = ":c:s:";
#else
    char const *options = ":c:";
#endif

    ACE_Get_Opt cmd_opts(argc, argv, options);
    cmd_opts.long_option("version", 'v');

    int option;
    while ((option = cmd_opts()) != EOF)
    {
        switch (option)
        {
            case 'c':
                cfg_file = cmd_opts.opt_arg();
                break;
            case 'v':
                printf("%s\n", _FULLVERSION);
                return 0;
#ifdef _WIN32
            case 's':
            {
                const char *mode = cmd_opts.opt_arg();

                if (!strcmp(mode, "install"))
                {
                    if (WinServiceInstall())
                        sLog.outString("Installing service");
                    return 1;
                }
                else if (!strcmp(mode, "uninstall"))
                {
                    if (WinServiceUninstall())
                        sLog.outString("Uninstalling service");
                    return 1;
                }
                else if (!strcmp(mode, "run"))
                    WinServiceRun();
                else
                {
                    sLog.outError("Runtime-Error: -%c unsupported argument %s", cmd_opts.opt_opt(), mode);
                    usage(argv[0]);
                    return 1;
                }
                break;
            }
#endif
            case ':':
                sLog.outError("Runtime-Error: -%c option requires an input argument", cmd_opts.opt_opt());
                usage(argv[0]);
                return 1;
            default:
                sLog.outError("Runtime-Error: bad format of commandline arguments");
                usage(argv[0]);
                return 1;
        }
    }

    if (!sConfig.SetSource(cfg_file))
    {
        sLog.outError("Invalid or missing configuration file : %s", cfg_file);
        sLog.outError("Verify that the file exists and has \'[world-server]' written in the top of the file!");
        return 1;
    }

    uint32 confVersion = sConfig.GetIntDefault("ConfVersion", 0);
    if (confVersion != _DARMIX_WORLD_CONFVER)
    {
        sLog.outError(" WARNING:");
        sLog.outError(" Your %s file is out of date.", cfg_file);
        sLog.outError(" Please, check for updates.");
        sleep(5);
    }

    // and run the 'Master'
    // todo - Why do we need this 'Master'? Can't all of this be in the Main as for auth?
    return sMaster.Run();

    // at sMaster return function exist with codes
    // 0 - normal shutdown
    // 1 - shutdown at error
    // 2 - restart command used, this code can be used by restarter for restart BlizzLikeCore
}

