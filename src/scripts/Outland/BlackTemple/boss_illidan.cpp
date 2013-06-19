/*******************************************************
 * File:'boss_illidan'
 * ScriptName:'npc_illidan'
 * SDCreator: Helper
 * SDComplete: 0%
 * SDComment: Illidan Stormrage
 * SDCategory: NPC
 *******************************************************/
#include "ScriptPCH.h"
#include "black_temple.h"

//Spell ID
enum Spells
{
	//Illidan Spells
	SPELL_CATACLYSMIC_BOLT                     = 38441, //Cataclysmic Bolt
	SPELL_BERSERK                              = 47008, //Berserk
	SPELL_SHEAR                                = 41032, //Shear
	SPELL_MORTAL_STRIKE                        = 37335, //Mortal Strike
	SPELL_IMPALE                               = 28783, //Impale
	SPELL_FIRE_BALL                            = 40598, //Fire Ball
	SPELL_DRAW_SOUL                            = 40904, //Draw Soul
	SPELL_FLAME_CRASH                          = 40832, //Flame Crash
	SPELL_SHADOW_BLAST                         = 41078, //Shadow Blast
	SPELL_DUAL_WIELD                           = 42459, //Dual Wield
	SPELL_FROST_BLAST                          = 27808, //Frost Blast

	//Illidan Summoner
	SPELL_CHAIN_LIGHTING                       = 40536, //Chain Lighting
	SPELL_FLAME_BLAST                          = 40631, //Flame Blast
	SPELL_SHADOW_CHANNELING                    = 46757  //Shadow Channeling
};

//Creature ID
enum CreatureEntry
{
    ILLIDAN_STORMRAGE       = 50006,
    ILLIDAN_SUMMONER        = 22997,
};

enum Phase
{
	PHASE_SUMMONER           = 1,
	PHASE_ILLIDAN            = 2
};

// Locations of the Hand of Deceiver adds
Position SummonerLocations[2]=
{
    {695.105f, 305.303f, 354.256f, 0.00000f},
    {659.338f, 305.303f, 354.256f, 3.12825f}
};

struct mob_illidan_controllerAI : public Scripted_NoMovementAI
{
	mob_illidan_controllerAI(Creature* c) : Scripted_NoMovementAI(c), summons(me)
    {
        pInstance = c->GetInstanceData();
    }

    ScriptedInstance* pInstance;
    SummonList summons;

	bool bSummonedSummoner;
    bool bIllidanDeath;

	uint32 phase;
	uint8 summonerDeathCount;

	void InitializeAI()
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        me->addUnitState(UNIT_STAT_STUNNED);

        ScriptedAI::InitializeAI();
    }

	void Reset()
	{
		phase = PHASE_SUMMONER;

		summonerDeathCount = 0;
		bSummonedSummoner = false;
		bIllidanDeath = false;
		summons.DespawnAll();
	}

	void JustSummoned(Creature* summoned)
	{
		switch (summoned->GetEntry())
		{
		case ILLIDAN_SUMMONER:
			summoned->CastSpell(summoned, SPELL_SHADOW_CHANNELING, false);
			break;
		case ILLIDAN_STORMRAGE:
			summoned->AddThreat(me->getVictim(), 1.0f);
			summoned->CastSpell(me, SPELL_DUAL_WIELD, false);
			break;
		}
		summons.Summon(summoned);
	}

	void UpdateAI(const uint32 diff)
	{
		if (!bSummonedSummoner)
		{
            for (uint8 i = 0; i < 2; ++i)
                me->SummonCreature(ILLIDAN_SUMMONER, SummonerLocations[i], TEMPSUMMON_DEAD_DESPAWN, 0);

			bSummonedSummoner = true;
		}

		if (summonerDeathCount > 1 && phase == PHASE_SUMMONER)
		{
			phase = PHASE_ILLIDAN;
			DoSpawnCreature(ILLIDAN_STORMRAGE, 0, 0,0, 0, TEMPSUMMON_MANUAL_DESPAWN, 0);
		}
	}
};
struct mob_illidan_summonerAI : public ScriptedAI
{
    mob_illidan_summonerAI(Creature* c) : ScriptedAI(c)
    {
		pInstance = c->GetInstanceData();
	}

