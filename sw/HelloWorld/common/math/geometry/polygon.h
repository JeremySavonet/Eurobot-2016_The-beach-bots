/*
 *  Copyright Droids Corporation ( 2009 )
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  ( at your option )  any later version.
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

#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "lines.h"
#include "vect_base.h"

/*
 * A polygon.
 * An array of points, defining the corners of a geometric figure.
 */
typedef struct _poly
{
    point_t* pts; // Array of corner-points
    uint8_t l;    // Length of the array of points
} poly_t;

/*
 * Checks if a point belongs to a polygon
 * [in] *p Point to check
 * [in] *pol Polygon to check
 * Return 0 if outside, 1 if inside, 2 if on edge.
 */
uint8_t is_in_poly( const point_t* p, poly_t* pol );

/*
 * Checks if a point belongs to a polygon
 * [in] *pol Polygon to check
 * [in] *x x-coordinate of point to check
 * [in] *y y-coordinate of point to check
 * Return 0 if outside, 1 if inside, 2 if on edge.
 */
uint8_t is_point_in_poly( poly_t* pol, int16_t x, int16_t y );

/*
 * Checks if a segment is crossing a polygon
 * [in] p1, p2 The two points defining the segment.
 * [in] pol The polygon to check.
 * [out] intersect_pt Contains the intersection point.
 * Returns 0 dont cross, 1 cross, 2 on a side,
 *  3 touch out ( a segment boundary is on a polygon edge,
 *  and the second segment boundary is out of the polygon )  */
uint8_t is_crossing_poly( point_t p1,
                          point_t p2,
                          point_t* intersect_pt,
                          poly_t* pol );

/*
 * Set coordinates of bounding box.
 * [in] x1 x-coordinate bottom-left corner
 * [in] y1 y-coordiante bottom-left corner
 * [in] x2 x-coordinate top-right corner
 * [in] y2 y-coordinate top-right corner
 */
void polygon_set_boundingbox( int32_t x1, int32_t y1, int32_t x2, int32_t y2 );

/*
 * Checks if a point is in the bounding box.
 * [in] *p Point to check
 * Return 1 if p is in the bounding box. */
uint8_t is_in_boundingbox( const point_t* p );

/*
 * Constructs the visibility ray graph.
 *
 *  Giving the list of poygons, compute the graph of "visibility rays".
 * This rays array is composed of indexes representing 2 polygon
 * vertices that can "see" each others:
 *
 *  i  : the first polygon number in the input polygon list
 *  i+1: the vertex index of this polygon ( vertex 1 )
 *  i+2: the second polygon number in the input polygon list
 *  i+3: the vertex index of this polygon ( vertex 2 )
 *
 *  Here, vertex 1 can "see" vertex 2 in our visibility graph
 *
 *  As the first polygon is not a real polygon but the start/stop
 *  point, the polygon is NOT an occluding polygon ( but its vertices
 *  are used to compute visibility to start/stop points )
 *
 * [in] *polys List of polygons
 * [in] npolys Number of polygons in the list
 * [out] *rays Rays ( WTFBBQ? )
 * Return Number of rays
 */

uint8_t calc_rays( poly_t* polys, uint8_t npolys, uint8_t* rays );

/*
 * Compute the weight of every rays: the length of the rays is used
 * here.
 *
 * Note: The +1 is a little hack to introduce a preference between two
 * possiblity path: If we have 3 checkpoints aligned in a path ( say A,
 * B, C )  the algorithm will prefer ( A, C) instead of (A, B, C)
 * [in] *polys Array of polygons
 * [in] npolys Number of polygons in the array
 * [in] *rays Array of the rays
 * [in] ray_n Number of rays in the array
 * [out] *weight List of the weights of each ray
 * */
void calc_rays_weight( poly_t* polys,
                       uint8_t npolys,
                       uint8_t* rays,
                       uint8_t ray_n,
                       uint16_t* weight );

#endif // _POLYGON_H_
