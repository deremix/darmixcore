/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Generic_Creature
Complete(%): 80
Comment: Should be replaced with core based AI
Category: Creatures
EndScriptData */

#include "ScriptPCH.h"

#define GENERIC_CREATURE_COOLDOWN   5000

struct generic_creatureAI : public ScriptedAI
{
    generic_creatureAI(Creature* c) : ScriptedAI(c) {}

    uint32 GlobalCooldown;      //This variable acts like the global cooldown that players have (1.5 seconds)
    uint32 BuffTimer;           //This variable keeps track of buffs
    bool IsSelfRooted;

    void Reset()
    {
        GlobalCooldown = 0;
        BuffTimer = 0;          //Rebuff as soon as we can
        IsSelfRooted = false;
    }

    void EnterCombat(Unit* who)
    {
        if (!me->IsWithinMeleeRange(who))
        {
            IsSelfRooted = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Always decrease our global cooldown first
        if (GlobalCooldown > diff)
            GlobalCooldown -= diff;
        else GlobalCooldown = 0;

        //Buff timer (only buff when we are alive and not in combat
        if (!me->isInCombat() && me->isAlive())
        {
            if (BuffTimer <= diff)
            {
                //Find a spell that targets friendly and applies an aura (these are generally buffs)
                SpellEntry const *info = SelectSpell(me, 0, 0, SELECT_TARGET_ANY_FRIEND, 0, 0, 0, 0, SELECT_EFFECT_AURA);

                if (info && !GlobalCooldown)
                {
                    //Cast the buff spell
                    DoCastSpell(me, info);

                    //Set our global cooldown
                    GlobalCooldown = GENERIC_CREATURE_COOLDOWN;

                    //Set our timer to 10 minutes before rebuff
                    BuffTimer = 600000;
                }//Try agian in 30 seconds
                else BuffTimer = 30000;
            } else BuffTimer -= diff;
        }
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //If we are within range melee the target
        if (me->IsWithinMeleeRange(me->getVictim()))
        {
            //Make sure our attack is ready and we arn't currently casting
            if (me->isAttackReady() && !me->IsNonMeleeSpellCasted(false))
            {
                bool Healing = false;
                SpellEntry const *info = NULL;

                //Select a healing spell if less than 30% hp
                if (me->GetHealth()*100 / me->GetMaxHealth() < 30)
                    info = SelectSpell(me, 0, 0, SELECT_TARGET_ANY_FRIEND, 0, 0, 0, 0, SELECT_EFFECT_HEALING);

                //No healing spell available, select a hostile spell
                if (info) Healing = true;
                else info = SelectSpell(me->getVictim(), 0, 0, SELECT_TARGET_ANY_ENEMY, 0, 0, 0, 0, SELECT_EFFECT_DONTCARE);

                //50% chance if elite or higher, 20% chance if not, to replace our white hit with a spell
                if (info && (rand() % (me->GetCreatureInfo()->rank > 1 ? 2 : 5) == 0) && !GlobalCooldown)
                {
                    //Cast the spell
                    if (Healing)DoCastSpell(me, info);
                    else DoCastSpell(me->getVictim(), info);

                    //Set our global cooldown
                    GlobalCooldown = GENERIC_CREATURE_COOLDOWN;
                }
                else me->AttackerStateUpdate(me->getVictim());

                me->resetAttackTimer();
            }
        }
        else
        {
            //Only run this code if we arn't already casting
            if (!me->IsNonMeleeSpellCasted(false))
            {
                bool Healing = false;
                SpellEntry const *info = NULL;

                //Select a healing spell if less than 30% hp ONLY 33% of the time
                if (me->GetHealth()*100 / me->GetMaxHealth() < 30 && rand() % 3 == 0)
                    info = SelectSpell(me, 0, 0, SELECT_TARGET_ANY_FRIEND, 0, 0, 0, 0, SELECT_EFFECT_HEALING);

                //No healing spell available, See if we can cast a ranged spell (Range must be greater than ATTACK_DISTANCE)
                if (info) Healing = true;
                else info = SelectSpell(me->getVictim(), 0, 0, SELECT_TARGET_ANY_ENEMY, 0, 0, NOMINAL_MELEE_RANGE, 0, SELECT_EFFECT_DONTCARE);

                //Found a spell, check if we arn't on cooldown
                if (info && !GlobalCooldown)
                {
                    //If we are currently moving stop us and set the movement generator
                    if (!IsSelfRooted)
                    {
                        IsSelfRooted = true;
                    }

                    //Cast spell
                    if (Healing) DoCastSpell(me,info);
                    else DoCastSpell(me->getVictim(),info);

                    //Set our global cooldown
                    GlobalCooldown = GENERIC_CREATURE_COOLDOWN;

                }//If no spells available and we arn't moving run to target
                else if (IsSelfRooted)
                {
                    //Cancel our current spell and then allow movement agian
                    me->InterruptNonMeleeSpells(false);
                    IsSelfRooted = false;
                }
            }
        }
    }
};

CreatureAI* GetAI_generic_creature(Creature* pCreature)
{
    return new generic_creatureAI (pCreature);
}


void AddSC_generic_creature()
{
    /*
    [Disabled to prevent "CRASH ALERT!"]
    Script *newscript;
    newscript = new Script;
    newscript->Name = "generic_creature";
    newscript->GetAI = &GetAI_generic_creature;
    newscript->RegisterSelf();
    */
}

