
//
//  readWriteInputFiles.c
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

#include "constants.h"
#include "structs.h"
#include "functions.h"


void writeSampleInputTextFiles(void)
{
    char *sampleDir = "Sample_Input_Text_Files";
    char fName[MAX_FILENAME_STRING_LEN];
    mkdir(sampleDir, 0700);
    
    
    //////////// GENERATE_VELOCITY_MOD
    FILE *fNameWrite = NULL;
    char *type = "GENERATE_VELOCITY_MOD";
    sprintf(fName,"%s/%s.txt",sampleDir,type);
    fNameWrite  = fopen(fName, "w");
    fprintf(fNameWrite,"CALL_TYPE=GENERATE_VELOCITY_MOD\n");
    fprintf(fNameWrite,"MODEL_VERSION=1.65\n");
    fprintf(fNameWrite,"OUTPUT_DIR=Full_Velo_Mod\n");
    fprintf(fNameWrite,"ORIGIN_LAT=-43.60\n");
    fprintf(fNameWrite,"ORIGIN_LON=172.30\n");
    fprintf(fNameWrite,"ORIGIN_ROT=-10\n");
    fprintf(fNameWrite,"EXTENT_X=140\n");
    fprintf(fNameWrite,"EXTENT_Y=120\n");
    fprintf(fNameWrite,"EXTENT_ZMAX=46\n");
    fprintf(fNameWrite,"EXTENT_ZMIN=0.0\n");
    fprintf(fNameWrite,"EXTENT_Z_SPACING=1.0\n");
    fprintf(fNameWrite,"EXTENT_LATLON_SPACING=1.0\n");
    fprintf(fNameWrite,"MIN_VS=0.5\n");
    fprintf(fNameWrite,"TOPO_TYPE=BULLDOZED\n");
    fclose(fNameWrite);
    
    //////////// EXTRACT_VELOCITY_SLICES
    fNameWrite = NULL;
    type = "EXTRACT_VELOCITY_SLICES";
    sprintf(fName,"%s/%s.txt",sampleDir,type);
    fNameWrite  = fopen(fName, "w");
    fprintf(fNameWrite,"CALL_TYPE=EXTRACT_VELOCITY_SLICES\n");
    fprintf(fNameWrite,"MODEL_VERSION=1.65\n");
    fprintf(fNameWrite,"OUTPUT_DIR=Full_Velo_Mod\n");
    fprintf(fNameWrite,"ORIGIN_LAT=-43.60\n");
    fprintf(fNameWrite,"ORIGIN_LON=172.30\n");
    fprintf(fNameWrite,"ORIGIN_ROT=-10\n");
    fprintf(fNameWrite,"EXTENT_X=140\n");
    fprintf(fNameWrite,"EXTENT_Y=120\n");
    fprintf(fNameWrite,"EXTENT_ZMAX=46\n");
    fprintf(fNameWrite,"EXTENT_ZMIN=0.0\n");
    fprintf(fNameWrite,"EXTENT_Z_SPACING=1.0\n");
    fprintf(fNameWrite,"EXTENT_LATLON_SPACING=1.0\n");
    fprintf(fNameWrite,"MIN_VS=0.5\n");
    fprintf(fNameWrite,"TOPO_TYPE=BULLDOZED\n");
    fprintf(fNameWrite,"EXTRACTED_SLICE_PARAMETERS_TEXTFILE=SecondaryInputFiles/SliceParametersExtracted.txt\n");
    fclose(fNameWrite);
    
    //////////// GENERATE_VELOCITY_SLICES
    fNameWrite = NULL;
    type = "GENERATE_VELOCITY_SLICES";
    sprintf(fName,"%s/%s.txt",sampleDir,type);
    fNameWrite  = fopen(fName, "w");
    fprintf(fNameWrite,"CALL_TYPE=GENERATE_VELOCITY_SLICES\n");
    fprintf(fNameWrite,"MODEL_VERSION=1.65\n");
    fprintf(fNameWrite,"OUTPUT_DIR=Generated_Slices\n");
    fprintf(fNameWrite,"GENERATED_SLICE_PARAMETERS_TEXTFILE=SecondaryInputFiles/SliceParametersGenerated.txt\n");
    fprintf(fNameWrite,"TOPO_TYPE=BULLDOZED\n");
    fclose(fNameWrite);
    
    
    
    //////////// GENERATE_PROFILE
    fNameWrite = NULL;
    type = "GENERATE_PROFILE";
    sprintf(fName,"%s/%s.txt",sampleDir,type);
    fNameWrite  = fopen(fName, "w");
    fprintf(fNameWrite,"CALL_TYPE=GENERATE_PROFILE\n");
    fprintf(fNameWrite,"MODEL_VERSION=1.65\n");
    fprintf(fNameWrite,"OUTPUT_DIR=Generated_Profile\n");
    fprintf(fNameWrite,"PROFILE_LAT=-43.60\n");
    fprintf(fNameWrite,"PROFILE_LON=172.30\n");
    fprintf(fNameWrite,"PROFILE_ZMAX=1.00\n");
    fprintf(fNameWrite,"PROFILE_ZMIN=0.00\n");
    fprintf(fNameWrite,"PROFILE_MIN_VS=0.500\n");
    fprintf(fNameWrite,"EXTENT_Z_SPACING_PROFILE=0.01\n");
    fprintf(fNameWrite,"TOPO_TYPE=BULLDOZED\n");
    fclose(fNameWrite);
    
    //////////// EXTRACT_THRESHOLD
    fNameWrite = NULL;
    type = "GENERATE_THRESHOLD";
    sprintf(fName,"%s/%s.txt",sampleDir,type);
    fNameWrite  = fopen(fName, "w");
    fprintf(fNameWrite,"CALL_TYPE=GENERATE_THRESHOLD\n");
    fprintf(fNameWrite,"MODEL_VERSION=1.65\n");
    fprintf(fNameWrite,"OUTPUT_DIR=Generated_Threshold\n");
    fprintf(fNameWrite,"ORIGIN_LAT=-43.60\n");
    fprintf(fNameWrite,"ORIGIN_LON=172.30\n");
    fprintf(fNameWrite,"ORIGIN_ROT=-10\n");
    fprintf(fNameWrite,"EXTENT_X=140\n");
    fprintf(fNameWrite,"EXTENT_Y=120\n");
    fprintf(fNameWrite,"EXTENT_LATLON_SPACING=1.00\n");
    fprintf(fNameWrite,"VS_TYPE=VS30\n");
    fclose(fNameWrite);


    //////////// GENERATE_MULTIPLE_PROFILES
    fNameWrite = NULL;
    type = "GENERATE_MULTIPLE_PROFILES";
    sprintf(fName,"%s/%s.txt",sampleDir,type);
    fNameWrite  = fopen(fName, "w");
    fprintf(fNameWrite,"CALL_TYPE=GENERATE_MULTIPLE_PROFILES\n");
    fprintf(fNameWrite,"MODEL_VERSION=1.65\n");
    fprintf(fNameWrite,"OUTPUT_DIR=Multiple_Profiles\n");
    fprintf(fNameWrite,"PROFILE_MIN_VS=0.500\n");
    fprintf(fNameWrite,"COORDINATES_TEXTFILE=SecondaryInputFiles/MultipleProfileParameters.txt\n");
    fclose(fNameWrite);

    //////////// EXTRACT_MULTIPLE_GRIDPOINT_VS
    fNameWrite = NULL;
    type = "GENERATE_VELOCITIES_ON_GRID";
    sprintf(fName,"%s/%s.txt",sampleDir,type);
    fNameWrite  = fopen(fName, "w");
    fprintf(fNameWrite,"CALL_TYPE=GENERATE_VELOCITIES_ON_GRID\n");
    fprintf(fNameWrite,"MODEL_VERSION=1.65\n");
    fprintf(fNameWrite,"OUTPUT_DIR=Gridpoint_Vs\n");
    fprintf(fNameWrite,"TOPO_TYPE=BULLDOZED\n");
    fprintf(fNameWrite,"MIN_VS=0.500\n");
    fprintf(fNameWrite,"VS_DEPTH=0.050\n");
    fprintf(fNameWrite,"COORDINATES_DIRECTORY=Gridpoint_Coords\n");
    fclose(fNameWrite);
}

