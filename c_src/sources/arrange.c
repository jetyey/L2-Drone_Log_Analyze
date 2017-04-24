/**
 * \file arrange.c
 * \brief Programme qui range les valeurs de parametre dans leur tableau en memeoire correspondant.
 * \author DE JESUS
 * \version 1.0
 * \date 19 avril 2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arrange.h"

/**
 * \fn void arrange(file, attptr, imuptr, currptr, magptr, gpsptr, parmptr, att, imu, curr, mag, gps)
 * \brief Fonction qui range les valeurs de parametre dans leur tableau en memeoire correspondant
 * 
 * \param[in] fichier est le fichier a lire,
 * \param[in/out] attptr est un pointeur int qui contient l'adresse du tableau en memoire
 * \param[in/out] imuptr est un pointeur int qui contient l'adresse du tableau en memoire
 * \param[in/out] currptr est un pointeur int qui contient l'adresse du tableau en memoire
 * \param[in/out] magptr est un pointeur int qui contient l'adresse du tableau en memoire
 * \param[in/out] gpsptr est un pointeur int qui contient l'adresse du tableau en memoire
 * \param[in/out] parmptr est un pointeur int qui contient l'adresse du tableau en memoire
 * \param[in] att est un tableau qui contient le mots à lire
 * \param[in] imu est un tableau qui contient le mots à lire
 * \param[in] curr est un tableau qui contient le mots à lire
 * \param[in] mag est un tableau qui contient le mots à lire
 * \param[in] gps est un tableau qui contient le mots à lire 
 */
