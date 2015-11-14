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
 *  Revision : $Id: quadramp.h,v 1.3.4.4 2009-05-18 12:29:51 zer0 Exp $
 *
 */

#ifndef _QUADRAMP_H_
#define _QUADRAMP_H_

/*
 * This module is responsible for making speed ramps to avoid large 
 * accelerations.
 * Its functions are compatible with control_system_manager.
 *
 */

/* 
 * A quadram instance.
 *
 * The role of this filter is to limit the speed and acceleration of a value.
 * It will typically be used as a consign filter in a control system to avoid
 * sudden movements.
 */
struct quadramp_filter
{
	double var_2nd_ord_pos; // Acceleration
	double var_2nd_ord_neg; // Deceleration
	double var_1st_ord_pos; // Speed (> 0)
	double var_1st_ord_neg; // Speed (< 0) 

	double previous_var; // Speed at the previous filter iteration.
	double previous_out; // Position at the previous filter iteration.
	int32_t previous_in; // Input at the previous filter iteration.
};

/* 
 * Initialization of the filter.
 * [in] q The quadramp instance.
 */
void quadramp_init( struct quadramp_filter *q );

/*
 * Resets the filter output to zero and stops any ramp. 
 * [in] q The quadramp instance.
 */
void quadramp_reset( struct quadramp_filter *q );

/*
 * Set acceleration. 
 * [in] q The quadramp instance.
 * [in] var_2nd_ord_pos The positive acceleration.
 * [in] var_2nd_ord_neg The negative acceleration.
 */
void quadramp_set_2nd_order_vars( struct quadramp_filter *q,
				                  double var_2nd_ord_pos,
				                  double var_2nd_ord_neg );

/*
 * Set speed. 
 * [in] q The quadramp instance.
 * [in] var_1st_ord_pos The positive speed.
 * [in] var_1st_ord_neg The negative speed.
 */
void quadramp_set_1st_order_vars( struct quadramp_filter *q,
				                  double var_1st_ord_pos,
				                  double var_1st_ord_neg );

/*
 * Set position.
 *
 * the new position and sets the speed to zero. 
 * [in] q The quadramp instance.
 * [in] pos The new position.
 */
void quadramp_set_position( struct quadramp_filter *q, int32_t pos );

/*
 * Is the ramp finished.
 *
 * Returns 1 when filter_input == filter_output && speed==0.
 * [in] q The quadramp instance.
 */
uint8_t quadramp_is_finished( struct quadramp_filter *q );

/*
 * Process the ramp.
 *
 * [in] data A pointer to a quadramp instance, casted to void *.
 * [in] in The input of the filter.
 *
 * Returns The output of the filter.
 */
int32_t quadramp_do_filter( void *data, int32_t in );

#endif // _QUADRAMP_H_
