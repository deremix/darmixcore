/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Fankriss
Complete(%): 100
Comment: sound not implemented
Category: Temple of Ahn'Qiraj
EndScriptData */

#include "ScriptPCH.h"

#define SOUND_SENTENCE_YOU 8588
#define SOUND_SERVE_TO     8589
#define SOUND_LAWS         8590
#define SOUND_TRESPASS     8591
#define SOUND_WILL_BE      8592

#define SPELL_MORTAL_WOUND 28467
#define SPELL_ROOT         28858

// Enrage for his spawns
#define SPELL_ENRAGE       28798

struct boss_fankrissAI : public ScriptedAI
{
    boss_fankrissAI(Creature* c) : ScriptedAI(c) {}

    uint32 MortalWound_Timer;
    uint32 SpawnHatchlings_Timer;
    uint32 SpawnSpawns_Timer;
    int Rand;
    int RandX;
    int RandY;

    Creature* Hatchling;
    Creature* Spawn;

    void Reset()
    {
        MortalWound_Timer = 10000 + rand()%5000;
        SpawnHatchlings_Timer = 6000 + rand()%6000;
        SpawnSpawns_Timer = 15000 + rand()%30000;
    }

    void SummonSpawn(Unit* victim)
    {
        if (!victim)
            return;

        Rand = 10 + (rand()%10);
        switch (rand()%2)
        {
            case 0: RandX = 0 - Rand; break;
            case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand =  10 + (rand()%10);
        switch (rand()%2)
        {
            case 0: RandY = 0 - Rand; break;
            case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Spawn = DoSpawnCreature(15630, float(RandX), float(RandY), 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
        if (Spawn)
            ((CreatureAI*)Spawn->AI())->AttackStart(victim);
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //MortalWound_Timer
        if (MortalWound_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_MORTAL_WOUND);
            MortalWound_Timer = 10000 + rand()%10000;
        } else MortalWound_Timer -= diff;

        //Summon 1-3 Spawns of Fankriss at random time.
        if (SpawnSpawns_Timer <= diff)
        {
            switch (rand()%3)
            {
                case 0:
                    SummonSpawn(SelectUnit(SELECT_TARGET_RANDOM,0));
                    break;
                case 1:
                    SummonSpawn(SelectUnit(SELECT_TARGET_RANDOM,0));
                    SummonSpawn(SelectUnit(SELECT_TARGET_RANDOM,0));
                    break;
                case 2:
                    SummonSpawn(SelectUnit(SELECT_TARGET_RANDOM,0));
                    SummonSpawn(SelectUnit(SELECT_TARGET_RANDOM,0));
                    SummonSpawn(SelectUnit(SELECT_TARGET_RANDOM,0));
                    break;
            }
            SpawnSpawns_Timer = 30000 + rand()%30000;
        } else SpawnSpawns_Timer -= diff;

        // Teleporting Random Target to one of the three tunnels and spawn 4 hatchlings near the gamer.
        //We will only telport if fankriss has more than 3% of hp so teleported gamers can always loot.
        if (me->GetHealth()*100 / me->GetMaxHealth() > 3)
        {
            if (SpawnHatchlings_Timer <= diff)
            {
                Unit* pTarget = NULL;
                pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                {
                    DoCast(pTarget, SPELL_ROOT);

                    if (DoGetThreat(pTarget))
                        DoModifyThreatPercent(pTarget, -100);

                    switch (rand()%3)
                    {
                        case 0:
                            DoTeleportPlayer(pTarget, -8106.0142f,1289.2900f,-74.419533f,5.112f);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-3, pTarget->GetPositionY()-3, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-3, pTarget->GetPositionY()+3, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-5, pTarget->GetPositionY()-5, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-5, pTarget->GetPositionY()+5, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            break;
                        case 1:
                            DoTeleportPlayer(pTarget, -7990.135354f,1155.1907f,-78.849319f,2.608f);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-3, pTarget->GetPositionY()-3, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-3, pTarget->GetPositionY()+3, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-5, pTarget->GetPositionY()-5, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-5, pTarget->GetPositionY()+5, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            break;
                        case 2:
                            DoTeleportPlayer(pTarget,-8159.7753f,1127.9064f,-76.868660f,0.675f);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-3, pTarget->GetPositionY()-3, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-3, pTarget->GetPositionY()+3, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-5, pTarget->GetPositionY()-5, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            Hatchling = me->SummonCreature(15962, pTarget->GetPositionX()-5, pTarget->GetPositionY()+5, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if (Hatchling)
                                ((CreatureAI*)Hatchling->AI())->AttackStart(pTarget);
                            break;
                    }
                }
                SpawnHatchlings_Timer = 45000 + rand()%15000;
            } else SpawnHatchlings_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_fankriss(Creature* pCreature)
{
    return new boss_fankrissAI (pCreature);
}

void AddSC_boss_fankriss()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_fankriss";
    newscript->GetAI = &GetAI_boss_fankriss;
    newscript->RegisterSelf();
}

