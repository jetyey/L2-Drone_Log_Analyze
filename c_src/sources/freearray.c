/**
 * \file freearray.c
 * \brief Programme qui libere l'espace memoire alloué pour les tableaux 2D
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "freearray.h"
/**
 * \fn void freearray(double **arrayptr, int ligne)
 * \brief Fonction qui libere l'espace memoire alloué pour les tableaux 2D
 * 
 * \param arrayptr		Pointeur vers un tableau d'un paramtre
 * \param ligne		 	Le nombre de ligne de tableau
 */
void freearray(double **arrayptr, int ligne) {
    int i;
    //FOR: Pass through every table created
    for (i = 0; i < ligne; ++i) {
        free(arrayptr[i]);
    }
    free(arrayptr); //free pointer
}
