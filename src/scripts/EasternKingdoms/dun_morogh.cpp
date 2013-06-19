/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Dun_Morogh
Complete(%): 50
Comment: Quest support: 1783
Category: Dun Morogh
EndScriptData */

/* ContentData
npc_narm_faulk
EndContentData */

#include "ScriptPCH.h"

/*######
## npc_narm_faulk
######*/

#define SAY_HEAL -1000187

struct npc_narm_faulkAI : public ScriptedAI
{
    uint32 lifeTimer;
    bool spellHit;

    npc_narm_faulkAI(Creature* c) : ScriptedAI(c) {}

    void Reset()
    {
        lifeTimer = 120000;
        me->SetUInt32Value(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
        me->SetStandState(UNIT_STAND_STATE_DEAD);
        spellHit = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void MoveInLineOfSight(Unit* /*who*/)
    {
        return;
    }

    void UpdateAI(const uint32 diff)
    {
       if (me->IsStandState())
        {
            if (lifeTimer <= diff)
            {
                EnterEvadeMode();
                return;
            }
            else
                lifeTimer -= diff;
        }
    }

    void SpellHit(Unit* /*Hitter*/, const SpellEntry *Spellkind)
    {
        if (Spellkind->Id == 8593 && !spellHit)
        {
            DoCast(me, 32343);
            me->SetStandState(UNIT_STAND_STATE_STAND);
            me->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
            //me->RemoveAllAuras();
            DoScriptText(SAY_HEAL, me);
            spellHit = true;
        }
    }

};
CreatureAI* GetAI_npc_narm_faulk(Creature* pCreature)
{
    return new npc_narm_faulkAI (pCreature);
}

void AddSC_dun_morogh()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_narm_faulk";
    newscript->GetAI = &GetAI_npc_narm_faulk;
    newscript->RegisterSelf();
}
