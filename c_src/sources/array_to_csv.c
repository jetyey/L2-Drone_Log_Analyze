/**
 * \file array_to_csv.c
 * \brief Programme qui detecte les problemes de GPS
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <string.h>

/* corriger les parametres de la fonction*/
/**
 * \fn void array_to_csv(char *file,char* abscisse,int abscisse_p , char* ordonne ,int ordonne_p , double **array,int lines)
 * \brief Fonction qui creer le fichier csv
 * \param file		 	Le fichier a lire
 * \param abscisse		Portera le nom, ici soit TimeMs
 * \param abscisse_p	La position de parametre dans le tableau attfields
 * \param ordonne		Portera la valeur où il y avait des problemes
 * \param ordonne_p		L'incrementation de i
 * \param array			Tableaux contenant les parametres
 * \param lines	 		Le nombre de ligne du tableau
 *
 */
void array_to_csv(char *file,char* abscisse,int abscisse_p , char* ordonne ,int ordonne_p , double **array,int lines)
{

FILE *csv_file;

int i;

// Ouverture du fichier en ecriture
// CSV du ROLL
csv_file=fopen(file,"w+");
// commencer par ecrire le nom des deux cases du tableau
fprintf(csv_file,"%s,%s\n",abscisse,ordonne);
//Répéter l'écriture des valeurs de "value1" et "value2" autant de fois que le nombre de lignes du tableau.
	
   for (i=0;i<lines;i++)
	{
        fprintf(csv_file,"%f,%f\n",array[i][abscisse_p], array[i][ordonne_p]);
		
		
     }
	
fclose(csv_file);
// On ferme le fichier
//printf("\n %s cree avec succes !",file);

}
