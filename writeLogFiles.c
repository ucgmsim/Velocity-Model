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



void writeSliceParametersLogFile(sliceParams *sliceParameters, modVersion modelVersion, gridStruct *location, char *outputDirectory, char *type)
{
    char sliceDir[128];
    sprintf(sliceDir,"%s/Slices",outputDirectory);
    
    struct stat st = {0};
    if (stat(sliceDir, &st) == -1)
    {
        // create output directory
        mkdir(sliceDir, 0700);
    }
    
    // generate file for writing
    FILE *fp2;
    char fName[128];
    sprintf(fName,"%s/SliceLogFile.txt",sliceDir);
    fp2 = fopen(fName,"w");
    fprintf(fp2,"Slice Parameters Log File.\n");
    fprintf(fp2,"Number_of_slices\t%i\n",sliceParameters->nSlices);
    fprintf(fp2,"Model_version\t%lf\n",modelVersion.version);

    
    if (strcmp(type,"EXTRACTED") == 0)
    {
        fprintf(fp2,"CornerLat1\t%lf\n",location->Lat[0][0]);
        fprintf(fp2,"CornerLat2\t%lf\n",location->Lat[0][location->nY-1]);
        fprintf(fp2,"CornerLat3\t%lf\n",location->Lat[location->nX-1][location->nY-1]);
        fprintf(fp2,"CornerLat4\t%lf\n",location->Lat[location->nX-1][0]);
        fprintf(fp2,"CornerLon1\t%lf\n",location->Lon[0][0]);
        fprintf(fp2,"CornerLon2\t%lf\n",location->Lon[0][location->nY-1]);
        fprintf(fp2,"CornerLon3\t%lf\n",location->Lon[location->nX-1][location->nY-1]);
        fprintf(fp2,"CornerLon4\t%lf\n",location->Lon[location->nX-1][0]);
    }
    else if (strcmp(type,"GENERATED") == 0)
    {
        
    }
    fclose(fp2);


}





void writeVeloModInputsLogFile(int argc, char *argv[])
{
    FILE *fp;
    const char *inputVector[18];
    inputVector[1] = "Sub routine name";
    inputVector[2] = "MODEL_VERSION";
    inputVector[3] = "OUTPUT_DIR";
    
    char logDir[128];
    sprintf(logDir,"%s/Log",argv[3]);

    struct stat st = {0};
    if (stat(logDir, &st) == -1)
    {
        // create output directory and the velocity model
        mkdir(logDir, 0700);
    }
    
    char logFileName[128];
    sprintf(logFileName,"%s/VelocityModelInputs.txt",logDir);
    fp = fopen(logFileName,"w");
    
    if (strcmp(argv[1], "GENERATE_VELOCITY_MOD") == 0)
    {
        inputVector[4] = "ORIGIN_LAT";
        inputVector[5] = "ORIGIN_LON";
        inputVector[6] = "ORIGIN_ROT";
        inputVector[7] = "EXTENT_X";
        inputVector[8] = "EXTENT_Y";
        inputVector[9] = "EXTENT_ZMAX";
        inputVector[10] = "EXTENT_ZMIN";
        inputVector[11] = "EXTENT_Z_SPACING";
        inputVector[12] = "EXTENT_LATLON_SPACING";
        inputVector[13] = "MIN_VS";
    }
    else if(strcmp(argv[1],"EXTRACT_VELOCITY_SLICES")== 0)
    {
        inputVector[4] = "ORIGIN_LAT";
        inputVector[5] = "ORIGIN_LON";
        inputVector[6] = "ORIGIN_ROT";
        inputVector[7] = "EXTENT_X";
        inputVector[8] = "EXTENT_Y";
        inputVector[9] = "EXTENT_ZMAX";
        inputVector[10] = "EXTENT_ZMIN";
        inputVector[11] = "EXTENT_Z_SPACING";
        inputVector[12] = "EXTENT_LATLON_SPACING";
        inputVector[13] = "SLICE_PARAMETERS_DIRECTORY";
        inputVector[14] = "MIN_VS";
    }
    else if (strcmp(argv[1], "GENERATE_VELOCITY_SLICES") == 0)
    {
        inputVector[4] = "SLICE_PARAMETERS_DIRECTORY";
        inputVector[5] = "MIN_VS";
    }
    else if (strcmp(argv[1], "GENERATE_INDIVIDUAL_PROFILE") == 0)
    {
        inputVector[4] = "PROFILE_LAT";
        inputVector[5] = "PROFILE_LON";
        inputVector[6] = "PROFILE_ZMAX";
        inputVector[7] = "PROFILE_ZMIN";
        inputVector[8] = "EXTENT_Z_SPACING";
        inputVector[8] = "MIN_VS";

    }
    else
    {
        printf("Error.\n");
    }
    
    fprintf(fp,"Code called with the following inputs.\n");
    
    for( int i = 1; i < argc; i ++)
    {
        fprintf(fp,"%s\t%s\n",inputVector[i],argv[i]);
    }

    fclose(fp);
    printf("Input file write complete.\n");
    
    
}

void writeVeloModLogFile(int argc, char *argv[],calculationLogStruct logStruct)
{
    FILE *fp;
    const char *inputVector[18];
    inputVector[1] = "Sub routine name";
    inputVector[2] = "MODEL_VERSION";
    inputVector[3] = "OUTPUT_DIR";
    
    char logDir[128];
    sprintf(logDir,"%s/Log",argv[3]);
    
    struct stat st = {0};
    if (stat(logDir, &st) == -1)
    {
        // create output directory and the velocity model
        mkdir(logDir, 0700);
    }
    
    char logFileName[128];
    sprintf(logFileName,"%s/VelocityModelCalculationLog.txt",logDir);
    fp = fopen(logFileName,"w");

    
    for( int i = 1; i < 3; i ++)
    {
        fprintf(fp,"%s\t%s\n",inputVector[i],argv[i]);
    }
    
    fprintf(fp,"%s\t%i\n","nPointsExceedingMinVelo",logStruct.nPointsExceedingMinVelo);
    

//    int nPointsExceedingMinVelo;
//    int nPointsInGlobalSurfaceExtensionZone;
//    int nPointsInBasinSurfaceExtensionZone
    
    
    fclose(fp);
    printf("Calculation log file write complete.\n");
    
    
}


