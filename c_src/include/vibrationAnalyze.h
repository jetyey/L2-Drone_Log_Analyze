#ifndef VIBRATIONANALYZE_H_INCLUDED
#define VIBRATIONANALYZE_H_INCLUDED
#include <cJSON.h>
void storeerrorVib(char*  ,double** ,char*, int ,char*,cJSON*);
int vibrationAnalyze(double **, double **,double** ,double** ,int*,int*,int*, int );

#endif // VIBRATIONANALYZE_H_INCLUDED
