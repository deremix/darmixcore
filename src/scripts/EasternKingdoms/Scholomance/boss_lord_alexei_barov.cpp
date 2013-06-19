/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Lord_Alexei_Barov
Complete(%): 100
Comment: aura applied/defined in database
Category: Scholomance
EndScriptData */

#include "ScriptPCH.h"
#include "scholomance.h"

#define SPELL_IMMOLATE             20294                    // Old ID  was 15570
#define SPELL_VEILOFSHADOW         17820

struct boss_lordalexeibarovAI : public ScriptedAI
{
    boss_lordalexeibarovAI(Creature* c) : ScriptedAI(c) {}

    uint32 Immolate_Timer;
    uint32 VeilofShadow_Timer;

    void Reset()
    {
        Immolate_Timer = 7000;
        VeilofShadow_Timer = 15000;

        me->LoadCreaturesAddon();
    }

    void JustDied(Unit* /*killer*/)
    {
        ScriptedInstance *pInstance = me->GetInstanceData();
        if (pInstance)
        {
            pInstance->SetData(DATA_LORDALEXEIBAROV_DEATH, 0);

            if (pInstance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Immolate_Timer
        if (Immolate_Timer <= diff)
        {
            Unit* pTarget = NULL;
            pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (pTarget) DoCast(pTarget, SPELL_IMMOLATE);

            Immolate_Timer = 12000;
        } else Immolate_Timer -= diff;

        //VeilofShadow_Timer
        if (VeilofShadow_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_VEILOFSHADOW);
            VeilofShadow_Timer = 20000;
        } else VeilofShadow_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_lordalexeibarov(Creature* pCreature)
{
    return new boss_lordalexeibarovAI (pCreature);
}

void AddSC_boss_lordalexeibarov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lord_alexei_barov";
    newscript->GetAI = &GetAI_boss_lordalexeibarov;
    newscript->RegisterSelf();
}

