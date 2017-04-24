/**
 * \file arrayallocation.c
 * \brief Programme qui detecte les problemes de GPS
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "arrayallocation.h"
/**
 * \fn double** arrayallocation( int taille)
 * \brief Fonction qui alloue des espaces memoires
 * 
 * \param[in] taille est le nombre de ligne de tableau
 * \return un espace en memoire alloué
 */
double** arrayallocation( int taille){
  return (double**)malloc(sizeof(double*)*(taille));
  
}
