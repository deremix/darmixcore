/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Molten_Core
Complete(%): 100
Comment:
Category: Molten Core
EndScriptData */

/* ContentData
mob_ancient_core_hound
EndContentData */

#include "ScriptPCH.h"
#include "ScriptedSimpleAI.h"

#define SPELL_CONE_OF_FIRE          19630
#define SPELL_BITE                  19771

//Random Debuff (each hound has only one of these)
#define SPELL_GROUND_STOMP          19364
#define SPELL_ANCIENT_DREAD         19365
#define SPELL_CAUTERIZING_FLAMES    19366
#define SPELL_WITHERING_HEAT        19367
#define SPELL_ANCIENT_DESPAIR       19369
#define SPELL_ANCIENT_HYSTERIA      19372

CreatureAI* GetAI_mob_ancient_core_hound(Creature* pCreature)
{
    SimpleAI *ai = new SimpleAI(pCreature);

    ai->Spell[0].Enabled          = true;
    ai->Spell[0].Spell_Id         = SPELL_CONE_OF_FIRE;
    ai->Spell[0].Cooldown         = 7000;
    ai->Spell[0].First_Cast       = 10000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    uint32 RandDebuff = RAND(SPELL_GROUND_STOMP,SPELL_ANCIENT_DREAD,SPELL_CAUTERIZING_FLAMES,
                             SPELL_WITHERING_HEAT,SPELL_ANCIENT_DESPAIR,SPELL_ANCIENT_HYSTERIA);

    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = RandDebuff;
    ai->Spell[1].Cooldown         = 24000;
    ai->Spell[1].First_Cast       = 15000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->Spell[2].Enabled          = true;
    ai->Spell[2].Spell_Id         = SPELL_BITE;
    ai->Spell[2].Cooldown         = 6000;
    ai->Spell[2].First_Cast       = 4000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_molten_core()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_ancient_core_hound";
    newscript->GetAI = &GetAI_mob_ancient_core_hound;
    newscript->RegisterSelf();
}