	ScriptedInstance* pInstance;

	uint32 ChainLightingTimer;
	uint32 FlameBlastTimer;
	uint32 BerserkTimer;

	void Reset()
	{
		ChainLightingTimer = 10000;
		FlameBlastTimer = 15000;
		BerserkTimer = 300000;
		if (pInstance)
            pInstance->SetData(DATA_ILLIDANSTORMRAGEEVENT, NOT_STARTED);
	}

	void EnterCombat(Unit* who)
	{
		if (pInstance)
        {
            pInstance->SetData(DATA_ILLIDANSTORMRAGEEVENT, IN_PROGRESS);
            if (Creature* pControl = Unit::GetCreature(*me, pInstance->GetData64(DATA_ILLIDANCONTROLLER)))
                pControl->AddThreat(who, 1.0f);
        }
		me->InterruptNonMeleeSpells(true);
		me->MonsterYell("For my lord Illidan Stormrage!", LANG_UNIVERSAL, who->GetGUID());
	}

	void JustDied(Unit* player)
	{
		if (!pInstance)
            return;

		if (Creature* pControl = Unit::GetCreature(*me, pInstance->GetData64(DATA_ILLIDANCONTROLLER)))
            ++(CAST_AI(mob_illidan_controllerAI, pControl->AI())->summonerDeathCount);

		me->MonsterYell("I cant... My lord will crush You!", LANG_UNIVERSAL, player->GetGUID());
	}

	void KilledUnit(Unit* player)
    {
        me->MonsterYell("Any one next want die?!", LANG_UNIVERSAL, player->GetGUID());
    }

	void UpdateAI(const uint32 diff)
	{
		if (!me->isInCombat())
            DoCast(me, SPELL_SHADOW_CHANNELING);

		if (!UpdateVictim())
            return;

		if (ChainLightingTimer <= diff)
		{
			DoCast(me->getVictim(), SPELL_CHAIN_LIGHTING);
			ChainLightingTimer = 10000;
		} else ChainLightingTimer -= diff;

		if (FlameBlastTimer <= diff)
		{
			DoCast(me->getVictim(), SPELL_FLAME_BLAST);
			FlameBlastTimer = 15000;
		} else FlameBlastTimer -= diff;

		DoMeleeAttackIfReady();
	}
};

struct boss_illidan_stormrageAI : public ScriptedAI
{
    boss_illidan_stormrageAI(Creature* c) : ScriptedAI(c)
    {
		pInstance = c->GetInstanceData();
	}

	ScriptedInstance* pInstance;

	uint32 CataclysmicBoltTimer;
	uint32 BerserkTimer;
	uint32 ShearTimer;
	uint32 MortalStrikeTimer;
	uint32 ImpaleTimer;
	uint32 FireBallTimer;
	uint32 DrawSoulTimer;
	uint32 FlameCrashTimer;
	uint32 FrostBlastTimer;

	uint8 Phase;

	void Reset()
	{
		CataclysmicBoltTimer = 5000+rand()%2000;
		BerserkTimer = 450000;
		ShearTimer = 15000+rand()%7000;
		MortalStrikeTimer = 20000+rand()%6000;
		ImpaleTimer = 30000+rand()%4000;
		FireBallTimer = 10000+rand()%1000;
		DrawSoulTimer = 12000+rand()%3000;
		FlameCrashTimer = 41000+rand()%2000;
		FrostBlastTimer = 22000+rand()%5000;

		Phase = PHASE_ILLIDAN;

		me->SetFloatValue(UNIT_FIELD_COMBATREACH, 12);
	}

	void EnterCombat(Unit* player)
    {
        DoZoneInCombat();

		switch (urand(0,1))
		{
		case 0:
			me->MonsterYell("You can't beat me!", LANG_UNIVERSAL, player->GetGUID());
			break;
		case 1:
			me->MonsterYell("I will take your life!", LANG_UNIVERSAL, player->GetGUID());
			break;
		}
    }

