#ifndef _VECT2_H_
#define _VECT2_H_

/** \brief Definition of reals used in vector 2d */
///typedef double Real;

#define TO_RAD(x) (((double)x)*(M_PI/180.0))
#define TO_DEG(x) (((double)x)*(180.0/M_PI))

/** \brief Cartesian vector structure
**/
typedef struct _vect2_cart
{
   double x;
   double y;
   
}vect2_cart;


/** \brief Polar vector structure
**/
typedef struct _vect2_pol
{
   double r;
   double theta;
   
}vect2_pol;

/************************ Begin prototyping ************************/

/** \addtogroup Convertions
 * This functions allow to convert between the 2 vector types.
 * @{
 */

/** \brief Convert a polar vector to a cartesian one
* \param vp reference to target polar vector to convert from.
* \param vc reference to target cartesian vector where the function write.
* \warning This function doesn't do any malloc ! You have to allocate structures before calling this function.
*
**/
void vect2_pol2cart(vect2_pol* vp, vect2_cart* vc);


/** \brief Convert a cartesian vector to a polar one
* \param vc reference to target cartesian vector to convert from.
* \param vp reference to target polar vector where the function write the result.
* \warning This function doesn't do any malloc ! You have to allocate structures before calling this function.
*
**/
void vect2_cart2pol(vect2_cart* vc, vect2_pol* vp);

/** @} */


/** \addtogroup Basics
 * This functions allow basic vector operations.
 * @{
 */


/** \brief Add 2 polar vectors and return the result
* \param v1 Reference to a polar vector to sum.
* \param v2 Reference to a polar vector to sum.
* \param vresult Reference to a polar vector to store the result.
* \warning This function doesn't do any malloc ! You have to allocate structures before calling this function.
* \note This function convert the 2 entry vectors to cartesian, sum them and then convert the result to polar.
* So please think before using it.
*
* \f[ \vec V_{result} = \vec V_1 + \vec V_2 \f]
**/
void vect2_add_pol(vect2_pol* v1, vect2_pol* v2, vect2_pol* vresult);


/** \brief Add 2 cartesian vectors and return the result
* \param v1 Reference to a cartesian vector to sum.
* \param v2 Reference to a cartesian vector to sum.
* \param vresult Reference to a polar vector to store the result.
* \warning This function doesn't do any malloc ! You have to allocate structures before calling this function.
*
* \f[ \vec V_{result} = \vec V_1 + \vec V_2 \f]
**/
void vect2_add_cart(vect2_cart* v1, vect2_cart* v2, vect2_cart* vresult);


/** \brief Substract 2 polar vectors and return the result
* \param v1 Reference to a polar vector to substract.
* \param v2 Reference to a polar vector to substract.
* \param vresult Reference to a polar vector to store the result.
* \warning This function doesn't do any 
	Real R = 1.45064930529587234;
	Real theta = 0.6734390282904231341;malloc ! You have to allocate structures before calling this function.
* \note This function convert the 2 entry vectors to cartesian, substract them and then convert the result to polar.
* So please think before using it.
*
* \f[ \vec V_{result} = \vec V_1 - \vec V_2 \f]
**/
void vect2_sub_pol(vect2_pol* v1, vect2_pol* v2, vect2_pol* vresult);

/** \brief Substract 2 cartesian vectors and return the result
* \param v1 Reference to a cartesian vector to substract.
* \param v2 Reference to a cartesian vector to substract.
* \param vresult Reference to a cartesian vector to store the result.
* \warning This function doesn't do any malloc ! You have to allocate structures before calling this function.
* 
* \f[ \vec V_{result} = \vec V_1 - \vec V_2 \f]
**/
void vect2_sub_cart(vect2_cart* v1, vect2_cart* v2, vect2_cart* vresult);


/** \brief Multiply a cartesian vector by a scalar and return the result
* \param v1 Reference to a cartesian vector.
* \param alpha The multiplying scalar.
* \param vresult Reference to a cartesian vector to store the result.
* \warning This function doesn't do any malloc ! You have to allocate structures before calling this function.
*
* \f[ \vec V_{result} = \alpha\vec V_1 \f]
**/
void vect2_scale_cart(vect2_cart* v1, double alpha, vect2_cart* vresult);

/** \brief Multiply a polar vector by a scalar and return the result
* \param v1 Reference to a polar vector.
* \param alpha The multiplying scalar.
* \param vresult Reference to a cartesian vector to store the result.
* \warning This function doesn't do any malloc ! You have to allocate structures before calling this function.
*
* \f[ \vec V_{result} = \alpha\vec V_1 \f]
**/
void vect2_scale_pol(vect2_pol* v1, double alpha, vect2_pol* vresult);

/** @} */


#endif /*_VECT2_H_*/
