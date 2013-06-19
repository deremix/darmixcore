/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef __BASE_H__
#define __BASE_H__

#include "Common.h"

/*
 * This class is intended to be a base class for every other class.
 * It defines the basic interface available for any object.
 */
class Base
{
    private:
        /// Object reference count
        int RefCount;

    protected:
        /*
         * Destroy this object. Destructor is virtual, because class contains
         * virtual methods; also it is private because it is never intended
         * to be called directly; use DecRef() instead: when reference counter
         * reaches zero, the object will be destroyed.
         */
        virtual ~Base ();

    public:

        Base ();

        void IncRef ();

        void DecRef ();
        int GetRefCount ();

};
#endif                                                      // __BASE_H__

