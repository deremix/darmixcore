/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Anubshiah
Complete(%): 100
Comment:
Category: Blackrock Depths
EndScriptData */

#include "ScriptPCH.h"

enum Spells
{
    SPELL_SHADOWBOLT                                       = 17228,
    SPELL_CURSEOFTONGUES                                   = 15470,
    SPELL_CURSEOFWEAKNESS                                  = 17227,
    SPELL_DEMONARMOR                                       = 11735,
    SPELL_ENVELOPINGWEB                                    = 15471
};

struct boss_anubshiahAI : public ScriptedAI
{
    boss_anubshiahAI(Creature* c) : ScriptedAI(c) {}

    uint32 ShadowBolt_Timer;
    uint32 CurseOfTongues_Timer;
    uint32 CurseOfWeakness_Timer;
    uint32 DemonArmor_Timer;
    uint32 EnvelopingWeb_Timer;

    void Reset()
    {
        ShadowBolt_Timer = 7000;
        CurseOfTongues_Timer = 24000;
        CurseOfWeakness_Timer = 12000;
        DemonArmor_Timer = 3000;
        EnvelopingWeb_Timer = 16000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //ShadowBolt_Timer
        if (ShadowBolt_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SHADOWBOLT);
            ShadowBolt_Timer = 7000;
        } else ShadowBolt_Timer -= diff;

        //CurseOfTongues_Timer
        if (CurseOfTongues_Timer <= diff)
        {
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                DoCast(pTarget, SPELL_CURSEOFTONGUES);
            CurseOfTongues_Timer = 18000;
        } else CurseOfTongues_Timer -= diff;

        //CurseOfWeakness_Timer
        if (CurseOfWeakness_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CURSEOFWEAKNESS);
            CurseOfWeakness_Timer = 45000;
        } else CurseOfWeakness_Timer -= diff;

        //DemonArmor_Timer
        if (DemonArmor_Timer <= diff)
        {
            DoCast(me, SPELL_DEMONARMOR);
            DemonArmor_Timer = 300000;
        } else DemonArmor_Timer -= diff;

        //EnvelopingWeb_Timer
        if (EnvelopingWeb_Timer <= diff)
        {
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                DoCast(pTarget, SPELL_ENVELOPINGWEB);
            EnvelopingWeb_Timer = 12000;
        } else EnvelopingWeb_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_anubshiah(Creature* pCreature)
{
    return new boss_anubshiahAI (pCreature);
}

void AddSC_boss_anubshiah()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_anubshiah";
    newscript->GetAI = &GetAI_boss_anubshiah;
    newscript->RegisterSelf();
}
