/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "Unit.h"
#include "TargetedMovementGenerator.h"
#include "FollowerReference.h"

void FollowerReference::targetObjectBuildLink()
{
    getTarget()->addFollower(this);
}

void FollowerReference::targetObjectDestroyLink()
{
    getTarget()->removeFollower(this);
}

void FollowerReference::sourceObjectDestroyLink()
{
    getSource()->stopFollowing();
}