void arrange(FILE *fichier, double **attptr, double **imuptr, double **currptr, double **magptr, double **gpsptr, double **parmptr,
                         char *att, char *imu, char *curr, char *mag, char *gps)
{

	char line[100]; 	//array containing line of the file
	char *token, *token0, *token1, *token2, *token3;		//pointer token to each token of line, token0/1/2/3 are for PARM
	char parm[5]="PARM";
	char learn[15]="COMPASS_LEARN";
	char ofsx[15]="COMPASS_OFS_X";
	char ofsy[15]="COMPASS_OFS_Y";
	char ofsz[15]="COMPASS_OFS_Z";
	//Row counter
    int rowATT = 0;
    int rowIMU = 0;
    int rowCURR = 0;
    int rowMAG = 0;
    int rowGPS = 0;

	//WHILE: Read file until end of the file
	while(!feof(fichier))
	{
		fgets(line, sizeof(line), fichier);     /*get line of file*/

		//get token
		token = strtok(line,",");	/*read string until ',' and turn it into token*/

        //Create line and column when ATT is read
        if(strcmp(att,token)==0)
        {
            attptr[rowATT] = (double*) malloc(sizeof(double)*10);  //create line
            //WHILE: Tokenize data
            while(token!=NULL)
            {
                //DO: Get tokens corresponding to ATT needed
                int column = 0 ;
                do
                {
                    token=strtok(NULL,",");
                    if(token!=NULL)
                    {
                        attptr[rowATT][column] =strtof(token, NULL);  //insert value in double
                        //printf(" %f",attptr[rowATT][column]);
                        column++ ;                                    //Increment column
                    }

                }while(token != NULL);
                //printf("\n");
                ++rowATT ;           //Increment row
            }token=strtok(NULL,", \n");
        }
         //Create line and column when IMU is read
        else if(strcmp(imu,token)==0)
        {
            imuptr[rowIMU] = (double*) malloc(sizeof(double)*13);  //create line
            //WHILE: Tokenize data
            while(token!=NULL)
            {
                //DO: Get tokens corresponding to ATT needed
                int column = 0 ;
                do
                {
                    token=strtok(NULL,",");
                    if(token!=NULL)
                    {
                        imuptr[rowIMU][column] =strtof(token, NULL);  //insert value in double
                        //printf(" %f",imuptr[rowIMU][column]);
                        column++ ;                                    //Increment column
                    }

                }while(token != NULL);
                //printf("\n");
                ++rowIMU ;      //Increment row
            }token=strtok(NULL,", \n");
        }
         //Create line and column when CURR is read
        else if(strcmp(curr,token)==0)
        {
            currptr[rowCURR] = (double*) malloc(sizeof(double)*7);  //create line
            //WHILE: Tokenize data
            while(token!=NULL)
            {
                //DO: Get tokens corresponding to ATT needed
                int column = 0 ;
                do
                {
                    token=strtok(NULL,",");
                    if(token!=NULL)
                    {
                        currptr[rowCURR][column] =strtof(token, NULL);  //insert value in double
                        //printf(" %f",currptr[rowCURR][column]);
                        column++ ;                                      //Increment column
                    }

                }while(token != NULL);
                //printf("\n");
                ++rowCURR;      //Increment row
            }token=strtok(NULL,", \n");
        }
         //Create line and column when MAG is read
        else if(strcmp(mag,token)==0)
        {
            magptr[rowMAG] = (double*) malloc(sizeof(double)*12);  //create line
            //WHILE: Tokenize data
            while(token!=NULL)
            {
                //DO: Get tokens corresponding to ATT needed
                int column = 0 ;
                do
                {
                    token=strtok(NULL,",");
                    if(token!=NULL)
                    {
                        magptr[rowMAG][column] =strtof(token, NULL);  //insert value in double
                        //printf(" %f",currptr[rowMAG][column]);
                        column++ ;                                   //Increment column
                    }
                }while(token != NULL);
                //printf("\n");
                ++rowMAG;           //Increment row
            }token=strtok(NULL,", \n");
        }
         //Create line and column when GPS is read
        else if(strcmp(gps,token)==0)
        {
            gpsptr[rowGPS] = (double*) malloc(sizeof(double)*14);  //create line
            //WHILE: Tokenize data
            while(token!=NULL)
            {
                //DO: Get tokens corresponding to ATT needed
                int column = 0 ;
                do
                {
                    token=strtok(NULL,", ");
                    if(token!=NULL)
                    {
                        gpsptr[rowGPS][column] =strtof(token, NULL);  //insert value in double
                        //printf(" %f",gpsptr[rowGPS][column]);
                        column++ ;                                    //Increment column
                    }

                }while(token != NULL);
              //  printf("\n");
                ++rowGPS;              //Increment row
            }token=strtok(NULL,", \n");
			
        }
        //Create table 1D with 4 columns
        else if(strcmp(parm,token)==0)
        {
            parmptr[0]=(double*)malloc(sizeof(double));
            token=strtok(NULL,", ");
			token=strtok(NULL,", ");
            //Get the value of COMPASS_LEARN
            if(strcmp(learn,token)==0)
            {
                token0=strtok(NULL,",");
                parmptr[0][0]=strtof(token0, NULL);
                //printf("Learn: %.0f\n",parmptr[0][0]);
            }
            //IF: COMPASS_LEARN==0 no need to analyze ofsx,ofsy and ofsz of every MAG
            if(parmptr[0][0]==0)
            {
                parmptr[0]=(double*)realloc(parmptr[0],5*sizeof(double));
                if(strcmp(ofsx,token)==0)
                {
                    token1=strtok(NULL,",");
                    parmptr[0][1]=strtof(token1, NULL);
                    //printf("COMPASS_OFS_X: %.4f\n",parmptr[0][1]);
                }
                if(strcmp(ofsy,token)==0)
                {
                    token2=strtok(NULL,",");
                    parmptr[0][2]=strtof(token2, NULL);
                    //printf("COMPASS_OFS_Y: %.4f\n",parmptr[0][2]);
                }
                if(strcmp(ofsz,token)==0)
                {
                    token3=strtok(NULL,",");
                    parmptr[0][3]=strtof(token3, NULL);
                    //printf("COMPASS_OFS_Z: %.4f\n",parmptr[0][3]);
                }
            }
        }
    }
	
    fclose(fichier);
    printf("Done reading\n");
}


