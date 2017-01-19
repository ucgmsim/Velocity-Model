//
//  writeVeloModLogFile.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 24/04/15.
//  Copyright (c) 2015 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"


calculation_log *initializeCalculationLog(int argc, char *argv[])
/*
 Purpose: to initialize the calculation log, insert values from the primary inputs etc

 Input variables:
    argc - argument count (global)
    *argv[] - argument vector (global)

 Output variables:
    *CALCULATION_LOG - struct containing calculation data and output directory (tracks various parameters for error reporting etc)

 */
{
    calculation_log *CALCULATION_LOG;
    CALCULATION_LOG = malloc(sizeof(calculation_log));
    return(CALCULATION_LOG);
}

void writeSliceParametersLogFile(char *OUTPUT_DIR, slice_parameters *SLICE_PARAMETRES,  model_extent *MODEL_EXTENT, global_mesh *GLOBAL_MESH, calculation_log *CALCULATION_LOG, char *type)
{

    // generate file for writing
    FILE *fp2;
    char fName[MAX_FILENAME_STRING_LEN];


    
    if (strcmp(type,"EXTRACTED") == 0)
    {
        sprintf(fName,"%s/Extracted_Slices/SliceLogFile.txt",OUTPUT_DIR);
        fp2 = fopen(fName,"w");
        if (fp2 == NULL)
        {
            printf("Unable to open slice log file for write.\n");
            exit(EXIT_FAILURE);
        }
        fprintf(fp2,"Slice Parameters Log File.\n");
        fprintf(fp2,"Number_of_slices\t%i\n",SLICE_PARAMETRES->nSlices);
        fprintf(fp2,"Model_version\t%s\n",MODEL_EXTENT->version);
        fprintf(fp2,"CornerLat1\t%lf\n",GLOBAL_MESH->Lat[0][0]);
        fprintf(fp2,"CornerLat2\t%lf\n",GLOBAL_MESH->Lat[0][GLOBAL_MESH->nY-1]);
        fprintf(fp2,"CornerLat3\t%lf\n",GLOBAL_MESH->Lat[GLOBAL_MESH->nX-1][GLOBAL_MESH->nY-1]);
        fprintf(fp2,"CornerLat4\t%lf\n",GLOBAL_MESH->Lat[GLOBAL_MESH->nX-1][0]);
        fprintf(fp2,"CornerLon1\t%lf\n",GLOBAL_MESH->Lon[0][0]);
        fprintf(fp2,"CornerLon2\t%lf\n",GLOBAL_MESH->Lon[0][GLOBAL_MESH->nY-1]);
        fprintf(fp2,"CornerLon3\t%lf\n",GLOBAL_MESH->Lon[GLOBAL_MESH->nX-1][GLOBAL_MESH->nY-1]);
        fprintf(fp2,"CornerLon4\t%lf\n",GLOBAL_MESH->Lon[GLOBAL_MESH->nX-1][0]);
    }
    else if (strcmp(type,"GENERATED") == 0)
    {
        sprintf(fName,"%s/Generated_Slices/SliceLogFile.txt",OUTPUT_DIR);
        fp2 = fopen(fName,"w");
        if (fp2 == NULL)
        {
            printf("Unable to open slice log file for write.\n");
            exit(EXIT_FAILURE);
        }
        fprintf(fp2,"Slice Parameters Log File.\n");
        fprintf(fp2,"Number_of_slices\t%i\n",SLICE_PARAMETRES->nSlices);
        fprintf(fp2,"Model_version\t%s\n",MODEL_EXTENT->version);
    }
    fclose(fp2);


}





//void writeVeloModInputsLogFile(int argc, char *argv[], char *OUTPUT_DIR)
//{
//    FILE *fp;
//    char logFileName[MAX_FILENAME_STRING_LEN];
//    sprintf(logFileName,"%s/Log/GlobalInputs.txt",OUTPUT_DIR);
//    fp = fopen(logFileName,"w");
//
//
//    fprintf(fp,"Code called with the following inputs.\n");
//
//    for( int i = 1; i < argc; i ++)
//    {
//        fprintf(fp,"%s\n",argv[i]);
//    }
//
//    fclose(fp);
//    printf("Input file write complete.\n");
//
//
//}

void writeVeloModCornersTextFile(global_mesh *GLOBAL_MESH, char *OUTPUT_DIR)
{
    FILE *fp;
    char logFileName[MAX_FILENAME_STRING_LEN];
    sprintf(logFileName,"%s/Log/VeloModCorners.txt",OUTPUT_DIR);
    fp = fopen(logFileName,"w");
    
    
    fprintf(fp,">Velocity model corners.\n");
    fprintf(fp,">Lon\tLat\n");
    fprintf(fp,"%lf\t%lf\n",GLOBAL_MESH->Lon[0][GLOBAL_MESH->nY-1],GLOBAL_MESH->Lat[0][GLOBAL_MESH->nY-1]);
    fprintf(fp,"%lf\t%lf\n",GLOBAL_MESH->Lon[0][0],GLOBAL_MESH->Lat[0][0]);
    fprintf(fp,"%lf\t%lf\n",GLOBAL_MESH->Lon[GLOBAL_MESH->nX-1][0],GLOBAL_MESH->Lat[GLOBAL_MESH->nX-1][0]);
    fprintf(fp,"%lf\t%lf\n",GLOBAL_MESH->Lon[GLOBAL_MESH->nX-1][GLOBAL_MESH->nY-1],GLOBAL_MESH->Lat[GLOBAL_MESH->nX-1][GLOBAL_MESH->nY-1]);

    fclose(fp);
    printf("Velocity model corners file write complete.\n");
    
    
}

//void checkVeloModInputsLogFile(int argc, char *argv[], char *OUTPUT_DIR)
//{
//    int count = 0;
//    FILE *fp;
//    char logFileName[MAX_FILENAME_STRING_LEN];
//    char line[MAX_FILENAME_STRING_LEN];
//    char tempLineCheck[MAX_FILENAME_STRING_LEN];
//    sprintf(logFileName,"%s/Log/GlobalInputs.txt",OUTPUT_DIR);
//    fp = fopen(logFileName,"r");
//    if (fp == NULL)
//    {
//        exit(EXIT_FAILURE);
//    }
//
//
//    while( fgets(line,MAX_FILENAME_STRING_LEN,fp) )
//    {
//        if (count > 2)
//        {
//            sprintf(tempLineCheck,"%s\n",argv[count]);
//
//            if (strcmp(line,tempLineCheck) != 0)
//            {
//                printf("Parameters for saved model do not match those of the model that is to be extracted from. See readme.\n");
//                exit(EXIT_FAILURE);
//            }
//
//        }
//        count += 1;
//    }
//
//    fclose(fp);
//
//}



