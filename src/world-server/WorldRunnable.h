/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef __WORLDRUNNABLE_H
#define __WORLDRUNNABLE_H

// Heartbeat thread for the World
class WorldRunnable : public ACE_Based::Runnable
{
    public:
        void run();
};
#endif

