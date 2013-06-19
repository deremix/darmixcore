/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKECORE_PROGRESSBAR_H
#define BLIZZLIKECORE_PROGRESSBAR_H

#include <stdio.h>
#include "Platform/Define.h"

class barGoLink
{
    static char const * const empty;
    static char const * const full;

    int rec_no;
    int rec_pos;
    int num_rec;
    int indic_len;

    public:

        void step( void );
        barGoLink( int );
        ~barGoLink();
};
#endif

