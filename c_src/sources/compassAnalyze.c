/**
 * \file compassAnalyze.c
 * \brief Programme qui detecte les problemes de GPS
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include  "compassAnalyze.h"
#include <cJSON.h>
/**
 * \fn int compassAnalyze(magptr, magerr , *MAG)
 * \brief Fonction qui detecte les probleme de compass et le stocke dans un tableau magerr
 * 
 * \param[in] magptr le tableau de double qui contient les valeurs de mag
 * \param[in/out] magerr est le tableau qui contiendra les valeurs des erreurs
 * \param[in] ligne nombre de ligne
 *
 * \return 0 s'il n'y a pas d'erreur.
 */
int compassAnalyze(double **magptr, double **magerr, int ligne)
{
  printf("\n--------------Analyzing Compass-----------------\n");
  if(ligne!=0){                                                                                    //Verify if there's MAG in log
    double mag_field;   //holds the result
    int i;
    int c=0;
    for(i=0;i<ligne;i++){
		magerr[i] = NULL ;
        mag_field = sqrt(pow(magptr[i][0],2)+pow(magptr[i][1],2)+pow(magptr[i][2],2));     //formula to get mag_field = sqrt(MagX^2+MagY^2+MagZ^2)
        if(mag_field<120 || mag_field>550){                                               //mag_field E [120;550]
            magerr[i]=magptr[i];
            c++;
        }
    }
    printf(">>>>>>>>>\t Number of compass errors: %d\n",c);
    if(c>0) return 1;                                                                               //return 1 if error detected
  }else{printf(">>>>>>>>>\t No MAG attribute in these log file\n");}
  return 0;
}

/**
 * \fn storeerrorCompass(char* where ,double** magerr , int size ,cJSON* problem)
 * \brief Fonction qui convertit le gpserr en csv
 * 
 * \param where		Le nom de reportoire qu'on enrigestre le fichier csv
 * \param magerr	Le tableau qui contiendra les valeurs des erreurs
 * \param size		Le nombre de lignes
 * \param thetime Le Time dans le logfile
 * \param problem	Objet JSON qui decrit rencontrer dans ce type de log
 */
void storeerrorCompass(char* where ,double** magerr , int size ,char * thetime,cJSON* problem){
	
	char name[100];
	
	FILE *csv_file = NULL;
	int files = 0 ;
	int i;
	int started = 0 ;
	
	for (i=0;i<size;i++)
	{
		if (started){
			
			if (magerr[i]) {
				fprintf(csv_file,"%f,%f,%f,%f\n",  magerr[i][0],magerr[i][1],magerr[i][2],magerr[i][3]);
			}else {
				started = 0 ;
				fclose(csv_file);
			}
		}
		else {
			
			if (magerr[i]){
				i-- ;
				
				
				sprintf(name,"%s/compass_%d.csv",where,files);
				
				csv_file=fopen(name,"w");
				
				fprintf(csv_file,"%s,MagX,MagY,MagZ\n",thetime);
				
				started = 1 ;
							
				files++ ;
				
			}
			
			
		}
		
		
	}
		
	cJSON_AddItemToArray(cJSON_GetObjectItem(problem,"nbfiles") , cJSON_CreateNumber((double)files));
	
}
