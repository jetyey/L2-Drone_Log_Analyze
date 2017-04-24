/**
 * \file compassoffsetAnalyze.c
 * \brief Programme qui detecte les problemes de GPS
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "compassoffsetAnalyze.h"
/**
 * \fn int compassoffsetAnalyze(magptr, magerr  ,*MAG)
 * \brief Fonction qui detecte les probleme de compassoffset et le stocke dans un tableau magerr
 * 
 * \param[in] magptr le tableau de double qui contient les valeurs de mag
 * \param[in/out] magerr1 est le tableau qui contiendra les valeurs des erreurs
 * \param[in/out] magerr2 est le tableau qui contiendra les valeurs des erreurs
 * \param[in/out] magerr3 est le tableau qui contiendra les valeurs des erreurs
 * \param[in] mag1 pour indiquer la presence d'un erreur
 * \param[in] mag2 pour indiquer la presence d'un erreur
 * \param[in] mag3 pour indiquer la presence d'un erreur
 * \param[in] ligne nombre de ligne
 *
 * \return 0 s'il n'y a pas d'erreur.
 */
int compassoffsetAnalyze(double **magptr, double **magerr1,double **magerr2,double **magerr3,int* mag1,int* mag2 , int* mag3, int ligne)
{
    printf("\n--------------Analyzing Compass offset-----------------\n");
    if(ligne!=0)                                                                                 //Verify if there's IMU in log
    {
        int line;
        int c=0;                       //counter of error
        for(line=0;line<ligne;line++){
            if(magptr[line][4]<(-150) || magptr[line][4]>150 ) {            //condition 1
                magerr1[line] = magptr[line];
                c++;
				*mag1 = 1 ;
            }
			if( magptr[line][5]<(-150) || magptr[line][5]>150 ){
				
				magerr2[line] = magptr[line];
                c++;
				*mag2 = 1 ;
				
			}
			if (magptr[line][6]<(-150) || magptr[line][6]>150) {
				
				magerr3[line] = magptr[line];
                c++;
				*mag3 = 1 ;
				
			}
        }
        printf(">>>>>>>>>\t Number of vibration errors: %d\n",c);
        if(c>0) return 1;                                                             //return 1 if error detected
   }else{printf(">>>>>>>>>\t No IMU attribute in these log file\n");}
   return 0;
}

/**
 * \fn int compassAnalyze(magptr, magerr , *MAG)
 * \brief Fonction qui detecte les probleme de compass et le stocke dans un tableau magerr
 * 
 * \param[in] magptr le tableau de double qui contient les valeurs de mag
 *
 * \return 0 s'il n'y a pas d'erreur.
 */	
int compassoffsetAnalyze2(double **parmptr)
{
    printf("\n--------------Analyzing Compass offset-----------------\n");
    int line = 0 ;
   
	if(parmptr[line][0]<(-150) || parmptr[line][0]>150){              //condition 1
		printf("Error offset COMPASS_OFS_X\n");
		printf(">>>>>>>>>\t Detected Compass offset problem\n");
        return 1;
	}
	if(parmptr[line][1]<(-150) || parmptr[line][1]>150){              //condition 2
		printf("Error offset COMPASS_OFS_Y\n");
		printf(">>>>>>>>>\t Detected Compass offset problem\n");
        return 1;
	}
	if(parmptr[line][2]<(-150) || parmptr[line][2]>150){             //condition 3
		printf("Error offset COMPASS_OFS_Z\n");
		printf(">>>>>>>>>\t Detected Compass offset problem\n");
        return 1;
	}
   
	
	printf(">>>>>>>>>\t No detected Compass offset problem\n");
	return 0;
    
}

/**
 * \fn storeerrorCompOffset(char* where ,double** magerr, char *whichmec, int size ,cJSON* problem)
 * \brief Fonction qui convertit le gpserr en csv
 * 
 * \param where		Le nom de reportoire qu'on enrigestre le fichier csv
 * \param magerr	Le tableau qui contiendra les valeurs des erreurs
 * \param whichmec 	Pour identifier quel probleme est detecté parmi les 2
 * \param size		Le nombre de lignes
 * \param thetime Le Time dans le logfile
 * \param problem	Objet JSON qui decrit rencontrer dans ce type de log
 */
void storeerrorCompOffset(char* where ,double** magerr ,char* whichmec, int size ,char* thetime , cJSON* problem){
	
	char name[100];
	
	FILE *csv_file = NULL;
	int files = 0 ;
	int i;
	int started = 0 ;
	
	for (i=0;i<size;i++)
	{
		if (started){
			
			if (magerr[i]) {
				fprintf(csv_file,"%f,%f,%f,%f\n",  magerr[i][0],magerr[i][4],magerr[i][5],magerr[i][6]);
			}else {
				started = 0 ;
				fclose(csv_file);
			}
		}
		else {
			
			if (magerr[i]){
				i-- ;
				
				
				sprintf(name,"%s/%s_%d.csv",where,whichmec,files);
				
				csv_file=fopen(name,"w");
				
				fprintf(csv_file,"%s,OfsX,OfsY,OfsZ\n", thetime);
				
				started = 1 ;
							
				files++ ;
				
			}
			
			
		}
		
		
	}
		
	cJSON_AddItemToArray(cJSON_GetObjectItem(problem,"nbfiles") , cJSON_CreateNumber((double)files));
	
}
