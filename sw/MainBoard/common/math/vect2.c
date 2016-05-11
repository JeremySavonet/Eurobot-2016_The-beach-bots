/*
 * TODO: Description and copyright
 */
#include "vect2.h"

#include <stdlib.h>
#include <math.h>

// Convert a polar vector to a cartesian one
void vect2_pol2cart( vect2_pol* vp, vect2_cart* vc )
{
    if( NULL == vp ) 
    {
        return;
    }
    if( NULL == vc ) 
    {
        return;
    }
    
    vc->x = (double)( ( vp->r ) * cos( vp->theta ) );
    vc->y = (double)( ( vp->r ) * sin( vp->theta ) );
} 

// Convert a cartesian vector to a polar one
void vect2_cart2pol( vect2_cart* vc, vect2_pol* vp )
{   
    if( NULL == vc )
    {
        return;
    }
    if( NULL == vp ) 
    {
        return;
    }
    
    vp->r = (double)( sqrt( ( vc->x ) * ( vc->x ) + ( vc->y ) * ( vc->y ) ) );
    if( 0 == vc->x ) 
    {
        vp->theta = (double)0.0;
    }
    else
    {
        vp->theta = (double)atan2( vc->y,vc->x );
    }
} 

// Add 2 polar vectors and return the result
void vect2_add_pol( vect2_pol* vp1, vect2_pol* vp2, vect2_pol* vresult )
{
    vect2_cart vc1={0,0};
    vect2_cart vc2={0,0};
    
    vect2_cart vc;
    
    vect2_pol2cart( vp1, &vc1 );
    vect2_pol2cart( vp2, &vc2 );
    
    vect2_add_cart( &vc1, &vc2, &vc );
    
    vect2_cart2pol( &vc, vresult );
} 

// Add 2 cartesian vectors and return the result
void vect2_add_cart( vect2_cart* vc1, vect2_cart* vc2, vect2_cart* vresult )
{
    vresult->x = vc1->x + vc2->x;
    vresult->y = vc1->y + vc2->y;
}


// Substract 2 polar vectors and return the result
void vect2_sub_pol( vect2_pol* vp1, vect2_pol* vp2, vect2_pol* vresult )
{
    vect2_cart vc1={0,0};
    vect2_cart vc2={0,0};
    
    vect2_cart vc;
    
    vect2_pol2cart( vp1, &vc1 );
    vect2_pol2cart( vp2, &vc2 );
    
    vect2_sub_cart( &vc1, &vc2, &vc );
    
    vect2_cart2pol( &vc, vresult );
} 

// Substract 2 cartesian vectors and return the result
void vect2_sub_cart( vect2_cart* vc1, vect2_cart* vc2, vect2_cart* vresult )
{
    vresult->x = vc1->x - vc2->x;
    vresult->y = vc1->y - vc2->y;
}


// Multiply a cartesian vector by a scalar and return the result
void vect2_scale_cart( vect2_cart* vc1, double alpha, vect2_cart* vresult )
{
    vresult->x = alpha*(vc1->x);
    vresult->y = alpha*(vc1->y);
}

// Multiply a polar vector by a scalar and return the result
void vect2_scale_pol( vect2_pol* vp1, double alpha, vect2_pol* vresult )
{
    vresult->r = alpha*vp1->r;
}
