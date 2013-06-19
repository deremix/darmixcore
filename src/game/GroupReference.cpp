/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "Player.h"
#include "Group.h"
#include "GroupReference.h"

void GroupReference::targetObjectBuildLink()
{
    // called from link()
    getTarget()->LinkMember(this);
}

void GroupReference::targetObjectDestroyLink()
{
    // called from unlink()
    getTarget()->DelinkMember(this);
}

void GroupReference::sourceObjectDestroyLink()
{
    // called from invalidate()
    getTarget()->DelinkMember(this);
}

