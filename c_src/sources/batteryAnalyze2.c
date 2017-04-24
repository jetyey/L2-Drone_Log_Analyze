/**
 * \file batteryAnalyze2.c
 * \brief Programme qui detecte les problemes de GPS
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include  "batteryAnalyze2.h"
/**
 * \fn int batteryAnalyze2(currptr,currerr, *CURR)
 * \brief Fonction qui detecte les probleme de l'interference de voltage et le stocke dans un tableau currerr
 * 
 * \param[in] currptr le tableau de double qui contient les valeurs de curr
 * \param[in/out] currerr est le tableau qui contiendra les valeurs des erreurs
 * \param[in] ligne nombre de ligne
 *
 * \return 0 s'il n'y a pas d'erreur.
 */
int batteryAnalyze2(double **currptr, double **currerr, int ligne)
{
    printf("\n--------------Analyzing Voltage problems-----------------\n");
    if(ligne!=0){
        int i,j,k;
        double somme,min=0,max=0;
        float average;
        int quotient= ligne/20;
        for(i=0;i<quotient;i++){
            somme=0;
        	for(j=i*20;j<i*20+20;j++){
            	somme+=currptr[j][5];
				currerr[j] = NULL ;
        	}
        	average=somme/20;
            if(min==0 && max==0){
                min=max=average;
            }
            min = (average < min ) ? average : min;
        	max = (average > max ) ? average : max;
        	if(max-min > 150){
        	    for(k=j-20;k<j;k++){
                    currerr[k] = currptr[k];
        	    }
        		return 1;
        	}
        }
        printf(">>>>>>>>>\t No Power Interferances detected\n");
        return 0;
    }else{printf(">>>>>>>>>\t No CURR attribute in these log file\n");}
    return 0;
}

/**
 * \fn storeerrorBat2(char* where ,double** currerr , int size ,cJSON* problem)
 * \brief Fonction qui convertit le currerr en csv
 * 
 * \param where		Le nom de reportoire qu'on enrigestre le fichier csv
 * \param currerr	Le tableau qui contiendra les valeurs des erreurs
 * \param size		Le nombre de lignes
 * \param thetime Le Time dans le logfile
 * \param problem	Objet JSON qui decrit rencontrer dans ce type de log
 */
void storeerrorBat2(char* where ,double** currerr , int size ,char* thetime,cJSON* problem){
	
	char name[100];
	
	FILE *csv_file = NULL;
	int files = 0 ;
	int i;
	int started = 0 ;
	
	for (i=0;i<size;i++)
	{
		if (started){
			
			if (currerr[i]) {
				
				fprintf(csv_file,"%f,%f\n",  currerr[i][0],currerr[i][5]);
			}else {
				started = 0 ;
				fclose(csv_file);
			
				break;
			}
		}
		else {
			
			if (currerr[i]){
				i-- ;
				
				
				sprintf(name,"%s/alimentation2_%d.csv",where,files);
				
				csv_file=fopen(name,"w");
				
				fprintf(csv_file,"%s,Vcc\n",thetime);
				
				started = 1 ;
							
				files++ ;
				
			}
			
			
		}
		
		
	}
		
	cJSON_AddItemToArray(cJSON_GetObjectItem(problem,"nbfiles") , cJSON_CreateNumber((double)files));
	
}
