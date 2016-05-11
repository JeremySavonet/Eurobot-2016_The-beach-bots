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
 *  Revision : $Id: control_system_manager.h,v 1.7.4.5 2008-03-02 17:18:34 zer0 Exp $
 *
 */

#ifndef _CONTROL_SYSTEM_MANAGER_
#define _CONTROL_SYSTEM_MANAGER_

// Platform includes
#include "ch.h"

typedef int32_t (*cs_feedback_filter) ( void*, int32_t );
typedef int32_t (*cs_correct_filter) ( void*, int32_t );
typedef int32_t (*cs_consign_filter) ( void*, int32_t );
typedef void   (*cs_process_in) ( void*, int32_t );
typedef int32_t (*cs_process_out) ( void* );


// Data structure used by the control_system_manager module
struct cs
{
    int32_t (*consign_filter)( void *, int32_t );
    void* consign_filter_params;
    
    int32_t (*correct_filter)( void*, int32_t );
    void* correct_filter_params;

    int32_t (*feedback_filter)( void*, int32_t );
    void* feedback_filter_params;

    /* 
     * Callback function for the output filter, 
     * eg: torque limiter or inertia adapter.
     * Note: This may not be the best way to do it.
     */
    int32_t (*output_filter)(void*, int32_t); 
    void* output_filter_params; // Parameter for output_filter, 
                                // will be passed as 1st param. 

    int32_t (*process_out)( void* );
    void* process_out_params;

    void (*process_in)( void*, int32_t );
    void* process_in_params;

    int32_t consign_value;
    int32_t error_value;
    int32_t out_value;
    
    int32_t filtered_feedback_value;
    int32_t filtered_consign_value;
    
    int enabled; // 1 if the control system is enabled, 0 otherwise.

};

// Initiate the control_system structure by setting all fields to NULL
void cs_init( struct cs* cs );

// Set the cs consign_filter fields in the cs structure
void cs_set_consign_filter( struct cs* cs,
                            int32_t (*consign_filter)( void*, int32_t ),
                            void* consign_filter_params );

// Set the cs correct_filter fields in the cs structure
void  cs_set_correct_filter( struct cs* cs,
                             int32_t (*correct_filter)( void*, int32_t ),
                             void* correct_filer_params );

// Set the cs feedback_filter fields in the cs structure 
void  cs_set_feedback_filter( struct cs* cs,
                              int32_t (*feedback_filter)( void*, int32_t ),
                              void* feedback_filer_params);

// Set the cs output_filter fields in the cs structure
void  cs_set_output_filter( struct cs* cs,
                            int32_t (*output_filter)( void*, int32_t ),
                            void* output_filer_params);

// Set the cs process_in fields in the cs structure
void cs_set_process_in( struct cs* cs,
                        void (*process_in)( void*, int32_t ),
                        void* process_in_params );

// Set the cs process_out fields in the cs structure
void cs_set_process_out( struct cs* cs,
                         int32_t (*process_out)( void* ),
                         void* process_out_params );


/*
 * This function do the main loop of the control system process.
 *
 * - Save the consign in the structure.
 * - Apply the consign filter to the consign.
 * - Read the process out
 * - Apply the feedback filter to the process out
 * - Substract filtered consign to filtered process out.
 * - Save the result in error_value and apply the correct filter.
 * - Save the filtered result and send it to process_in().
 * - Return this result.
 * 
 */
int32_t cs_do_process( struct cs* cs, int32_t consign );

/*
 * Apply cs_do_process() to the structure cs 
 * cs should be a (struct cs*)
 */
void cs_manage( void * cs );

// Return the last output sent to process
int32_t cs_get_out( struct cs* cs );

// Return the last calculated error
int32_t cs_get_error( struct cs* cs );

// Return the current consign
int32_t cs_get_consign( struct cs* cs );

// Return the current consign, after filter
int32_t cs_get_filtered_consign( struct cs* cs );

// Return the last feedback value, after filter
int32_t cs_get_filtered_feedback( struct cs* cs );

// Return the last feedback value, before filter
int32_t cs_get_feedback( struct cs* cs );

// Change the consign without calculating control system
void cs_set_consign( struct cs* cs, int32_t v );

// Disable control system (it will output zero)
void cs_disable( struct cs* cs );

// Enable control system (return the normal operation)
void cs_enable( struct cs* cs );

#endif // _CONTROL_SYSTEM_MANAGER_H
