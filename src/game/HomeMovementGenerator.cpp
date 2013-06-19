/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "HomeMovementGenerator.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "Traveller.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "DestinationHolderImp.h"
#include "WorldPacket.h"

void
HomeMovementGenerator<Creature>::Initialize(Creature & owner)
{
    owner.RemoveUnitMovementFlag(MOVEFLAG_WALK_MODE);
    _setTargetLocation(owner);
}

void
HomeMovementGenerator<Creature>::Reset(Creature &)
{
}

void
HomeMovementGenerator<Creature>::_setTargetLocation(Creature & owner)
{
    if (!&owner)
        return;

    if (owner.hasUnitState(UNIT_STAT_ROOT | UNIT_STAT_STUNNED | UNIT_STAT_DISTRACTED))
        return;

    float x, y, z;
    owner.GetHomePosition(x, y, z, ori);

    CreatureTraveller traveller(owner);

    i_destinationHolder.SetDestination(traveller, x, y, z, false);

    PathInfo path(&owner, x, y, z, true);
    PointPath pointPath = path.getFullPath();

    float speed = traveller.Speed() * 0.001f; // in ms
    uint32 traveltime = uint32(pointPath.GetTotalLength() / speed);
    modifyTravelTime(traveltime);
    owner.SendMonsterMoveByPath(pointPath, 1, pointPath.size(), traveltime);

    owner.clearUnitState(UNIT_STAT_ALL_STATE);
}

bool
HomeMovementGenerator<Creature>::Update(Creature &owner, const uint32& time_diff)
{
    CreatureTraveller traveller(owner);
    i_destinationHolder.UpdateTraveller(traveller, time_diff);

    if (time_diff > i_travel_time)
    {
        owner.AddUnitMovementFlag(MOVEFLAG_WALK_MODE);

        // restore orientation of not moving creature at returning to home
        if (owner.GetDefaultMovementType() == IDLE_MOTION_TYPE)
        {
            owner.SetOrientation(ori);
            WorldPacket packet;
            owner.BuildHeartBeatMsg(&packet);
            owner.SendMessageToSet(&packet, false);
        }

        owner.AI()->JustReachedHome();
        return false;
    }

    i_travel_time -= time_diff;

    return true;
}

