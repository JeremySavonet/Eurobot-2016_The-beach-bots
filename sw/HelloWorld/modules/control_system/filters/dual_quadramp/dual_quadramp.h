/*
 * dual_quadramp.h
 *
 * Author: Antoine Albertelli (CVRA)
 */

#ifndef _DUAL_QUADRAMP_H_
#define _DUAL_QUADRAMP_H_

#include "../quadramp/quadramp.h"

// Struct for quadramp filter in two dimensions.
struct dual_quadramp_filter 
{
    struct quadramp_filter x_quadramp;  // Filter for the x-axis 
    struct quadramp_filter y_quadramp;  // Filter for the y-axis
    int32_t x_pos;                      // Current position
    int32_t y_pos;                      // Current position
    int32_t x_consign;                  
    int32_t y_consign;                  
    int32_t speed;                      // Max. speed 
    int32_t acc;                        // Max. acceleration 
};

/*
 * Initialise le filtre.
 *
 * Ce filtre sert a deplacer un point dans un plan en respectant une certaine 
 * contrainte de vitesse et d'acceleration, ce qui permet par exemple 
 * de deplacer la consigne d'un bras sans perdre les objets.
 */
void dual_quadramp_init( struct dual_quadramp_filter *q );

/*
 * Fait une iteration du filtre.
 *
 * La methode utilisee pour le calcul des 2 rampes est simplement 
 * de mettre une vitesse et une acceleration proportionnelle a la distance a 
 * parcourir sur chaque axe, pour que les 2 axes arrivent a 
 * destination en meme temps.
 */
void dual_quadramp_do_filter( struct dual_quadramp_filter *q );

/*
 * Recupere la position.
 *
 * Recupere la position de consigne de chaque axe, sans appeler le filtre.
 */
void dual_quadramp_get_pos( struct dual_quadramp_filter *q, 
                            int32_t *x, 
                            int32_t *y );

/*
 * Fixe une position pour les axes.
 *
 * equivalent de quadramp_set_position mais en 2D
 */
void dual_quadramp_set_pos( struct dual_quadramp_filter *q, 
                            int32_t x, 
                            int32_t y );

// Regle l'acceleration et la vitesse. 
void dual_quadramp_set_acc_and_speed( struct dual_quadramp_filter *q, 
                                      int32_t speed, 
                                      int32_t acc );

#endif // _DUAL_QUADRAMP_H_
