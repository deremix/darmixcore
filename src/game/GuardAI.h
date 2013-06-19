/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_GUARDAI_H
#define BLIZZLIKE_GUARDAI_H

#include "CreatureAI.h"
#include "Timer.h"

class Creature;

class GuardAI : public CreatureAI
{
    enum GuardState
    {
        STATE_NORMAL = 1,
        STATE_LOOK_AT_VICTIM = 2
    };

    public:

        explicit GuardAI(Creature* c);

        void MoveInLineOfSight(Unit*);
        void EnterEvadeMode();
        void JustDied(Unit*);
        bool IsVisible(Unit*) const;

        void UpdateAI(const uint32);
        static int Permissible(const Creature* );

    private:
        uint64 i_victimGuid;
        GuardState i_state;
        TimeTracker i_tracker;
};
#endif

