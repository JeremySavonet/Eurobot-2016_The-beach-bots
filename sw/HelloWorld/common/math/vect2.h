/*
 * TODO: description + copyright
 */

#ifndef _VECT2_H_
#define _VECT2_H_

// Definition of reals used in vector 2d
#define TO_RAD( x ) ( ( ( double )x ) * ( M_PI / 180.0 ) )
#define TO_DEG( x ) ( ( ( double )x ) * ( 180.0 / M_PI ) )

// Cartesian vector structure
typedef struct _vect2_cart
{
    double x;
    double y;

} vect2_cart;

// Polar vector structure
typedef struct _vect2_pol
{
    double r;
    double theta;

} vect2_pol;

/************************ Begin prototyping ************************/

/*
 * Convert a polar vector to a cartesian one
 * vp reference to target polar vector to convert from.
 * vc reference to target cartesian vector where the function write.
 * Note: This function doesn't do any malloc !
 * You have to allocate structures before calling this function.
 *
 */
void vect2_pol2cart( vect2_pol* vp, vect2_cart* vc );

/*
 * Convert a cartesian vector to a polar one
 * vc reference to target cartesian vector to convert from.
 * vp reference to target polar vector where the function write the result.
 * Note: This function doesn't do any malloc !
 * You have to allocate structures before calling this function.
 *
 */
void vect2_cart2pol( vect2_cart* vc, vect2_pol* vp );

/*
 * Add 2 polar vectors and return the result
 * v1 Reference to a polar vector to sum.
 * v2 Reference to a polar vector to sum.
 * vresult Reference to a polar vector to store the result.
 * Note: This function doesn't do any malloc !
 * You have to allocate structures before calling this function.
 * Note: This function convert the 2 entry vectors to cartesian,
 * sum them and then convert the result to polar.
 * So please think before using it.
 *
 * \f[ \vec V_{result} = \vec V_1 + \vec V_2 \f]
 */
void vect2_add_pol( vect2_pol* v1, vect2_pol* v2, vect2_pol* vresult );

/*
 * Add 2 cartesian vectors and return the result
 * v1 Reference to a cartesian vector to sum.
 * v2 Reference to a cartesian vector to sum.
 * vresult Reference to a polar vector to store the result.
 * Note: This function doesn't do any malloc !
 * You have to allocate structures before calling this function.
 *
 * \f[ \vec V_{result} = \vec V_1 + \vec V_2 \f]
 */
void vect2_add_cart( vect2_cart* v1, vect2_cart* v2, vect2_cart* vresult );

/*
 * Substract 2 polar vectors and return the result
 * v1 Reference to a polar vector to substract.
 * v2 Reference to a polar vector to substract.
 * vresult Reference to a polar vector to store the result.
 * Note: This function doesn't do any malloc !
 * You have to allocate structures before calling this function.
 * Note: This function convert the 2 entry vectors to cartesian,
 * substract them and then convert the result to polar.
 * So please think before using it.
 *
 * \f[ \vec V_{result} = \vec V_1 - \vec V_2 \f]
 */
void vect2_sub_pol( vect2_pol* v1, vect2_pol* v2, vect2_pol* vresult );

/*
 * Substract 2 cartesian vectors and return the result
 * v1 Reference to a cartesian vector to substract.
 * v2 Reference to a cartesian vector to substract.
 * vresult Reference to a cartesian vector to store the result.
 * Note: This function doesn't do any malloc !
 * You have to allocate structures before calling this function.
 *
 * \f[ \vec V_{result} = \vec V_1 - \vec V_2 \f]
 */
void vect2_sub_cart( vect2_cart* v1, vect2_cart* v2, vect2_cart* vresult );

/*
 * Multiply a cartesian vector by a scalar and return the result
 * v1 Reference to a cartesian vector.
 * alpha The multiplying scalar.
 * vresult Reference to a cartesian vector to store the result.
 * Note: This function doesn't do any malloc !
 * You have to allocate structures before calling this function.
 *
 * \f[ \vec V_{result} = \alpha\vec V_1 \f]
 */
void vect2_scale_cart( vect2_cart* v1, double alpha, vect2_cart* vresult );

/*
 * Multiply a polar vector by a scalar and return the result
 * v1 Reference to a polar vector.
 * alpha The multiplying scalar.
 * vresult Reference to a cartesian vector to store the result.
 * Note: This function doesn't do any malloc !
 * You have to allocate structures before calling this function.
 *
 * \f[ \vec V_{result} = \alpha\vec V_1 \f]
 */
void vect2_scale_pol( vect2_pol* v1, double alpha, vect2_pol* vresult );

#endif // _VECT2_H_
