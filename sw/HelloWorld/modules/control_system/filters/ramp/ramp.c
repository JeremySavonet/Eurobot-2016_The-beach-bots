/*
 *  Copyright Droids Corporation, Microb Technology, Eirbot (2005)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 */

#include "ramp.h"

void ramp_init( struct ramp_filter* r )
{
    r->var_neg = 0xFFFFFFFF;
    r->var_pos = 0xFFFFFFFF;
    r->prev_out = 0;
}

void ramp_set_vars( struct ramp_filter* r, uint32_t neg, uint32_t pos )
{
    r->var_neg = neg;
    r->var_pos = pos;
}

// Filter the in value using the ramp_filter r
int32_t ramp_do_filter( void* data, int32_t in )
{
    uint32_t variation;
    struct ramp_filter* r = ( struct ramp_filter* )data;

    // test if the variation is positive or negative
    if( in > r->prev_speed )
    {
        variation = in - r->prev_speed;

        // test if the variation is too high
        if( variation < r->var_pos )
        {
            r->prev_speed = in;
        }
        else
        {
            r->prev_speed = r->prev_speed + r->var_pos;
        }
    }
    else
    {
        variation = r->prev_speed - in;
        if( variation < r->var_neg )
        {
            r->prev_speed = in;
        }
        else
        {
            r->prev_speed = r->prev_speed - r->var_neg;
        }
    }

    r->prev_out += r->prev_speed;

    // We need a speed
    return r->prev_out;
}
