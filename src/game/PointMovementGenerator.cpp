/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "PointMovementGenerator.h"
#include "Errors.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "MapManager.h"
#include "DestinationHolderImp.h"
#include "World.h"
#include "PathFinder.h"
#include "CreatureGroups.h"

//----- Point Movement Generator
template<class T>
void PointMovementGenerator<T>::Initialize(T &unit)
{
    unit.StopMoving();
    Traveller<T> traveller(unit);

    if (m_usePathfinding)
    {
        PathInfo path(&unit, i_x, i_y, i_z);
        PointPath pointPath = path.getFullPath();

        float speed = traveller.Speed() * 0.001f; // in ms
        uint32 traveltime = uint32(pointPath.GetTotalLength() / speed);
        if (unit.GetTypeId() != TYPEID_UNIT)
            unit.SetUnitMovementFlags(SPLINEFLAG_WALKMODE);
        unit.SendMonsterMoveByPath(pointPath, 1, pointPath.size(), traveltime);

        PathNode p = pointPath[pointPath.size()-1];
        i_destinationHolder.SetDestination(traveller, p.x, p.y, p.z, false);
    }
    else
    i_destinationHolder.SetDestination(traveller, i_x, i_y, i_z, !m_usePathfinding);
}

template<class T>
bool PointMovementGenerator<T>::Update(T &unit, const uint32 &diff)
{
    if (!&unit)
        return false;

    if (unit.hasUnitState(UNIT_STAT_ROOT | UNIT_STAT_STUNNED))
    {
        if (unit.hasUnitState(UNIT_STAT_CHARGING))
            return false;
        else
            return true;
    }

    Traveller<T> traveller(unit);

    i_destinationHolder.UpdateTraveller(traveller, diff);

    if (i_destinationHolder.HasArrived())
    {
        unit.clearUnitState(UNIT_STAT_MOVE);
        arrived = true;
        return false;
    }

    return true;
}

template<class T>
void PointMovementGenerator<T>:: Finalize(T &unit)
{
    if (unit.hasUnitState(UNIT_STAT_CHARGING))
        unit.clearUnitState(UNIT_STAT_CHARGING | UNIT_STAT_JUMPING);
    if (arrived) // without this crash!
        MovementInform(unit);
}

template<class T>
void PointMovementGenerator<T>::MovementInform(T &/*unit*/)
{
}

template <> void PointMovementGenerator<Creature>::MovementInform(Creature &unit)
{
    if (id == EVENT_FALL_GROUND)
    {
        unit.setDeathState(JUST_DIED);
        unit.SetFlying(true);
    }
    unit.AI()->MovementInform(POINT_MOTION_TYPE, id);
}

template void PointMovementGenerator<Player>::Initialize(Player&);
template bool PointMovementGenerator<Player>::Update(Player &, const uint32 &diff);
template void PointMovementGenerator<Player>::MovementInform(Player&);
template void PointMovementGenerator<Player>::Finalize(Player&);

template void PointMovementGenerator<Creature>::Initialize(Creature&);
template bool PointMovementGenerator<Creature>::Update(Creature&, const uint32 &diff);
template void PointMovementGenerator<Creature>::Finalize(Creature&);

void AssistanceMovementGenerator::Finalize(Unit &unit)
{
    unit.ToCreature()->SetNoCallAssistance(false);
    unit.ToCreature()->CallAssistance();
    if (unit.isAlive())
        unit.GetMotionMaster()->MoveSeekAssistanceDistract(sWorld.getConfig(CONFIG_CREATURE_FAMILY_ASSISTANCE_DELAY));
}

