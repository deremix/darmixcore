/*
 * BlizzLikeCore Copyright (C) 2013  BlizzLikeGroup
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_REFERENCEHOLDER_H
#define BLIZZLIKE_REFERENCEHOLDER_H

/* ReferenceHolder holds the actualy referenced obejct as well the refence
   count.  The ReferenecHolder implements as a policy base object and
   will decided by the Reference class to be consnsitent.
*/

template
<
typename T,
class THREADING_MODEL
>
struct ReferenceHolder : public THREADING_MODEL
{
    explicit ReferenceHolder(T *ref) : i_referencee(ref), i_referenceCount(0) {}
    T *i_referencee;
    unsigned int i_referenceCount;
    typedef typename THREADING_MODEL::Lock Lock;
};
#endif

