/*
 * Simple utils module in which we defines usefull macros
 */

#ifndef _UTILS_H_
#define _UTILS_H_

/*
 *  signed maxmimum : both signs are tested
 */
#define S_MAX( to_saturate, value_max )     \
    do                                      \
    {                                       \
        if( to_saturate > value_max )       \
            to_saturate = value_max;        \
        else if( to_saturate < -value_max ) \
            to_saturate = -value_max;       \
    } while( 0 )

/*
 *  unsigned maximum : result >0 is forced
 */
#define U_MAX( to_saturate, value_max ) \
    do                                  \
    {                                   \
        if( to_saturate > value_max )   \
            to_saturate = value_max;    \
        else if( to_saturate < 0 )      \
            to_saturate = 0;            \
    } while( 0 )

/*
 *   simple maximum
 */
#define MAX( to_saturate, value_max ) \
    do                                \
    {                                 \
        if( to_saturate > value_max ) \
            to_saturate = value_max;  \
    } while( 0 )

/*
 *  simple minimum
 */
#define MIN( to_saturate, value_min ) \
    do                                \
    {                                 \
        if( to_saturate < value_min ) \
            to_saturate = value_min;  \
    } while( 0 )

/*
 *  Computes the absolute value of a number
 *
 *  While the abs() function in the libc works only with int type
 *  this macro works with every numerical type including floats
 *
 *  @note On float this is a bad idea, fabs() is faster.
 */
#define ABS( val )                       \
    ( {                                  \
        __typeof( val ) __val = ( val ); \
        if( __val < 0 )                  \
            __val = -__val;              \
        __val;                           \
    } )

#endif // _UTILS_H_
