/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Razuvious
Complete(%): 50
Comment: Missing adds and event is impossible without Mind Control
Category: Naxxramas
EndScriptData */

#include "ScriptPCH.h"

//Razuvious - NO TEXT sound only
//8852 aggro01 - Hah hah, I'm just getting warmed up!
//8853 aggro02 Stand and fight!
//8854 aggro03 Show me what you've got!
//8861 slay1 - You should've stayed home!
//8863 slay2-
//8858 cmmnd3 - You disappoint me, students!
//8855 cmmnd1 - Do as I taught you!
//8856 cmmnd2 - Show them no mercy!
//8859 cmmnd4 - The time for practice is over! Show me what you've learned!
//8861 Sweep the leg! Do you have a problem with that?
//8860 death - An honorable... death...
//8947 - Aggro Mixed? - ?

#define SOUND_AGGRO1    8852
#define SOUND_AGGRO2    8853
#define SOUND_AGGRO3    8854
#define SOUND_SLAY1     8861
#define SOUND_SLAY2     8863
#define SOUND_COMMND1   8855
#define SOUND_COMMND2   8856
#define SOUND_COMMND3   8858
#define SOUND_COMMND4   8859
#define SOUND_COMMND5   8861
#define SOUND_DEATH     8860
#define SOUND_AGGROMIX  8847

#define SPELL_UNBALANCINGSTRIKE     26613
#define SPELL_DISRUPTINGSHOUT       29107

struct boss_razuviousAI : public ScriptedAI
{
    boss_razuviousAI(Creature* c) : ScriptedAI(c) {}

    uint32 UnbalancingStrike_Timer;
    uint32 DisruptingShout_Timer;
    uint32 CommandSound_Timer;

    void Reset()
    {
        UnbalancingStrike_Timer = 30000;                    //30 seconds
        DisruptingShout_Timer = 25000;                      //25 seconds
        CommandSound_Timer = 40000;                         //40 seconds
    }

    void KilledUnit(Unit*)
    {
        if (rand()%3)
            return;

        switch (rand()%2)
        {
            case 0:
                DoPlaySoundToSet(me, SOUND_SLAY1);
                break;
            case 1:
                DoPlaySoundToSet(me, SOUND_SLAY2);
                break;
        }
    }

    void JustDied(Unit*)
    {
        DoPlaySoundToSet(me, SOUND_DEATH);
    }

    void EnterCombat(Unit*)
    {
        switch (rand()%3)
        {
            case 0:
                DoPlaySoundToSet(me, SOUND_AGGRO1);
                break;
            case 1:
                DoPlaySoundToSet(me, SOUND_AGGRO2);
                break;
            case 2:
                DoPlaySoundToSet(me, SOUND_AGGRO3);
                break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //UnbalancingStrike_Timer
        if (UnbalancingStrike_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_UNBALANCINGSTRIKE);
            UnbalancingStrike_Timer = 30000;
        } else UnbalancingStrike_Timer -= diff;

        //DisruptingShout_Timer
        if (DisruptingShout_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_DISRUPTINGSHOUT);
            DisruptingShout_Timer = 25000;
        } else DisruptingShout_Timer -= diff;

        //CommandSound_Timer
        if (CommandSound_Timer <= diff)
        {
            switch (rand()%5)
            {
                case 0:
                    DoPlaySoundToSet(me, SOUND_COMMND1);
                    break;
                case 1:
                    DoPlaySoundToSet(me, SOUND_COMMND2);
                    break;
                case 2:
                    DoPlaySoundToSet(me, SOUND_COMMND3);
                    break;
                case 3:
                    DoPlaySoundToSet(me, SOUND_COMMND4);
                    break;
                case 4:
                    DoPlaySoundToSet(me, SOUND_COMMND5);
                    break;
            }

            CommandSound_Timer = 40000;
        } else CommandSound_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_razuvious(Creature* pCreature)
{
    return new boss_razuviousAI (pCreature);
}

void AddSC_boss_razuvious()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_razuvious";
    newscript->GetAI = &GetAI_boss_razuvious;
    newscript->RegisterSelf();
}

