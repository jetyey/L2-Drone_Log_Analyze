/**
 * \file vibrationAnalyze.c
 * \brief Programme qui detecte les problemes de vibration
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "vibrationAnalyze.h"
#include <cJSON.h>
/**
 * \fn int vibrationAnalyze(imuptr, imuerr, *IMU)
 * \brief Fonction qui detecte les probleme de vibration et le stocke dans un tableau imuerr
 * 
 * \param[in] imuptr Tableau de double qui contient les valeurs de gps
 * \param[in,out] imuerr1 Le tableau qui contiendra les valeurs des erreurs
 * \param[in,out] imuerr2 Lle tableau qui contiendra les valeurs des erreurs
 * \param[in,out] imuerr3 Le tableau qui contiendra les valeurs des erreurs
 * \param[in] imu1 Indiquer la presence d'un erreur
 * \param[in] imu2 Indiquer la presence d'un erreur
 * \param[in] imu3 Indiquer la presence d'un erreur
 * \param[in] ligne Nombre de ligne
 *
 * \return 0 s'il n'y a pas d'erreur.
 */
int vibrationAnalyze(double **imuptr,  double **imuerr1,double **imuerr2,double **imuerr3,int* imu1 ,int* imu2 ,int* imu3 , int ligne)
{
    printf("\n--------------Analyzing high vibration problems-----------------\n");
    if(ligne!=0)                                                                                 //Verify if there's IMU in log
    {
        int line;
        int c=0;                       //counter of error
        for(line=0;line<ligne;line++){
            if(imuptr[line][4]<(-3) || imuptr[line][4]>3 )            //condition 1
			{   
				imuerr1[line] = imuptr[line];
				*imu1 = 1;
                c++;
            }
			if (imuptr[line][5]<(-3) || imuptr[line][5]>3)
			{   
				imuerr2[line] = imuptr[line];
				*imu2 = 1;
                c++;
            }
			if (imuptr[line][6]<(-15) || imuptr[line][6]>5)
			{   
				imuerr3[line] = imuptr[line];
				*imu3 = 1;
                c++;
            }
        }
        printf(">>>>>>>>>\t Number of vibration errors: %d\n",c);
        if(c>0) return 1;                                                             //return 1 if error detected
   }else{printf(">>>>>>>>>\t No IMU attribute in these log file\n");}
   return 0;
}

/**
 * \fn storeerrorVib(char* where ,double** imuerr, char *whichmec, int size ,cJSON* problem)
 * \brief Fonction qui convertit imuerr en csv
 * \param where Le nom de reportoire où on enrigestre le fichier csv
 * \param imuerr Le tableau qui contiendra les valeurs des erreurs
 * \param whichmec Pour identifier quel probleme est detecté parmi les 2
 * \param size Nombre de lignes
 * \param thetime Le Time dans le logfile
 * \param problem Objet JSON qui decrit renconté dans ce type de lo
 */
void storeerrorVib(char* where ,double** imuerr ,char* whichmec, int size ,char* thetime ,cJSON* problem){
	
	char name[100];
	
	FILE *csv_file = NULL;
	int files = 0 ;
	int i;
	int started = 0 ;
	
	for (i=0;i<size;i++)
	{
		if (started){
			
			if (imuerr[i]) {
				fprintf(csv_file,"%f,%f,%f,%f\n",  imuerr[i][0],imuerr[i][4],
																imuerr[i][5],imuerr[i][6]);
			}else {
				started = 0 ;
				fclose(csv_file);
			}
		}
		else {
			
			if (imuerr[i]){
				i-- ;
				
				
				sprintf(name,"%s/%s_%d.csv",where,whichmec,files);
				
				csv_file=fopen(name,"w");
				
				fprintf(csv_file,"%s,AccX,Acc,AccZ\n",thetime);
				
				started = 1 ;
							
				files++ ;
				
			}
			
			
		}
		
		
	}

	cJSON_AddItemToArray(cJSON_GetObjectItem(problem,"nbfiles") , cJSON_CreateNumber((double)files));

}
