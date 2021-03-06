/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_UNITAI_H
#define BLIZZLIKE_UNITAI_H

#include "Platform/Define.h"
#include <list>
#include "Unit.h"

class Unit;
class Player;
struct AISpellInfoType;

//Selection method used by SelectTarget
enum SelectAggroTarget
{
    SELECT_TARGET_RANDOM = 0,                               //Just selects a random target
    SELECT_TARGET_TOPAGGRO,                                 //Selects targes from top aggro to bottom
    SELECT_TARGET_BOTTOMAGGRO,                              //Selects targets from bottom aggro to top
    SELECT_TARGET_NEAREST,
    SELECT_TARGET_FARTHEST,
};

class UnitAI
{
    protected:
        Unit* const me;
    public:
        explicit UnitAI(Unit* u) : me(u) {}
        virtual bool CanAIAttack(const Unit* /*who*/) const { return true; }
        virtual void AttackStart(Unit*);
        virtual void UpdateAI(const uint32 /*diff*/) = 0;

        virtual void InitializeAI() { if (!me->isDead()) Reset(); }

        virtual void Reset() {};

        // Called when unit is charmed
        virtual void OnCharmed(bool /*apply*/) = 0;

        // Pass parameters between AI
        virtual void DoAction(const int32 param = 0) { UNUSED(param); }
        virtual uint32 GetData(uint32 id = 0) { UNUSED(id); return 0; }
        virtual void SetData(uint32 /*id*/, uint32 /*value*/) {}
        virtual void SetGUID(const uint64& /*guid*/, int32 id = 0) { UNUSED(id); }
        virtual uint64 GetGUID(int32 id = 0) { UNUSED(id); return 0; }

        Unit* SelectTarget(SelectAggroTarget target, uint32 position = 0, float dist = 0, bool playerOnly = false, int32 aura = 0);
        void SelectTargetList(std::list<Unit*> &targetList, uint32 num, SelectAggroTarget target, float dist = 0, bool playerOnly = false, int32 aura = 0);

        void AttackStartCaster(Unit* victim, float dist);

        void DoCast(uint32 spellId);
        void DoCast(Unit* victim, uint32 spellId, bool triggered = false);
        void DoCastVictim(uint32 spellId, bool triggered = false);
        void DoCastAOE(uint32 spellId, bool triggered = false);

        float DoGetSpellMaxRange(uint32 spellId, bool positive = false);

        void DoMeleeAttackIfReady();
        bool DoSpellAttackIfReady(uint32 spell);

        static AISpellInfoType *AISpellInfo;
        static void FillAISpellInfo();
};

class PlayerAI : public UnitAI
{
    protected:
        Player* const me;
    public:
        explicit PlayerAI(Player* p) : UnitAI((Unit*)p), me(p) {}

        void OnCharmed(bool apply);
};

class SimpleCharmedAI : public PlayerAI
{
    public:
        void UpdateAI(const uint32 diff);
};

#endif

