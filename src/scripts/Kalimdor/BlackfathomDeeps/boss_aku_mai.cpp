/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "ScriptPCH.h"
#include "blackfathom_deeps.h"

enum Spells
{
    SPELL_POISON_CLOUD                                     = 3815,
    SPELL_FRENZIED_RAGE                                    = 3490
};

struct boss_aku_maiAI : public ScriptedAI
{
    boss_aku_maiAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
    }

    uint32 uiPoisonCloudTimer;
    bool bIsEnraged;

    ScriptedInstance *pInstance;

    void Reset()
    {
        uiPoisonCloudTimer = urand(5000,9000);
        bIsEnraged = false;
        if (pInstance)
            pInstance->SetData(TYPE_AKU_MAI, NOT_STARTED);
    }

    void EnterCombat(Unit* /*who*/)
    {
        if (pInstance)
            pInstance->SetData(TYPE_AKU_MAI, IN_PROGRESS);
    }

    void JustDied(Unit* /*killer*/)
    {
        if (pInstance)
            pInstance->SetData(TYPE_AKU_MAI, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (uiPoisonCloudTimer < diff)
        {
            DoCastVictim(SPELL_POISON_CLOUD);
            uiPoisonCloudTimer = urand(25000,50000);
        } else uiPoisonCloudTimer -= diff;

        if (!bIsEnraged && HealthBelowPct(30))
        {
            DoCast(me,SPELL_FRENZIED_RAGE);
            bIsEnraged = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_aku_mai(Creature* pCreature)
{
    return new boss_aku_maiAI (pCreature);
}

void AddSC_boss_aku_mai()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_aku_mai";
    newscript->GetAI = &GetAI_boss_aku_mai;
    newscript->RegisterSelf();
}
