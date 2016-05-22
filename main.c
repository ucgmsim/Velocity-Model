
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


#include "constants.h"
#include "structs.h"
#include "functions.h"

//int main(int argc, char *argv[])
int main(void)
{
    ///*
     int argc = 14;
     char *argv[20];
    argv[1] = "GENERATE_VELOCITY_MOD";
    argv[2] = "0.03";
    argv[3] = "Original";
    argv[4] = "-43.6";
    argv[5] = "173.2";
    argv[6] = "-10";
    argv[7] = "70";
    argv[8] = "60";
    argv[9] = "46";
    argv[10] = "0";
    argv[11] = "1";
    argv[12] = "1";
    argv[13] = "0.5";
     //*/
    
    
    /*
    int argc = 15;
    char *argv[15];
    argv[1] = "EXTRACT_VELOCITY_SLICES";
    argv[2] = "1.64";
    argv[3] = "v1.64";
    argv[4] = "-43.6";
    argv[5] = "172.3";
    argv[6] = "-10";
    argv[7] = "70";
    argv[8] = "60";
    argv[9] = "46";
    argv[10] = "0";
    argv[11] = "0.1";
    argv[12] = "0.1";
    argv[13] = "0.5";
    argv[14] = "SliceParameters";
    */ //./CVM EXTRACT_VELOCITY_SLICES 1.64 v1.64 -43.6 172.3 -10 70 60 46 0 0.1 0.1 0.5 SliceParameters
    
    /*
    int argc = 5;
    char *argv[5];
    argv[1] = "GENERATE_VELOCITY_SLICES";
    argv[2] = "1.63";
    argv[3] = "v1.63";
    argv[4] = "SliceParametersBPV";
    */ //./CVM GENERATE_VELOCITY_SLICES 1.62 v1.62 SliceParameters
    
    
    /*
    int argc = 9;
    char *argv[9];
    argv[1] = "GENERATE_INDIVIDUAL_PROFILE";
    argv[2] = "0.63";
    argv[3] = "ProfileV0.63";
    argv[4] = "-44.00";
    argv[5] = "171.7";
    argv[6] = "1.6";
    argv[7] = "-.1";
    argv[8] = "0.003";
    */

    char *generateType = argv[1];
        
    modOrigin modelOrigin;
    modVersion modelVersion;
    char *outputDirectory;
    modExtent modelExtent;
    sliceExtent sliceBounds;
    globalDataValues *globDataVals = NULL;
    surfaceDepthsGlobal *surfDepsGlob = NULL;
    gridStruct *location = NULL;
    

    //=============================================================
    
    // Generate velocity model "GENERATE_VELOCITY_MOD" or "EXTRACT_VELOCITY_SLICES"
     
    //=============================================================
    if ((strcmp(generateType, "GENERATE_VELOCITY_MOD") == 0) || (strcmp(generateType,"EXTRACT_VELOCITY_SLICES")== 0))
    {
        if (strcmp(generateType, "GENERATE_VELOITY_MOD") == 0 && argc != 14)
        {
            printf("Incorrect number of inputs: Recieved %i, require 13.\n",argc-1);
            exit(0);
        }
        if (strcmp(generateType,"EXTRACT_VELOCITY_SLICES") == 0 && argc != 15)
        {
            printf("Incorrect number of inputs: Recieved %i, require 14.\n",argc-1);
            exit(0);
        }
        // Model Version
        char *tempPoint;
        modelVersion.version = strtod(argv[2],&tempPoint);
        printf("========================================\n");
        printf("Generating velocity model version %f.\n", modelVersion.version);
        printf("========================================\n");

        // Model origin struct
        modelOrigin.mlat = atof(argv[4]);
        modelOrigin.mlon = atof(argv[5]);
        modelOrigin.mrot = atof(argv[6]);
        
        // Model extent struct
        modelExtent.Xmax = atoi(argv[7]);
        modelExtent.Ymax = atoi(argv[8]);
        modelExtent.Zmax = atof(argv[9]); // max depth (positive downwards)
        modelExtent.Zmin = atof(argv[10]);
        modelExtent.hDep = atof(argv[11]);
        modelExtent.hLatLon = atof(argv[12]);
        
        // generate the model grid
        location = generateModelGrid(modelOrigin, modelExtent);
        
        // create directory to output files to
        outputDirectory = argv[3];
        struct stat st = {0};
        if (stat(outputDirectory, &st) == -1)
        {
            location->saveSurfaceDepths = 0;

            // create output directory and the velocity model
            mkdir(outputDirectory, 0700);
            printf("Output directory created.\n");
            
            // obtain surface filenames based off version number
            surfNames surfSubModNames;
            surfSubModNames = getSurfSubModNames(modelVersion);
            
            // determine the depths of each surface for each lat lon point
            surfDepsGlob = getSurfaceValues(location, surfSubModNames, outputDirectory);
            
            // assign values
            globDataVals = assignValues(modelVersion, location, surfSubModNames, surfDepsGlob, outputDirectory);
            
            // assign min shear wave velocity
            globDataVals->minVs = atof(argv[13]);
            // write data to file
            writeCVMData(location, globDataVals, outputDirectory);
            
            // insert values into log struct and write to file
            calculationLogStruct logStruct;
            logStruct.nPointsExceedingMinVelo = globDataVals->nPtsBelowVsThreshold;
            writeVeloModLogFile(argc, argv, logStruct);
            
            if (strcmp(generateType,"GENERATE_VELOCITY_MOD") == 0)
            {
                // write log file
                writeVeloModInputsLogFile(argc, argv);
                
                printf("GENERATE_VELO_MOD routine complete.\n");
                printf("========================================\n\n");
            }

            
        }
        else
        {
            // if the output directory exists assume velocity model exists
            printf("Output directory already exists.\n");

        }
        
        free(globDataVals);
        
        if (strcmp(generateType,"EXTRACT_VELOCITY_SLICES") == 0)
        {
            sliceParams *sliceParameters;
            sliceParameters = readExtractedSliceParametersFile(argv[14]);
            
            globalDataValues *globDataValsRead;
            globDataValsRead = loadCvmDataAll(location, outputDirectory);
            
            for( int i = 0; i < sliceParameters->nSlices; i++)
            {
                // slice extent struct
                sliceBounds.nSections = 1; // can do arbitary transects, restricted here to 1 slice between 2 points.
                sliceBounds.resXY = sliceParameters->LatLonRes[i]; // hard coded number of points to take along the slice
                sliceBounds.latPtsSlice[0] = sliceParameters->latA[i];
                sliceBounds.latPtsSlice[1] = sliceParameters->latB[i];
                sliceBounds.lonPtsSlice[0] = sliceParameters->lonA[i];
                sliceBounds.lonPtsSlice[1] = sliceParameters->lonB[i];
                
                // extract slice from data
                extractSlice(location, modelOrigin, sliceBounds, globDataValsRead, outputDirectory,i+1);
                
                sliceBounds.zMax = 1; // positive downwards
                sliceBounds.zMin = 0;
                sliceBounds.resZ = 1;
                sliceBounds.saveSlices = 0;
                generateSlice(modelOrigin, sliceBounds, modelVersion, outputDirectory);

                printf("Slice %i of %i complete.\n",i+1, sliceParameters->nSlices);
            }
            char *type = "EXTRACTED";
            writeSliceParametersLogFile(sliceParameters, modelVersion, location, outputDirectory, type);
            
            free(sliceParameters);
            free(globDataValsRead);
            
            // write log file
            writeVeloModInputsLogFile(argc, argv);

            printf("EXTRACT_VELOCITY_SLICE routine complete.\n");
            printf("========================================\n\n");
            
        }
    
        // free allocated memory
        free(surfDepsGlob);
        free(location);
        
    }
    
    //=============================================================
    
    // Generate profile "GENERATE_INDIVIDUAL_PROFILE"
    
    //=============================================================
    else if (strcmp(generateType, "GENERATE_INDIVIDUAL_PROFILE") == 0)
    {
        if (argc != 9)
        {
            printf("Incorrect number of inputs: Recieved %i, require 8.\n",argc-1);
            exit(0);
        }
        
        // Model Version
        modelVersion.version = atof(argv[2]);
        printf("========================================\n\n");
        printf("Generating velocity model version %f.\n", modelVersion.version);
        printf("========================================\n");
        
        // create directory to output files to
        outputDirectory = argv[3];
        struct stat st = {0};
        if (stat(outputDirectory, &st) == -1)
        {
            mkdir(outputDirectory, 0700);
            printf("Output directory created.\n");
        }
        else
        {
            printf("Output directory already exists.\n");
        }
        
        // Model origin struct - used as the point where the profile is desired
        modelOrigin.mlat = atof(argv[4]);
        modelOrigin.mlon = atof(argv[5]);
        modelOrigin.mrot = 0;
        
        // Model extent struct
        modelExtent.Ymax = 0.5;
        modelExtent.Xmax = 0.5;
        modelExtent.Zmax = atof(argv[6]); // max depth (positive downwards)
        modelExtent.Zmin = atof(argv[7]);
        modelExtent.hDep = atof(argv[8]);
        modelExtent.hLatLon = 1;
        
        // Generate profile
        generateProfile(modelOrigin, modelVersion, modelExtent, outputDirectory);
        
        // write log file
        writeVeloModInputsLogFile(argc, argv);
        
        printf("GENERATE_INDIVIDUAL_PROFILE routine complete.\n");
        printf("========================================\n\n");

    }
    
    
    //=============================================================
    
    // Generate high resolution velocity slice "GENERATE_VELOCITY_SLICES"
    
    //=============================================================
    else if (strcmp(generateType, "GENERATE_VELOCITY_SLICES") == 0)
    {
        if (argc != 5)
        {
            printf("Incorrect number of inputs: Recieved %i, require 4.\n",argc-1);
            exit(0);
        }
        
        // Model Version
        modelVersion.version = atof(argv[2]);
        printf("Generating velocity model version %f.\n", modelVersion.version);
        
        // create directory to output files to
        outputDirectory = argv[3];
        struct stat st = {0};
        if (stat(outputDirectory, &st) == -1)
        {
            mkdir(outputDirectory, 0700);
            printf("Output directory created.\n");
        }
        else
        {
            printf("Output directory already exists.\n");
        }
        
        sliceParams *sliceParameters;
        sliceParameters = readGeneratedSliceParametersFile(argv[4]);

        
        for( int i = 0; i < sliceParameters->nSlices; i++)
        {
            // slice extent struct
            sliceBounds.nSections = 1;
            sliceBounds.zMax = sliceParameters->depMax[i]; // positive downwards
            sliceBounds.zMin = sliceParameters->depMin[i];
            sliceBounds.resZ = (sliceParameters->depMax[i] - sliceParameters->depMin[i])/sliceParameters->DepRes[i];
            sliceBounds.resXY = sliceParameters->LatLonRes[i];
            sliceBounds.latPtsSlice[0] = sliceParameters->latA[i];
            sliceBounds.latPtsSlice[1] = sliceParameters->latB[i];
            sliceBounds.lonPtsSlice[0] = sliceParameters->lonA[i];
            sliceBounds.lonPtsSlice[1] = sliceParameters->lonB[i];
            sliceBounds.saveSlices = 1;

            
            modelOrigin.mlat = sliceBounds.latPtsSlice[0];
            modelOrigin.mlon = sliceBounds.lonPtsSlice[0];
            modelOrigin.mrot = 0;
            
            generateSlice(modelOrigin, sliceBounds, modelVersion, outputDirectory);
            
            printf("Slice %i of %i complete.\n",i+1, sliceParameters->nSlices);
        }

        char *type = "GENERATED";
        writeSliceParametersLogFile(sliceParameters, modelVersion, location, outputDirectory, type);
        
        free(sliceParameters);

        // write log file
        writeVeloModInputsLogFile(argc, argv);
        
        printf("GENERATE_VELOCITY_SLICES routine complete.\n");
        printf("========================================\n\n");

        


    }
}



