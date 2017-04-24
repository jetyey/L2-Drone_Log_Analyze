#ifndef COMPASSANALYZE_H_INCLUDED
#define COMPASSANALYZE_H_INCLUDED
#include <cJSON.h>
void storeerrorCompass(char*  ,double** , int ,char* ,cJSON*);
int compassAnalyze(double **,  double **, int);

#endif // COMPASSANALYZE_H_INCLUDED
