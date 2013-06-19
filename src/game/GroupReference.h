/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef _GROUPREFERENCE_H
#define _GROUPREFERENCE_H

#include "Utilities/LinkedReference/Reference.h"

class Group;
class Player;

class GroupReference : public Reference<Group, Player>
{
    protected:
        uint8 iSubGroup;
        void targetObjectBuildLink();
        void targetObjectDestroyLink();
        void sourceObjectDestroyLink();
    public:
        GroupReference() : Reference<Group, Player>(), iSubGroup(0) {}
        ~GroupReference() { unlink(); }
        GroupReference *next() { return (GroupReference*)Reference<Group, Player>::next(); }
        uint8 getSubGroup() const { return iSubGroup; }
        void setSubGroup(uint8 pSubGroup) { iSubGroup = pSubGroup; }
};
#endif

