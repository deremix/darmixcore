/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "Base.h"

Base::~Base ()
{
}

/*
 * Decrement object's reference count; as soon as the last reference
 * to the object is removed, it is destroyed.
 */

void Base::DecRef ()
{
    if (!--RefCount)
        delete this;
}

/*
 * Object initialization. The initial reference count is set to one;
 * this means if you call DecRef() immediately after creating the object,
 * it will be destroyed.
 */
Base::Base ()
{
    RefCount = 1;
}

/*
 * Increment reference count.
 * Every time when you copy a pointer to a object and store it for
 * later use you MUST call IncRef() on it; this will allow to keep
 * objects as long as they are referenced by some entity.
 */
void Base::IncRef ()
{
    ++RefCount;

}

/*
 * Query number of references to this object.
 * I would rather prefer to have the reference counter strictly private,
 * but sometimes, mostly for debugging, such a function can help.
 */
int Base::GetRefCount ()
{
    return RefCount;
}

