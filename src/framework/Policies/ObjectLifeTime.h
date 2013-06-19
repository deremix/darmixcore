/*
 * BlizzLikeCore Copyright (C) 2013  BlizzLikeGroup
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_OBJECTLIFETIME_H
#define BLIZZLIKE_OBJECTLIFETIME_H

#include <stdexcept>
#include "Platform/Define.h"

typedef void (* Destroyer)(void);

namespace BlizzLike
{
    void at_exit( void (*func)() );

    template <class T>
        class ObjectLifeTime
    {
        public:
            static void ScheduleCall(void (*destroyer)() )
            {
                at_exit( destroyer );
            }

            DECLSPEC_NORETURN static void OnDeadReference(void) ATTR_NORETURN;

    };

    template <class T>
        void ObjectLifeTime<T>::OnDeadReference(void)// We don't handle Dead Reference for now
    {
        throw std::runtime_error("Dead Reference");
    }
}
#endif