char *readParameter(char *fileName, char *quality)
{
    char line[MAX_FILENAME_STRING_LEN];
    char *returnValue=(char*) malloc(MAX_FILENAME_STRING_LEN*sizeof(char));
    char *type;
    char *val;
    int valAssigned = 0;

    FILE *fNameRead = NULL;
    fNameRead  = fopen(fileName, "r");
    if (fNameRead == NULL)
    {
        printf("File not found. (%s).\n",fileName);
        exit(EXIT_FAILURE);
    }

    while (fgets (line, sizeof(line), fNameRead))
    {
        type = strtok (line,"=");
        val = strtok (NULL,"=");
        if(strcmp(type, quality) == 0)
        {
            val[strlen(val)-1] = 0;
            strcpy(returnValue,val);
//            printf("%s %s\n",type, val);
            valAssigned = 1;
            break;

        }
    }
    if (valAssigned == 0)
    {
        printf("Quality %s not set in file %s. See readme.\n",quality,fileName);
        exit(EXIT_FAILURE);
    }
    fclose(fNameRead);
    return returnValue;
}


gen_extract_velo_mod_call readGenVMInputTextFile(char *fileName)
{
    gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL;
    GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LAT = atof(readParameter(fileName,"ORIGIN_LAT"));
    GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LON = atof(readParameter(fileName,"ORIGIN_LON"));
    GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_ROT = atof(readParameter(fileName,"ORIGIN_ROT"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Y = atof(readParameter(fileName,"EXTENT_Y"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_X = atof(readParameter(fileName,"EXTENT_X"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMAX = atof(readParameter(fileName,"EXTENT_ZMAX"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMIN = atof(readParameter(fileName,"EXTENT_ZMIN"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Z_SPACING = atof(readParameter(fileName,"EXTENT_Z_SPACING"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_LATLON_SPACING = atof(readParameter(fileName,"EXTENT_LATLON_SPACING"));
    GEN_EXTRACT_VELO_MOD_CALL.MIN_VS = atof(readParameter(fileName,"MIN_VS"));
    GEN_EXTRACT_VELO_MOD_CALL.TOPO_TYPE = readParameter(fileName,"TOPO_TYPE");

    return GEN_EXTRACT_VELO_MOD_CALL;
}

gen_extract_velo_mod_call readExtractVMInputTextFile(char *fileName)
{
    gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL;
    GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LAT = atof(readParameter(fileName,"ORIGIN_LAT"));
    GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LON = atof(readParameter(fileName,"ORIGIN_LON"));
    GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_ROT = atof(readParameter(fileName,"ORIGIN_ROT"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Y = atof(readParameter(fileName,"EXTENT_Y"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_X = atof(readParameter(fileName,"EXTENT_X"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMAX = atof(readParameter(fileName,"EXTENT_ZMAX"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMIN = atof(readParameter(fileName,"EXTENT_ZMIN"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Z_SPACING = atof(readParameter(fileName,"EXTENT_Z_SPACING"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_LATLON_SPACING = atof(readParameter(fileName,"EXTENT_LATLON_SPACING"));
    GEN_EXTRACT_VELO_MOD_CALL.MIN_VS = atof(readParameter(fileName,"MIN_VS"));
    GEN_EXTRACT_VELO_MOD_CALL.TOPO_TYPE = readParameter(fileName,"TOPO_TYPE");
    GEN_EXTRACT_VELO_MOD_CALL.EXTRACTED_SLICE_PARAMETERS_TEXTFILE = readParameter(fileName,"EXTRACTED_SLICE_PARAMETERS_TEXTFILE");

    return GEN_EXTRACT_VELO_MOD_CALL;
}

gen_velo_slices_call readGenerateSliceInputTextFile(char *fileName)
{
    gen_velo_slices_call GEN_VELO_SLICES_CALL;
    GEN_VELO_SLICES_CALL.GENERATED_SLICE_PARAMETERS_TEXTFILE = readParameter(fileName,"GENERATED_SLICE_PARAMETERS_TEXTFILE");
    GEN_VELO_SLICES_CALL.TOPO_TYPE = readParameter(fileName,"TOPO_TYPE");

    return GEN_VELO_SLICES_CALL;
}

gen_profile_call readGenerateProfileInputTextFile(char *fileName)
{
    gen_profile_call GEN_PROFILE_CALL;
    GEN_PROFILE_CALL.PROFILE_LAT = atof(readParameter(fileName,"PROFILE_LAT"));
    GEN_PROFILE_CALL.PROFILE_LON = atof(readParameter(fileName,"PROFILE_LON"));
    GEN_PROFILE_CALL.PROFILE_ZMAX = atof(readParameter(fileName,"PROFILE_ZMAX"));
    GEN_PROFILE_CALL.PROFILE_ZMIN = atof(readParameter(fileName,"PROFILE_ZMIN"));
    GEN_PROFILE_CALL.PROFILE_MIN_VS = atof(readParameter(fileName,"PROFILE_MIN_VS"));
    GEN_PROFILE_CALL.EXTENT_Z_SPACING_PROFILE = atof(readParameter(fileName,"EXTENT_Z_SPACING_PROFILE"));
    GEN_PROFILE_CALL.TOPO_TYPE = readParameter(fileName,"TOPO_TYPE");

    return GEN_PROFILE_CALL;
}


gen_extract_velo_mod_call readThresholdInputTextFile(char *fileName)
{
    gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL;
    GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LAT = atof(readParameter(fileName,"ORIGIN_LAT"));
    GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LON = atof(readParameter(fileName,"ORIGIN_LON"));
    GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_ROT = atof(readParameter(fileName,"ORIGIN_ROT"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_X = atof(readParameter(fileName,"EXTENT_X"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Y = atof(readParameter(fileName,"EXTENT_Y"));
    GEN_EXTRACT_VELO_MOD_CALL.EXTENT_LATLON_SPACING = atof(readParameter(fileName,"EXTENT_LATLON_SPACING"));
    GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE = readParameter(fileName,"VS_TYPE");

    return GEN_EXTRACT_VELO_MOD_CALL;
}



gen_multi_profiles_call readGenMultiProfileInputTextFile(char *fileName)
{
    gen_multi_profiles_call GEN_MULTI_PROFILES_CALL;
    GEN_MULTI_PROFILES_CALL.COORDINATES_TEXTFILE = readParameter(fileName,"COORDINATES_TEXTFILE");
    GEN_MULTI_PROFILES_CALL.PROFILE_MIN_VS = atof(readParameter(fileName,"PROFILE_MIN_VS"));

    return GEN_MULTI_PROFILES_CALL;

}


gen_extract_multi_gridpoint_vs_call readExtractMultiInputTextFile(char *fileName)
{
    gen_extract_multi_gridpoint_vs_call GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL;
    GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL.TOPO_TYPE = readParameter(fileName, "TOPO_TYPE");
    GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL.MIN_VS = atof(readParameter(fileName, "MIN_VS"));
    GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL.VS_DEPTH = atof(readParameter(fileName, "VS_DEPTH"));
    GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL.COORDINATES_DIRECTORY = readParameter(fileName, "COORDINATES_DIRECTORY");


    return GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL;
}

multi_profile_parameters *readProfilesTextFile(char *coordsTextFile)
{
    multi_profile_parameters  *MULTI_PROFILE_PARAMETERS;
    MULTI_PROFILE_PARAMETERS = malloc(sizeof(multi_profile_parameters));
    if (MULTI_PROFILE_PARAMETERS == NULL)
    {
        printf("Memory allocation of MULTI_PROFILE_PARAMETERS failed.\n");
        exit(EXIT_FAILURE);
    }


    slice_parameters *SLICE_PARAMETERS;
    SLICE_PARAMETERS = malloc(sizeof(slice_parameters));

    FILE *file;

    file = fopen(coordsTextFile, "r");

    if (file == NULL)
    {
        printf("Profile parameters text file %s not found.\n",coordsTextFile);
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", &MULTI_PROFILE_PARAMETERS->nProfiles);

    if(MULTI_PROFILE_PARAMETERS->nProfiles>=MAX_NUM_SLICES)
    {
        printf("Number of profiles in the text file exceeds the maximum allowable value of %i.\n",MAX_NUM_GEN_MULTI_PROFILES);
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < MULTI_PROFILE_PARAMETERS->nProfiles; i++)
    {
        fscanf(file, "%lf %lf %lf %lf %lf", &MULTI_PROFILE_PARAMETERS->lats[i], &MULTI_PROFILE_PARAMETERS->lons[i], &MULTI_PROFILE_PARAMETERS->zMax[i], &MULTI_PROFILE_PARAMETERS->zMin[i], &MULTI_PROFILE_PARAMETERS->zSpacing[i]);
    }

    printf("Profiles text file read complete.\n");
    fclose(file);
    return MULTI_PROFILE_PARAMETERS;




}








