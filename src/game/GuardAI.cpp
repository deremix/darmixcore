/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "GuardAI.h"
#include "Errors.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "World.h"
#include "CreatureAIImpl.h"

int GuardAI::Permissible(const Creature* creature)
{
    if (creature->isGuard())
        return PERMIT_BASE_SPECIAL;

    return PERMIT_BASE_NO;
}

GuardAI::GuardAI(Creature* c) : CreatureAI(c), i_victimGuid(0), i_state(STATE_NORMAL), i_tracker(TIME_INTERVAL_LOOK)
{
}

void GuardAI::MoveInLineOfSight(Unit* u)
{
    // Ignore Z for flying creatures
    if (!me->canFly() && me->GetDistanceZ(u) > CREATURE_Z_ATTACK_RANGE)
        return;

    if (!me->getVictim() && me->canAttack(u) &&
        (u->IsHostileToPlayers() || me->IsHostileTo(u) /*|| u->getVictim() && me->IsFriendlyTo(u->getVictim())*/) &&
        u->isInAccessiblePlaceFor(me))
    {
        float attackRadius = me->GetAttackDistance(u);
        if (me->IsWithinDistInMap(u,attackRadius))
        {
            //Need add code to let guard support player
            AttackStart(u);
            //u->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
        }
    }
}

void GuardAI::EnterEvadeMode()
{
    if (!me->isAlive())
    {
        DEBUG_LOG("Creature stopped attacking because he's dead [guid=%u]", me->GetGUIDLow());
        me->GetMotionMaster()->MoveIdle();

        i_state = STATE_NORMAL;

        i_victimGuid = 0;
        me->CombatStop(true);
        me->DeleteThreatList();
        return;
    }

    Unit* victim = ObjectAccessor::GetUnit(*me, i_victimGuid);

    if (!victim)
    {
        DEBUG_LOG("Creature stopped attacking because victim is non exist [guid=%u]", me->GetGUIDLow());
    }
    else if (!victim ->isAlive())
    {
        DEBUG_LOG("Creature stopped attacking because victim is dead [guid=%u]", me->GetGUIDLow());
    }
    else if (victim ->HasStealthAura())
    {
        DEBUG_LOG("Creature stopped attacking because victim is using stealth [guid=%u]", me->GetGUIDLow());
    }
    else if (victim ->isInFlight())
    {
        DEBUG_LOG("Creature stopped attacking because victim is flying away [guid=%u]", me->GetGUIDLow());
    }
    else
    {
        DEBUG_LOG("Creature stopped attacking because victim outran him [guid=%u]", me->GetGUIDLow());
    }

    me->RemoveAllAuras();
    me->DeleteThreatList();
    i_victimGuid = 0;
    me->CombatStop(true);
    i_state = STATE_NORMAL;

    // Remove TargetedMovementGenerator from MotionMaster stack list, and add HomeMovementGenerator instead
    if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
        me->GetMotionMaster()->MoveTargetedHome();
}

void GuardAI::UpdateAI(const uint32 /*diff*/)
{
    // update i_victimGuid if me->getVictim() != 0 and changed
    if (!UpdateVictim())
        return;

    i_victimGuid = me->getVictim()->GetGUID();

    if (me->isAttackReady())
    {
        if (me->IsWithinMeleeRange(me->getVictim()))
        {
            me->AttackerStateUpdate(me->getVictim());
            me->resetAttackTimer();
        }
    }
}

bool GuardAI::IsVisible(Unit* pl) const
{
    return me->IsWithinDistInMap(pl,sWorld.getConfig(CONFIG_SIGHT_GUARDER))
        && pl->isVisibleForOrDetect(me,true);
}

void GuardAI::JustDied(Unit* killer)
{
    if (Player* pkiller = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
        me->SendZoneUnderAttackMessage(pkiller);
}

