
//
//  main.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 19/06/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#ifndef _AIX
#include <getopt.h>
#endif

#include "constants.h"
#include "structs.h"
#include "functions.h"

int main(int argc, char *argv[])
//int main(void)
{
  //   int argc = 2;
////    int argc = 1;
     //char *argv[2];
     //argv[1] = "TOPO_SLICES_EXTRACT/GENERATE_VELOCITY_SLICES.txt";
    //char *inputFName;

    // set call type flags to zero (0)
    int EXTRACT_VELOCITY_SLICES = 0;
    int GENERATE_VELOCITY_MOD = 0;
    int GENERATE_VELOCITY_SLICES = 0;
    int GENERATE_PROFILE = 0;
    int EXTRACT_THRESHOLD = 0;
    int EXTRACT_MULTIPLE_GRIDPOINT_VS = 0;
    int GENERATE_MULTIPLE_PROFILES = 0;
    char *CALL_TYPE = NULL;
    char *MODEL_VERSION;
    char *OUTPUT_DIR;
    char *parametersTextFile = (char*) malloc(MAX_FILENAME_STRING_LEN*sizeof(char));
    char *inputFName;

    // generate structs to house paramteters for each call type
    gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL;
    gen_velo_slices_call GEN_VELO_SLICES_CALL;
    gen_profile_call GEN_PROFILE_CALL;
    gen_extract_multi_gridpoint_vs_call GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL;
    gen_multi_profiles_call GEN_MULTI_PROFILES_CALL;

    if ( argc==1 ) //if no inputs are given
    {
        printf("No input arguments given. Writing sample input text files.\n");
        writeSampleInputTextFiles();
        exit(EXIT_SUCCESS);
    }
    else if ( argc==2 )
    {
        strcpy(parametersTextFile,argv[1]);
        CALL_TYPE = readParameter(parametersTextFile,"CALL_TYPE");
        MODEL_VERSION = readParameter(parametersTextFile,"MODEL_VERSION");
        OUTPUT_DIR = readParameter(parametersTextFile,"OUTPUT_DIR");


        if(strcmp(CALL_TYPE, "GENERATE_VELOCITY_MOD") == 0)
        {
            GENERATE_VELOCITY_MOD = 1;
            GEN_EXTRACT_VELO_MOD_CALL = readGenVMInputTextFile(parametersTextFile);
            inputFName = "GENERATE_VELOCITY_MOD";
        }
        else if (strcmp(CALL_TYPE, "EXTRACT_VELOCITY_SLICES") == 0)
        {
            EXTRACT_VELOCITY_SLICES = 1;
            GEN_EXTRACT_VELO_MOD_CALL  = readExtractVMInputTextFile(parametersTextFile);
            inputFName = "EXTRACT_VELOCITY_SLICES";
        }
        else if (strcmp(CALL_TYPE, "GENERATE_VELOCITY_SLICES") == 0)
        {
            GENERATE_VELOCITY_SLICES = 1;
            GEN_VELO_SLICES_CALL = readGenerateSliceInputTextFile(parametersTextFile);
            inputFName = "GENERATE_VELOCITY_SLICES";
        }
        else if (strcmp(CALL_TYPE, "GENERATE_PROFILE") == 0)
        {
            GENERATE_PROFILE = 1;
            GEN_PROFILE_CALL = readGenerateProfileInputTextFile(parametersTextFile);
            inputFName = "GENERATE_PROFILE";

        }
        else if (strcmp(CALL_TYPE, "EXTRACT_THRESHOLD") == 0)
        {
            EXTRACT_THRESHOLD = 1;
            GEN_EXTRACT_VELO_MOD_CALL = readThresholdInputTextFile(parametersTextFile);
            inputFName = "EXTRACT_THRESHOLD";
        }
        else if (strcmp(CALL_TYPE, "GENERATE_MULTIPLE_PROFILES") == 0)
        {
            GENERATE_MULTIPLE_PROFILES = 1;
            GEN_MULTI_PROFILES_CALL = readGenMultiProfileInputTextFile(parametersTextFile);
            inputFName = "GENERATE_MULTIPLE_PROFILES";
        }
        else if (strcmp(CALL_TYPE, "EXTRACT_MULTIPLE_GRIDPOINT_VS") == 0)
        {
            EXTRACT_MULTIPLE_GRIDPOINT_VS = 1;
            GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL = readExtractMultiInputTextFile(parametersTextFile);
            inputFName = "EXTRACT_MULTIPLE_GRIDPOINT_VS";
        }


    }
    else
    {
        printf("Incorrect number of inputs given, only 1 required. See readme.\n");
        exit(EXIT_FAILURE);
    }



    
    // check if the output directory exists, exit if necessary
    if (GENERATE_VELOCITY_MOD == 1 || GENERATE_VELOCITY_SLICES == 1 || GENERATE_PROFILE == 1 || EXTRACT_THRESHOLD == 1 || EXTRACT_MULTIPLE_GRIDPOINT_VS == 1 || GENERATE_MULTIPLE_PROFILES == 1)
    {
        struct stat st;
        
        if (stat(OUTPUT_DIR, &st) != -1)
        {
            printf("Output directory must not exist for this call type. See readme.\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            createAllOutputDirectories(OUTPUT_DIR, CALL_TYPE);
        }
        
    }
    else if (EXTRACT_VELOCITY_SLICES == 1)
    {
        struct stat st;
        if (stat(OUTPUT_DIR, &st) != -1)
        {
            createAllOutputDirectories(OUTPUT_DIR, CALL_TYPE);
        }
        else
        {
            printf("Output directory must exist for this EXTRACT_VELOCITY_SLICES call type. See readme.\n");
            exit(EXIT_FAILURE);
        }
    }


    // generate the log file struct
    calculation_log *CALCULATION_LOG;
    CALCULATION_LOG = initializeCalculationLog(argc, argv);
    
    // run the routines associated with each calltype
    if (GENERATE_VELOCITY_MOD == 1)
    {
        printf("==========================================\n");
        printf("Running GENERATE_VELOCITY_MOD.\n");
        printf("==========================================\n");
        printf("Generating model version %s.\n",MODEL_VERSION);
        runGenerateVelocityModel(MODEL_VERSION, OUTPUT_DIR, GEN_EXTRACT_VELO_MOD_CALL, CALCULATION_LOG);
        printf("==========================================\n");
        printf("Completed running GENERATE_VELOCITY_MOD.\n");
        printf("==========================================\n");
    }
    else if (EXTRACT_VELOCITY_SLICES == 1)
    {
        printf("==========================================\n");
        printf("Running EXTRACT_VELOCITY_SLICES.\n");
        printf("==========================================\n");
        runExtractFromVelocityModel(MODEL_VERSION, OUTPUT_DIR,GEN_EXTRACT_VELO_MOD_CALL, CALCULATION_LOG);
        printf("==========================================\n");
        printf("Completed running EXTRACT_VELOCITY_SLICES.\n");
        printf("==========================================\n");
        
    }
    else if (GENERATE_VELOCITY_SLICES == 1)
    {
        printf("==========================================\n");
        printf("Running GENERATE_VELOCITY_SLICES.\n");
        printf("==========================================\n");
        runGenerateVelocitySlices(MODEL_VERSION, OUTPUT_DIR, GEN_VELO_SLICES_CALL, CALCULATION_LOG);
        printf("==========================================\n");
        printf("Completed running EXTRACT_VELOCITY_SLICES.\n");
        printf("==========================================\n");
    }
    else if (GENERATE_PROFILE == 1)
    {
        printf("==========================================\n");
        printf("Running GENERATE_PROFILE.\n");
        printf("==========================================\n");
        runGenerateProfile(MODEL_VERSION, OUTPUT_DIR, GEN_PROFILE_CALL, CALCULATION_LOG);
        printf("==========================================\n");
        printf("Completed running GENERATE_PROFILE.\n");
        printf("==========================================\n");
    }
    else if (EXTRACT_THRESHOLD == 1)
    {
        printf("==========================================\n");
        printf("Running EXTRACT_THRESHOLD.\n");
        printf("==========================================\n");
        runThresholdVelocityModel(MODEL_VERSION, OUTPUT_DIR, GEN_EXTRACT_VELO_MOD_CALL, CALCULATION_LOG);
        printf("==========================================\n");
        printf("Completed running EXTRACT_THRESHOLD.\n");
        printf("==========================================\n");
    }
    else if (GENERATE_MULTIPLE_PROFILES == 1)
    {
        printf("==========================================\n");
        printf("Running GENERATE_MULTIPLE_PROFILES.\n");
        printf("==========================================\n");
//        runGenerateProfile(MODEL_VERSION, OUTPUT_DIR, GEN_PROFILE_CALL, CALCULATION_LOG);
        printf("==========================================\n");
        printf("Completed running GENERATE_MULTIPLE_PROFILES.\n");
        printf("==========================================\n");
    }
    else if (EXTRACT_MULTIPLE_GRIDPOINT_VS == 1)
    {
        printf("==========================================\n");
        printf("Running EXTRACT_MULTIPLE_GRIDPOINT_VS.\n");
        printf("==========================================\n");
//        runThresholdVelocityModel(MODEL_VERSION, OUTPUT_DIR, GEN_EXTRACT_VELO_MOD_CALL, CALCULATION_LOG);
        printf("==========================================\n");
        printf("Completed running EXTRACT_MULTIPLE_GRIDPOINT_VS.\n");
        printf("==========================================\n");
    }

    /// Copy the original input file to output_directory/Log
    FILE *fp1, *fp2;
    char a;
    fp1 = fopen(parametersTextFile, "r");
    char outFileCat[MAX_FILENAME_STRING_LEN];
    printf("%s.\n",parametersTextFile);
    printf("%s.\n",inputFName);
    sprintf(outFileCat,"%s/Log/%s.txt",OUTPUT_DIR,inputFName);
    printf("%s.\n",outFileCat);
    fp2 = fopen(outFileCat, "w");
    if (fp2 == NULL)
    {
        printf("Cannot open file %c.\n",outFileCat);
        exit(EXIT_FAILURE);
    }
    do
    {
        a = fgetc(fp1);
        fputc(a, fp2);
    } while (a != EOF);
    close(fp1);
    close(fp2);




}




