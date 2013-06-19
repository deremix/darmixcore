/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Temporus
Complete(%): 75
Comment: More abilities need to be implemented
Category: Caverns of Time, The Dark Portal
EndScriptData */

#include "ScriptPCH.h"
#include "dark_portal.h"

#define SAY_ENTER               -1269000
#define SAY_AGGRO               -1269001
#define SAY_BANISH              -1269002
#define SAY_SLAY1               -1269003
#define SAY_SLAY2               -1269004
#define SAY_DEATH               -1269005

#define SPELL_HASTE             31458
#define SPELL_MORTAL_WOUND      31464
#define SPELL_WING_BUFFET       31475
#define H_SPELL_WING_BUFFET     38593
#define SPELL_REFLECT           38592                       //Not Implemented (Heroic mod)

struct boss_temporusAI : public ScriptedAI
{
    boss_temporusAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
        HeroicMode = me->GetMap()->IsHeroic();
    }

    ScriptedInstance *pInstance;
    bool HeroicMode;

    uint32 Haste_Timer;
    uint32 SpellReflection_Timer;

    void Reset()
    {
        Haste_Timer = 20000;
        SpellReflection_Timer = 40000;
    }

    void EnterCombat(Unit*)
    {
        DoScriptText(SAY_AGGRO, me);
    }

    void KilledUnit(Unit*)
    {
        switch (rand()%2)
        {
            case 0: DoScriptText(SAY_SLAY1, me); break;
            case 1: DoScriptText(SAY_SLAY2, me); break;
        }
    }

    void JustDied(Unit*)
    {
        DoScriptText(SAY_DEATH, me);

        if (pInstance)
            pInstance->SetData(TYPE_RIFT,SPECIAL);
    }

    void MoveInLineOfSight(Unit* who)
    {
        //Despawn Time Keeper
        if (who->GetTypeId() == TYPEID_UNIT && who->GetEntry() == C_TIME_KEEPER)
        {
            if (me->IsWithinDistInMap(who,20.0f))
            {
                DoScriptText(SAY_BANISH, me);

                me->DealDamage(who, who->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
        }

        ScriptedAI::MoveInLineOfSight(who);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Attack Haste
        if (Haste_Timer <= diff)
        {
            DoCast(me, SPELL_HASTE);
            Haste_Timer = 20000+rand()%5000;
        } else Haste_Timer -= diff;

        //Spell Reflection
        if (SpellReflection_Timer <= diff)
        {
            DoCast(me, SPELL_REFLECT);
            SpellReflection_Timer = 40000+rand()%10000;
        } else SpellReflection_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_temporus(Creature* pCreature)
{
    return new boss_temporusAI (pCreature);
}

void AddSC_boss_temporus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_temporus";
    newscript->GetAI = &GetAI_boss_temporus;
    newscript->RegisterSelf();
}

