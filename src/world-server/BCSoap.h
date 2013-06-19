/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef _blizzlikeSOAP_H
#define _blizzlikeSOAP_H

#include "Common.h"
#include "World.h"
#include "AccountMgr.h"
#include "Log.h"

#include "soapH.h"
#include "soapStub.h"

#include <ace/Semaphore.h>
#include <ace/Task.h>


class BCSoapRunnable: public ACE_Based::Runnable
{
    public:
        BCSoapRunnable() { }
        void run();
        void setListenArguments(std::string host, uint16 port)
        {
            m_host = host;
            m_port = port;
        }
    private:
        std::string m_host;
        uint16 m_port;
};

class SOAPWorkingThread : public ACE_Task<ACE_MT_SYNCH>
{
    public:
        SOAPWorkingThread ()
        { }

        virtual int svc (void)
        {
            while (1)
            {
                ACE_Message_Block *mb = 0;
                if (this->getq (mb) == -1)
                {
                    ACE_DEBUG ((LM_INFO,
                                ACE_TEXT ("(%t) Shutting down\n")));
                    break;
                }

                // Process the message.
                process_message (mb);
            }

            return 0;
        }
    private:
        void process_message (ACE_Message_Block *mb);
};


class SOAPCommand
{
    public:
        SOAPCommand():
            pendingCommands(0, USYNC_THREAD, "pendingCommands")
        {

        }
        ~SOAPCommand()
        {
        }

        void appendToPrintBuffer(const char* msg)
        {
            m_printBuffer += msg;
        }

        ACE_Semaphore pendingCommands;

        void setCommandSuccess(bool val)
        {
            m_success = val;
        }
        bool hasCommandSucceeded()
        {
            return m_success;
        }

        static void print(void* callbackArg, const char* msg)
        {
            ((SOAPCommand*)callbackArg)->appendToPrintBuffer(msg);
        }

        static void commandFinished(void* callbackArg, bool success);

        bool m_success;
        std::string m_printBuffer;
};

#endif

