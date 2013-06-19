/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef __ADDONHANDLER_H
#define __ADDONHANDLER_H

#include "Common.h"
#include "Config/Config.h"
#include "Policies/Singleton.h"

#include "WorldPacket.h"

class AddonHandler
{
    public:
        /* Construction */
        AddonHandler();
        ~AddonHandler();
                                                            //built addon packet
        bool BuildAddonPacket(WorldPacket* Source, WorldPacket* Target);
};
#define sAddOnHandler BlizzLike::Singleton<AddonHandler>::Instance()
#endif

