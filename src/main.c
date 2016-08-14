
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
#include "getopt.h"

#include "constants.h"
#include "structs.h"
#include "functions.h"

//int main(int argc, char *argv[])
int main(void)
{
    /*
     int argc = 27;
     char *argv[27];
     argv[1] = "-A";
     argv[2] = "GENERATE_VELOCITY_MOD";
    argv[3] = "-B";
    argv[4] = "1.64";
    argv[5] = "-C";
    argv[6] = "v1.64_CF";
    argv[7] = "-D";
    argv[8] = "-43.65";
    argv[9] = "-E";
    argv[10] = "172.1";
    argv[11] = "-F";
    argv[12] = "-42.00";
    argv[13] = "-G";
    argv[14] = "210.";
    argv[15] = "-H";
    argv[16] = "146.";
    argv[17] = "-I";
    argv[18] = "70.";
    argv[19] = "-J";
    argv[20] = "0";
    argv[21] = "-K";
    argv[22] = "1";
    argv[23] = "-L";
    argv[24] = "1";
    argv[25] = "-M";
    argv[26] = "0.5";
     */
    
     /*
    int argc = 29;
     char *argv[29];
     argv[1] = "-A";
     argv[2] = "EXTRACT_VELOCITY_SLICES";
    argv[3] = "-B";
    argv[4] = "1.64";
    argv[5] = "-C";
    argv[6] = "v1.64_CF";
    argv[7] = "-D";
    argv[8] = "-43.65";
    argv[9] = "-E";
    argv[10] = "172.1";
    argv[11] = "-F";
    argv[12] = "-42.00";
    argv[13] = "-G";
    argv[14] = "210.";
    argv[15] = "-H";
    argv[16] = "146.";
    argv[17] = "-I";
    argv[18] = "70.";
    argv[19] = "-J";
    argv[20] = "0";
    argv[21] = "-K";
    argv[22] = "1";
    argv[23] = "-L";
    argv[24] = "1";
    argv[25] = "-M";
    argv[26] = "0.5";
     argv[27] = "-N";
     argv[28] = "ExtractedSliceParametersECVM";
     */

    
    /*
    int argc = 19;
    char *argv[19];
    argv[1] = "-A";
    argv[2] = "GENERATE_PROFILE";
    argv[3] = "-B";
    argv[4] = "Test_0.1";
    argv[5] = "-C";
    argv[6] = "CACS";
    argv[7] = "-Q";
    argv[8] = "-43.5968";
    argv[9] = "-R";
    argv[10] = "172.5";
    argv[11] = "-S";
    argv[12] = "5";
    argv[13] = "-T";
    argv[14] = "-1";
    argv[15] = "-U";
    argv[16] = "-0.5";
    argv[17] = "-V";
    argv[18] = "0.05";
     */
    
    
    ///*
    int argc = 11;
    char *argv[11];
    argv[1] = "-A";
    argv[2] = "GENERATE_VELOCITY_SLICES";
    argv[3] = "-B";
    argv[4] = "1.01";
    argv[5] = "-C";
    argv[6] = "v1.01";
    argv[7] = "-O";
    argv[8] = "0.5";
    argv[9] = "-P";
    argv[10] = "GeneratedSliceParameters";
     //*/
    
    
    
    // set call type flags to zero (0)
    int EXTRACT_VELOCITY_SLICES = 0;
    int GENERATE_VELOCITY_MOD = 0;
    int GENERATE_VELOCITY_SLICES = 0;
    int GENERATE_PROFILE = 0;
    
    // generate structs to house paramteters for each call type
    gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL;
    GEN_EXTRACT_VELO_MOD_CALL.numInputsSet = 0;
    gen_velo_slices_call GEN_VELO_SLICES_CALL;
    GEN_VELO_SLICES_CALL.numInputsSet = 0;
    gen_profile_call GEN_PROFILE_CALL;
    GEN_PROFILE_CALL.numInputsSet = 0;
    
    char *MODEL_VERSION = NULL;
    char *OUTPUT_DIR = NULL;
    char *genCallType = NULL;
//    int long_index = 0;
    int numCallTypesSet = 0;
    int opt = 0;
    
    char *optString = "A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:Q:R:S:T:U:V:";

    opt = getopt( argc, argv, optString  );
    while( opt != -1 )
    {
        switch( opt )
        {
            // call type specifiers
            case 'A':
                if(strcmp(optarg, "GENERATE_VELOCITY_MOD") == 0)
                {
                    genCallType = "GENERATE_VELOCITY_MOD";
                    GENERATE_VELOCITY_MOD = 1;
                }
                else if (strcmp(optarg, "EXTRACT_VELOCITY_SLICES") == 0)
                {
                    genCallType = "EXTRACT_VELOCITY_SLICES";
                    EXTRACT_VELOCITY_SLICES = 1;
                }
                else if (strcmp(optarg, "GENERATE_VELOCITY_SLICES") == 0)
                {
                    genCallType = "GENERATE_VELOCITY_SLICES";
                    GENERATE_VELOCITY_SLICES = 1;
                }
                else if (strcmp(optarg, "GENERATE_PROFILE") == 0)
                {
                    genCallType = "GENERATE_PROFILE";
                    GENERATE_PROFILE = 1;
                }
                else
                {
                    printf("Option -A set incorrectly, see readme.\n");
                    exit(EXIT_FAILURE);
                }
                numCallTypesSet += 1;
                break;
                
            // Required inputs
            case 'B' : MODEL_VERSION = optarg;
                break;
            case 'C' : OUTPUT_DIR = optarg;
                break;
                
            // Optional inputs (GENERATE_VELOCITY_MOD / EXTRACT_VELOCITY_SLICES)
            case 'D' : GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LAT = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'E' : GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LON = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'F' : GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_ROT = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'G' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_X = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'H' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Y = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'I' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMAX = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'J' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMIN = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'K' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Z_SPACING = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'L' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_LATLON_SPACING = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'M' : GEN_EXTRACT_VELO_MOD_CALL.MIN_VS = atof(optarg);
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
            case 'N' : GEN_EXTRACT_VELO_MOD_CALL.EXTRACTED_SLICE_PARAMETERS_DIRECTORY = optarg;
                GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
                break;
                
            // Optional inputs (GENERATE_VELOCITY_SLICES)
            case 'O' : GEN_VELO_SLICES_CALL.MIN_VS_SLICE = atof(optarg);
                GEN_VELO_SLICES_CALL.numInputsSet += 1;
                break;
            case 'P' : GEN_VELO_SLICES_CALL.GENERATED_SLICE_PARAMETERS_DIRECTORY = optarg;
                GEN_VELO_SLICES_CALL.numInputsSet += 1;
                break;

                
                // Optional inputs (GENERATE_PROFILE)
            case 'Q' : GEN_PROFILE_CALL.PROFILE_LAT = atof(optarg);
                GEN_PROFILE_CALL.numInputsSet += 1;
                break;
            case 'R' : GEN_PROFILE_CALL.PROFILE_LON = atof(optarg);
                GEN_PROFILE_CALL.numInputsSet += 1;
                break;
            case 'S' : GEN_PROFILE_CALL.PROFILE_ZMAX = atof(optarg);
                GEN_PROFILE_CALL.numInputsSet += 1;
                break;
            case 'T' : GEN_PROFILE_CALL.PROFILE_ZMIN = atof(optarg);
                GEN_PROFILE_CALL.numInputsSet += 1;
                break;
            case 'U' : GEN_PROFILE_CALL.PROFILE_MIN_VS = atof(optarg);
                GEN_PROFILE_CALL.numInputsSet += 1;
                break;
            case 'V' : GEN_PROFILE_CALL.EXTENT_Z_SPACING_PROFILE = atof(optarg);
                GEN_PROFILE_CALL.numInputsSet += 1;
                break;
                
            default:
                printf("Non existant option set, see readme.\n");
                exit(EXIT_FAILURE);
                break;
        }
         opt = getopt( argc, argv, optString );
        
    }
    
    ///*
    // check that the appropriate parameters have been set for each call type
    if ( numCallTypesSet != 1) // implies that either zero or two (or more) call types have been entered
    {
        printf("Incorrect invocation of NZVM, see readme for instructions.\n");
        exit(EXIT_FAILURE);
    }
    
    if ((MODEL_VERSION == 0) || (OUTPUT_DIR == NULL))
    {
        printf("Model version and/or output directory not set, see readme for instructions.\n");
        exit(EXIT_FAILURE);
    }
    
    if (GENERATE_VELOCITY_MOD == 1)
    {
        if(GEN_EXTRACT_VELO_MOD_CALL.numInputsSet != 10)
        {
            printf("Incorrect number of parameters set for GENERATE_VELOCITY_MOD call, see readme for instructions.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (EXTRACT_VELOCITY_SLICES == 1)
    {
        if(GEN_EXTRACT_VELO_MOD_CALL.numInputsSet != 11)
        {
            printf("Incorrect number of parameters set for EXTRACT_VELOCITY_SLICES call, see readme for instructions.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (GENERATE_VELOCITY_SLICES == 1)
    {
        if(GEN_VELO_SLICES_CALL.numInputsSet != 2)
        {
            printf("Incorrect number of parameters set for GENERATE_VELOCITY_SLICES call, see readme for instructions.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (GENERATE_PROFILE == 1)
    {
        if(GEN_PROFILE_CALL.numInputsSet != 6)
        {
            printf("Incorrect number of parameters set for GENERATE_PROFILE call, see readme for instructions.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    // check if the output directory exists, exit if necessary
    if (GENERATE_VELOCITY_MOD == 1 || GENERATE_VELOCITY_SLICES == 1 || GENERATE_PROFILE == 1)
    {
        struct stat st;
        
        if (stat(OUTPUT_DIR, &st) != -1)
        {
            printf("Output directory must not exist for this call type. See readme.\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            createAllOutputDirectories(OUTPUT_DIR, genCallType);
            writeVeloModInputsLogFile(argc, argv, OUTPUT_DIR);
        }
        
    }
    else if (EXTRACT_VELOCITY_SLICES == 1)
    {
        struct stat st;
        if (stat(OUTPUT_DIR, &st) != -1)
        {
            createAllOutputDirectories(OUTPUT_DIR, genCallType);
            // check that the parameters from model that slices are to be extracted match the input parameters from this run
            checkVeloModInputsLogFile(argc, argv, OUTPUT_DIR);
            printf("Parameters for saved model match those of the model that is to be extracted from.\n");
            
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
    
    
}


/*
 int argc = 14;
 char *argv[14];
 argv[1] = "-GENERATE_VELOCITY_MOD";
 argv[2] = "-MODEL_VERSION=1.64";
 argv[3] = "-OUTPUT_DIR=v1.64_ECVM_Local";
 argv[4] = "-ORIGIN_LAT=-43.65";
 argv[5] = "-ORIGIN_LON=172.1";;
 argv[6] = "-ORIGIN_ROT=-42.00";
 argv[7] = "-EXTENT_X=210.";
 argv[8] = "-EXTENT_Y=146.";
 argv[9] = "-EXTENT_ZMAX=70.";
 argv[10] = "-EXTENT_ZMIN=0";
 argv[11] = "-EXTENT_Z_SPACING=0.1";
 argv[12] = "-EXTENT_LATLON_SPACING=0.1";
 argv[13] = "-MIN_VS=0.5";
 */

/*
 int argc = 15;
 char *argv[15];
 argv[1] = "-EXTRACT_VELOCITY_SLICES";
 argv[2] = "-MODEL_VERSION=1.64";
 argv[3] = "-OUTPUT_DIR=v1.64_ECVM";
 argv[4] = "-ORIGIN_LAT=-43.65";
 argv[5] = "-ORIGIN_LON=172.1";;
 argv[6] = "-ORIGIN_ROT=-42.00";
 argv[7] = "-EXTENT_X=210.";
 argv[8] = "-EXTENT_Y=146.";
 argv[9] = "-EXTENT_ZMAX=70.";
 argv[10] = "-EXTENT_ZMIN=0";
 argv[11] = "-EXTENT_Z_SPACING=0.1";
 argv[12] = "-EXTENT_LATLON_SPACING=0.1";
 argv[13] = "-MIN_VS=0.5";
 argv[14] = "-EXTRACTED_SLICE_PARAMETERS_DIRECTORY=ExtractedSliceParametersECVM";
 */

/*
 int argc = 6;
 char *argv[6];
 argv[1] = "-GENERATE_VELOCITY_SLICES";
 argv[2] = "-MODEL_VERSION=1.63";
 argv[3] = "-OUTPUT_DIR=v1.63";
 argv[4] = "-MIN_VS_SLICE=0.5";
 argv[5] = "-GENERATED_SLICE_PARAMETERS_DIRECTORY=SliceParametersBPV";
 */

/*
 int argc = 10;
 char *argv[10];
 argv[1] = "-GENERATE_PROFILE";
 argv[2] = "-MODEL_VERSION=Test_0.1";
 argv[3] = "-OUTPUT_DIR=CACS";
 argv[4] = "-PROFILE_LAT=-43.5968";
 argv[5] = "-PROFILE_LON=172.5";
 argv[6] = "-PROFILE_ZMAX=5";
 argv[7] = "-PROFILE_ZMIN=-1";
 argv[8] = "-PROFILE_MIN_VS=0.5";
 argv[9] = "-EXTENT_Z_SPACING_PROFILE=0.05";
 */

/*
 // Specifying the expected options
 int opt;
 static struct option long_options[] =
 {
 // call type specifiers
 {"GENERATE_VELOCITY_MOD",   no_argument, 0,  '1' },
 {"EXTRACT_VELOCITY_SLICES",   no_argument, 0,  '2' },
 {"GENERATE_VELOCITY_SLICES",   no_argument, 0,  '3' },
 {"GENERATE_PROFILE",   no_argument, 0,  '4' },
 
 // Required inputs
 {"MODEL_VERSION",   required_argument, 0,  'a' },
 {"OUTPUT_DIR",  required_argument, 0,  'b' },
 
 // Optional inputs (GENERATE_VELOCITY_MOD / EXTRACT_VELOCITY_SLICES)
 {"ORIGIN_LAT",   required_argument, 0,  'c' },
 {"ORIGIN_LON",   required_argument, 0,  'd' },
 {"ORIGIN_ROT",   required_argument, 0,  'e' },
 {"EXTENT_X",   required_argument, 0,  'f' },
 {"EXTENT_Y",   required_argument, 0,  'g' },
 {"EXTENT_ZMAX",   required_argument, 0,  'h' },
 {"EXTENT_ZMIN",   required_argument, 0,  'i' },
 {"EXTENT_Z_SPACING",   required_argument, 0,  'j' },
 {"EXTENT_LATLON_SPACING",   required_argument, 0,  'k' },
 {"MIN_VS",   required_argument, 0,  'l' },
 {"EXTRACTED_SLICE_PARAMETERS_DIRECTORY",   required_argument, 0,  'm' },
 
 // Optional inputs (GENERATE_VELOCITY_SLICES)
 {"GENERATED_SLICE_PARAMETERS_DIRECTORY",   required_argument, 0,  'n' },
 {"MIN_VS_SLICE",   required_argument, 0,  'o' },
 
 // Optional inputs (GENERATE_PROFILE)
 {"PROFILE_LAT",   required_argument, 0,  'p' },
 {"PROFILE_LON",   required_argument, 0,  'q' },
 {"PROFILE_ZMAX",   required_argument, 0,  'r' },
 {"PROFILE_ZMIN",   required_argument, 0,  's' },
 {"PROFILE_MIN_VS",   required_argument, 0,  't' },
 {"EXTENT_Z_SPACING_PROFILE",   required_argument, 0,  'u' },
 
 {0,           0,                 0,  0   }
 };
 
 // set the options for each call type
 while ((opt = getopt_long_only(argc, argv,"",long_options, &long_index )) != -1)
 {
 switch (opt)
 {
 // call type specifiers
 case '1' : GENERATE_VELOCITY_MOD = 1;
 genCallType = "GENERATE_VELOCITY_MOD";
 numCallTypesSet += 1;
 break;
 case '2' : EXTRACT_VELOCITY_SLICES = 1;
 genCallType = "EXTRACT_VELOCITY_SLICES";
 numCallTypesSet += 1;
 break;
 case '3' : GENERATE_VELOCITY_SLICES = 1;
 genCallType = "GENERATE_VELOCITY_SLICES";
 numCallTypesSet += 1;
 break;
 case '4' : GENERATE_PROFILE = 1;
 genCallType = "GENERATE_PROFILE";
 numCallTypesSet += 1;
 break;
 
 // Required inputs
 case 'a' : MODEL_VERSION = optarg;
 break;
 case 'b' : OUTPUT_DIR = optarg;
 break;
 
 // Optional inputs (GENERATE_VELOCITY_MOD / EXTRACT_VELOCITY_SLICES)
 case 'c' : GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LAT = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'd' : GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LON = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'e' : GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_ROT = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'f' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_X = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'g' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Y = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'h' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMAX = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'i' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMIN = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'j' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Z_SPACING = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'k' : GEN_EXTRACT_VELO_MOD_CALL.EXTENT_LATLON_SPACING = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'l' : GEN_EXTRACT_VELO_MOD_CALL.MIN_VS = atof(optarg);
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 case 'm' : GEN_EXTRACT_VELO_MOD_CALL.EXTRACTED_SLICE_PARAMETERS_DIRECTORY = optarg;
 GEN_EXTRACT_VELO_MOD_CALL.numInputsSet += 1;
 break;
 
 // Optional inputs (GENERATE_VELOCITY_SLICES)
 case 'n' : GEN_VELO_SLICES_CALL.GENERATED_SLICE_PARAMETERS_DIRECTORY = optarg;
 GEN_VELO_SLICES_CALL.numInputsSet += 1;
 break;
 case 'o' : GEN_VELO_SLICES_CALL.MIN_VS_SLICE = atof(optarg);
 GEN_VELO_SLICES_CALL.numInputsSet += 1;
 break;
 
 // Optional inputs (GENERATE_PROFILE)
 case 'p' : GEN_PROFILE_CALL.PROFILE_LAT = atof(optarg);
 GEN_PROFILE_CALL.numInputsSet += 1;
 break;
 case 'q' : GEN_PROFILE_CALL.PROFILE_LON = atof(optarg);
 GEN_PROFILE_CALL.numInputsSet += 1;
 break;
 case 'r' : GEN_PROFILE_CALL.PROFILE_ZMAX = atof(optarg);
 GEN_PROFILE_CALL.numInputsSet += 1;
 break;
 case 's' : GEN_PROFILE_CALL.PROFILE_ZMIN = atof(optarg);
 GEN_PROFILE_CALL.numInputsSet += 1;
 break;
 case 't' : GEN_PROFILE_CALL.PROFILE_MIN_VS = atof(optarg);
 GEN_PROFILE_CALL.numInputsSet += 1;
 break;
 case 'u' : GEN_PROFILE_CALL.EXTENT_Z_SPACING_PROFILE = atof(optarg);
 GEN_PROFILE_CALL.numInputsSet += 1;
 break;
 default: ;
 exit(EXIT_FAILURE);
 }
 }
 */




