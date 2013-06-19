/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Highlord_Mograine
Complete(%): 100
Comment: SCRIPT OBSOLETE
Category: Naxxramas
EndScriptData */

#include "ScriptPCH.h"

//All horsemen
#define SPELL_SHIELDWALL           29061
#define SPELL_BESERK               26662

// highlord mograine
#define SPELL_MARK_OF_MOGRAINE     28834
#define SPELL_RIGHTEOUS_FIRE       28882                    // Applied as a 25% chance on melee hit to proc. me->GetVictim()

#define SAY_TAUNT1                 "Enough prattling. Let them come! We shall grind their bones to dust."
#define SAY_TAUNT2                 "Conserve your anger! Harness your rage! You will all have outlets for your frustration soon enough."
#define SAY_TAUNT3                 "Life is meaningless. It is in death that we are truly tested."
#define SAY_AGGRO1                 "You seek death?"
#define SAY_AGGRO2                 "None shall pass!"
#define SAY_AGGRO3                 "Be still!"
#define SAY_SLAY1                  "You will find no peace in death."
#define SAY_SLAY2                  "The master's will is done."
#define SAY_SPECIAL                "Bow to the might of the Highlord!"
#define SAY_DEATH                  "I... am... released! Perhaps it's not too late to - noo! I need... more time..."

#define SOUND_TAUNT1               8842
#define SOUND_TAUNT2               8843
#define SOUND_TAUNT3               8844
#define SOUND_AGGRO1               8835
#define SOUND_AGGRO2               8836
#define SOUND_AGGRO3               8837
#define SOUND_SLAY1                8839
#define SOUND_SLAY2                8840
#define SOUND_SPECIAL              8841
#define SOUND_DEATH                8838

#define SPIRIT_OF_MOGRAINE         16775

struct boss_highlord_mograineAI : public ScriptedAI
{
    boss_highlord_mograineAI(Creature* c) : ScriptedAI(c) {}

    uint32 Mark_Timer;
    uint32 RighteousFire_Timer;
    bool ShieldWall1;
    bool ShieldWall2;

    void Reset()
    {
        Mark_Timer = 20000;                                 // First Horsemen Mark is applied at 20 sec.
        RighteousFire_Timer = 2000;                         // applied approx 1 out of 4 attacks
        ShieldWall1 = true;
        ShieldWall2 = true;
    }

    void InitialYell()
    {
        if (!me->isInCombat())
        {
            switch (rand()%3)
            {
                case 0:
                    me->MonsterYell(SAY_AGGRO1,LANG_UNIVERSAL,0);
                    DoPlaySoundToSet(me,SOUND_AGGRO1);
                    break;
                case 1:
                    me->MonsterYell(SAY_AGGRO2,LANG_UNIVERSAL,0);
                    DoPlaySoundToSet(me,SOUND_AGGRO2);
                    break;
                case 2:
                    me->MonsterYell(SAY_AGGRO3,LANG_UNIVERSAL,0);
                    DoPlaySoundToSet(me,SOUND_AGGRO3);
                    break;
            }
        }
    }

    void KilledUnit()
    {
        switch (rand()%2)
        {
            case 0:
                me->MonsterYell(SAY_SLAY1,LANG_UNIVERSAL,0);
                DoPlaySoundToSet(me,SOUND_SLAY1);
                break;
            case 1:
                me->MonsterYell(SAY_SLAY2,LANG_UNIVERSAL,0);
                DoPlaySoundToSet(me,SOUND_SLAY2);
                break;
        }
    }

    void JustDied(Unit*)
    {
        me->MonsterYell(SAY_DEATH,LANG_UNIVERSAL,0);
        DoPlaySoundToSet(me, SOUND_DEATH);
    }

    void EnterCombat(Unit*)
    {
        InitialYell();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        // Mark of Mograine
        if (Mark_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_MARK_OF_MOGRAINE);
            Mark_Timer = 12000;
        } else Mark_Timer -= diff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (ShieldWall1 && (me->GetHealth()*100 / me->GetMaxHealth()) < 50)
        {
            if (ShieldWall1)
            {
                DoCast(me,SPELL_SHIELDWALL);
                ShieldWall1 = false;
            }
        }
        if (ShieldWall2 && (me->GetHealth()*100 / me->GetMaxHealth()) < 20)
        {
            if (ShieldWall2)
            {
                DoCast(me,SPELL_SHIELDWALL);
                ShieldWall2 = false;
            }
        }

        // Righteous Fire
        if (RighteousFire_Timer <= diff)
        {
            if (rand()%4 == 1)                               // 1/4
            {
                DoCast(me->getVictim(),SPELL_RIGHTEOUS_FIRE);
            }
            RighteousFire_Timer = 2000;
        } else RighteousFire_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_highlord_mograine(Creature* pCreature)
{
    return new boss_highlord_mograineAI (pCreature);
}

void AddSC_boss_highlord_mograine()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_highlord_mograine";
    newscript->GetAI = &GetAI_boss_highlord_mograine;
    newscript->RegisterSelf();
}

