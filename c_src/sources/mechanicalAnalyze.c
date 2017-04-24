/**
 * \file mechanicalAnalyze.c
 * \brief Programme qui detecte les problemes de GPS
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>
#include "mechanicalAnalyze.h"
/**
 * \fn int mechanicalAnalyze(attptr,atterr, *ATT)
 * \brief Fonction qui detecte les probleme mecanique et le stocke dans un tableau atterr
 * 
 * \param[in] attptr le tableau de double qui contient les valeurs de att
 * \param[in/out] atterr1 est le tableau qui contiendra les valeurs des erreurs
 * \param[in/out] atterr2 est le tableau qui contiendra les valeurs des erreurs
 * \param[in/out] atterr3 est le tableau qui contiendra les valeurs des erreurs
 * \param[in] at1 pour indiquer la presence d'un erreur
 * \param[in] at2 pour indiquer la presence d'un erreur
 * \param[in] at3 pour indiquer la presence d'un erreur
 * \param[in] ligne nombre de ligne
 *
 * \return 0 s'il n'y a pas d'erreur.
 */
int mechanicalAnalyze(double **attptr, double **atterr1, double **atterr2,double **atterr3, int* at1 , int* at2 ,int* at3,int ligne)
{
    printf("\n--------------Analyzing Mechanical problems-----------------\n");
    if(ligne!=0){
        int i,c=0;
        for(i=0;i<ligne;i++){

            if(attptr[i][1]-attptr[i][2]>10  || attptr[i][1]-attptr[i][2]< -10) 
			{
				atterr1[i] = attptr[i];
				c++;
				*at1 = 1 ;
			}
			if(attptr[i][3]-attptr[i][4]>10  || attptr[i][3]-attptr[i][4] < -10) 
			{
				atterr2[i] = attptr[i];
				c++;
				*at2 = 1 ;
			}
			if(attptr[i][5]-attptr[i][6]>15  || attptr[i][5]-attptr[i][6] < -15 ) 
			{
				atterr3[i] = attptr[i];
				c++;
				*at3 = 1 ;
				
			}
			
        }
        printf(">>>>>>>>>\t Number of mechanical problems detected: %d\n",c);
        if(c>0) return 1;
    }else{printf(">>>>>>>>>\t No ATT attribute in these log file\n");}
    return 0;
}

/**
 * \fn storeerrorMec(char* where ,double** atterr, char* whichmec, int size ,cJSON* problem)
 * \brief Fonction qui convertit le gpserr en csv
 * 
 * \param where		Le nom de reportoire qu'on enrigestre le fichier csv
 * \param atterr	Le tableau qui contiendra les valeurs des erreurs
 * \param whichmec 	Pour identifier quel probleme est detecté parmi les 3
 * \param size		Le nombre de lignes
 * \param thetime Le Time dans le logfile
 * \param problem	Objet JSON qui decrit rencontrer dans ce type de log
 */
void storeerrorMec(char* where ,double** atterr , char* whichmec ,int size , char* thetime , cJSON* problem){
	
	char name[100];
	
	FILE *csv_file = NULL;
	int files = 0 ;
	int i;
	int started = 0 ;
	
	for (i=0;i<size;i++)
	{
		if (started){
			
			if (atterr[i]) {
				fprintf(csv_file,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",  atterr[i][0],atterr[i][1],atterr[i][2],
															atterr[i][3],atterr[i][4],
															atterr[i][5],atterr[i][6],atterr[i][7],atterr[i][8]);
			}else {
				started = 0 ;
				fclose(csv_file);
			}
		}
		else {
			
			if (atterr[i]){
				i-- ;
				
				
				sprintf(name,"%s/%s_%d.csv",where,whichmec,files);
				
				csv_file=fopen(name,"w");
				
				fprintf(csv_file,"%s,DesRoll,Roll,DesPitch,Pitch,DesYaw,Yaw\n",thetime);
				
				started = 1 ;
							
				files++ ;
				
			}
			
			
		}
		
		
	}
	cJSON_AddItemToArray(cJSON_GetObjectItem(problem,"nbfiles") , cJSON_CreateNumber((double)files));
}
