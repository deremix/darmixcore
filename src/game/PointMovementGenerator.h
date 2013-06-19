/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_POINTMOVEMENTGENERATOR_H
#define BLIZZLIKE_POINTMOVEMENTGENERATOR_H

#include "MovementGenerator.h"
#include "DestinationHolder.h"
#include "Traveller.h"
#include "FollowerReference.h"

template<class T>
class PointMovementGenerator
: public MovementGeneratorMedium< T, PointMovementGenerator<T> >
{
    public:
        PointMovementGenerator(uint32 _id, float _x, float _y, float _z, bool _usePathfinding) : id(_id),
            i_x(_x), i_y(_y), i_z(_z), i_nextMoveTime(0), arrived(false), m_usePathfinding(_usePathfinding) {}

        void Initialize(T &);
        void Finalize(T &unit);
        void Reset(T &unit){unit.StopMoving();}
        bool Update(T &, const uint32 &diff);

        void MovementInform(T &);

        MovementGeneratorType GetMovementGeneratorType() { return POINT_MOTION_TYPE; }

        bool GetDestination(float& x, float& y, float& z) const { x=i_x; y=i_y; z=i_z; return true; }
    private:
        uint32 id;
        float i_x,i_y,i_z;
        bool m_usePathfinding;
        TimeTracker i_nextMoveTime;
        DestinationHolder< Traveller<T> > i_destinationHolder;
        bool arrived;
};

class AssistanceMovementGenerator
: public PointMovementGenerator<Creature>
{
    public:
        AssistanceMovementGenerator(float _x, float _y, float _z) :
            PointMovementGenerator<Creature>(0, _x, _y, _z, true) {}

        MovementGeneratorType GetMovementGeneratorType() { return ASSISTANCE_MOTION_TYPE; }
        void Finalize(Unit &);
};

#endif

