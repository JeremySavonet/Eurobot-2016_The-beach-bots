/*
 *  Copyright Droids Corporation ( 2009 )
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  ( at your option ) any later version.
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
 *  Revision : $Id: f16.h,v 1.6.4.3 2008-05-10 15:06:26 zer0 Exp $
 *
 */

#ifndef _VECT_BASE_H_
#define _VECT_BASE_H_

// Platform includes
#include "ch.h"
#include "hal.h"
#include "test.h"

// A 2D-vector
typedef struct _vect_t
{
    float x; // x-coordinate
    float y; // y-coordinate
} vect_t;

// A 2D-point
typedef struct _point_t
{
    float x; // x-coordinate
    float y; // y-coordinate
} point_t;

/*
 * Computes dot product between 2 vectors.
 * [in] *v First vector
 * [in] *w Second vector
 * Return Dot product
 */
float vect_pscal( vect_t* v, vect_t* w );

/*
 * Returns the Z component of a cross product.
 * [in] *v First vector
 * [in] *w Second vector
 * Return Cross product
 */
float vect_pvect( vect_t* v, vect_t* w );

/*
 * Returns the sign of the dot product.
 * [in] *v First vector
 * [in] *w Second vector
 * Return Sign of the dot product ( z > 0 ? 1 : -1 )
 */
int8_t vect_pscal_sign( vect_t* v, vect_t* w );

/*
 * Returns the sign of the Z component of the cross product.
 * [in] *v First vector
 * [in] *w Second vector
 * Return Sign of the cross product ( z > 0 ? 1 : -1 )
 */
int8_t vect_pvect_sign( vect_t* v, vect_t* w );

/*
 * Computes the norm of a vector, given the raw coordinates of a start and an
 * end point.
 * [in] x1 x-coordinate of the start point
 * [in] y1 y-coordinate of the start point
 * [in] x2 x-coordinate of the end point
 * [in] y2 y-coordinate of the end point
 * Return Norm of the vector
 */
float xy_norm( float x1, float y1, float x2, float y2 );

/*
 * Computes the norm of a vector, given the start and end points.
 * [in] *p1 Start point
 * [in] *p2 End point
 * Return Norm of the vector
 */
float pt_norm( const point_t* p1, const point_t* p2 );

/*
 * Computes the norm of a vector.
 * [in] *v Vector
 * Return Norm of the vector
 */
float vect_norm( const vect_t* v );

/*
 * Rotates a vector by 90 deg CCW
 * [in,out] *v Vector to rotate
 */
void vect_rot_trigo( vect_t* v );

/*
 * Rotates a vector by 90 deg CW.
 * [in,out] *v Vector to rotate
 */
void vect_rot_retro( vect_t* v );

/*
 * Returns the angle between two vectors.
 * [in] *v Fist vector
 * [in] *w Second vector
 * Return Angle in radian
 */
float vect_get_angle( vect_t* v, vect_t* w );

/*
 * Scales a vector by a factor.
 * [in,out] *v Vector to scale
 * [in] l factor
 */
void vect_resize( vect_t* v, float l );

#endif // _VECT_BASE_H_
