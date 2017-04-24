/**
 * \file count.c
 * \brief Programme qui compte le nombre de ligne pour chaque parametre.
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "count.h"

/**
 * \fn void count(file, ATT, IMU, CURR, MAG, GPS, att, imu, curr, mag, gps)
 * \brief Fonction qui compte le nombre de ligne pour chaque parametre.
 * 
 * \param file est le fichier a lire
 * \param ATT est un pointeur int qui contient le nombres de ligne pour chaque parametre
 * \param IMU est un pointeur int qui contient le nombres de ligne pour chaque parametre
 * \param CURR est un pointeur int qui contient le nombres de ligne pour chaque parametre
 * \param MAG est un pointeur int qui contient le nombres de ligne pour chaque parametre
 * \param GPS est un pointeur int qui contient le nombres de ligne pour chaque parametre
 * \param att est un tableau qui contient le mots à lire
 * \param imu est un tableau qui contient le mots à lire
 * \param curr est un tableau qui contient le mots à lire
 * \param mag est un tableau qui contient le mots à lire
 * \param gps est un tableau qui contient le mots à lire
 */
void count(FILE *fichier, int *ATT, int *IMU, int *CURR, int *MAG, int *GPS,
                          char *att, char *imu, char *curr, char *mag, char *gps)
{

	char line[100]; 	//array containing line of the file
	char *token;		//pointeur token to each token of line
    int linenmb=0;      //row counter
	//WHILE: Read file until end of the file
	while(!feof(fichier))
	{
		fgets(line, sizeof(line), fichier);
        linenmb++;
		//Get token
		token = strtok(line,", ");	/*read string until ',' and turn it into token*/

        //IF: Search desired attribute
        if(strcmp(att,token)==0)
        {
            ++(*ATT);
        }else  if(strcmp(imu,token)==0)
        {
            ++(*IMU);
        }else  if(strcmp(curr,token)==0)
        {
            ++(*CURR);
        }else if(strcmp(mag,token)==0)
        {
            ++(*MAG);
        }else if(strcmp(gps,token)==0)
            ++(*GPS);
    }
    printf("\nNumber of lines in the log file: %d\n",linenmb);
	fclose(fichier);
}