	void EnterEvadeMode()
	{
		ScriptedAI::EnterEvadeMode();

		if (pInstance)
            if (Creature* pControl = Unit::GetCreature(*me, pInstance->GetData64(DATA_ILLIDANCONTROLLER)))
                CAST_AI(mob_illidan_controllerAI, pControl->AI())->Reset();
	}

	void JustDied(Unit* player)
	{
		if (!pInstance)
			return;

		pInstance->SetData(DATA_ILLIDANSTORMRAGEEVENT, DONE);

		switch (urand(0,1))
		{
		case 0:
			me->MonsterYell("No, this is no my END!", LANG_UNIVERSAL, player->GetGUID());
			break;
		case 1:
			me->MonsterYell("I come back!", LANG_UNIVERSAL, player->GetGUID());
			break;
		}
	}

	void KilledUnit(Unit* player)
    {
		switch (urand(0,1))
		{
		case 0:
			me->MonsterYell("Who shall be next to taste my blades?!", LANG_UNIVERSAL, player->GetGUID());
			DoPlaySoundToSet(me, 11473);
			break;
		case 1:
			me->MonsterYell("This is too easy!", LANG_UNIVERSAL, player->GetGUID());
			DoPlaySoundToSet(me, 11472);
			break;
		}
    }

	void UpdateAI(const uint32 diff)
	{
		if (!UpdateVictim())
			return;

        if (CataclysmicBoltTimer <= diff)
        {
            Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if (!pTarget)
                pTarget = me->getVictim();

            if (pTarget)
                DoCast(pTarget, SPELL_CATACLYSMIC_BOLT);
            CataclysmicBoltTimer = 5000;
        } else CataclysmicBoltTimer -= diff;

		if (BerserkTimer <= diff)
		{
			DoCast(me, SPELL_BERSERK);
			BerserkTimer = 450000;
		} else BerserkTimer -= diff;

		if (ShearTimer <= diff)
		{
			DoCast(me->getVictim(), SPELL_SHEAR);
			ShearTimer = 15000;
		} else ShearTimer -= diff;

		if (MortalStrikeTimer <= diff)
		{
			DoCast(me->getVictim(), SPELL_MORTAL_STRIKE);
			MortalStrikeTimer = 10000;
		} else MortalStrikeTimer -= diff;

		if (ImpaleTimer <= diff)
        {
            //select a random unit other than the main tank
            Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);

            //if there aren't other units, cast on the tank
            if (!pTarget)
                pTarget = me->getVictim();

            if (pTarget)
                DoCast(pTarget, SPELL_IMPALE);
            ImpaleTimer = 25000;
        } else ImpaleTimer -= diff;

		if (FireBallTimer <= diff)
		{
			DoCast(me->getVictim(), SPELL_FIRE_BALL);
			FireBallTimer = 12000;
		} else FireBallTimer -= diff;

		if (DrawSoulTimer <= diff)
		{
			DoCast(me->getVictim(), SPELL_DRAW_SOUL);
			DrawSoulTimer = 20000;
		} else DrawSoulTimer -= diff;

		if (FlameCrashTimer <= diff)
		{
			DoCast(me->getVictim(), SPELL_FLAME_CRASH);
			FlameCrashTimer = 30000;
		} else FlameCrashTimer -= diff;

		if (FrostBlastTimer <= diff)
		{
			DoCast(me->getVictim(), SPELL_FROST_BLAST);
			FrostBlastTimer = 41000;
		} else FrostBlastTimer -= diff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_illidan_controller(Creature* creature)
{
	return new mob_illidan_controllerAI (creature);
}
CreatureAI* GetAI_mob_illidan_summoner(Creature* pCreature)
{
    return new mob_illidan_summonerAI (pCreature);
}

CreatureAI* GetAI_boss_illidan_stormrage(Creature* pCreature)
{
    return new boss_illidan_stormrageAI (pCreature);
}

void AddSC_boss_illidan()
{
    Script *newscript;
	newscript = new Script;
	newscript->Name = "mob_illidan_controller";
	newscript->GetAI = &GetAI_mob_illidan_controller;
	newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_illidan_summoner";
    newscript->GetAI = &GetAI_mob_illidan_summoner;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_illidan_stormrage";
    newscript->GetAI = &GetAI_boss_illidan_stormrage;
    newscript->RegisterSelf();
}

