/*
 * BlizzLikeCore Copyright (C) 2013  BlizzLikeGroup
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_SINGLETON_H
#define BLIZZLIKE_SINGLETON_H

#include "CreationPolicy.h"
#include "ThreadingModel.h"
#include "ObjectLifeTime.h"

namespace BlizzLike
{
    template
        <
        typename T,
        class ThreadingModel = BlizzLike::SingleThreaded<T>,
        class CreatePolicy = BlizzLike::OperatorNew<T>,
        class LifeTimePolicy = BlizzLike::ObjectLifeTime<T>
        >
        class Singleton
    {
        public:
            static T& Instance();

        protected:
            Singleton() {};

        private:

            // Prohibited actions...this does not prevent hijacking.
            Singleton(const Singleton &);
            Singleton& operator=(const Singleton &);

            // Singleton Helpers
            static void DestroySingleton();

            // data structure
            typedef typename ThreadingModel::Lock Guard;
            static T *si_instance;
            static bool si_destroyed;
    };
}
#endif

