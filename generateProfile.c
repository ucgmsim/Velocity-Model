//
//  generateProfile.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 19/12/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
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

void generateProfile(modOrigin modelOrigin, modVersion modelVersion, modExtent modelExtent, char *outputDirectory)
{
    printf("Generating model version %f.\n", modelVersion.version);
    
    // generate the model grid
    gridStruct *location = NULL;
    location = generateModelGrid(modelOrigin,modelExtent);
    
    // obtain surface filenames based off version number
    surfNames surfSubModNames;
    surfSubModNames = getSurfSubModNames(modelVersion);
    
    // determine the depths of each surface for each lat lon point
    surfaceDepthsGlobal *surfDepsGlob = NULL;
    location->saveSurfaceDepths = 0;
    surfDepsGlob = getSurfaceValues(location, surfSubModNames, outputDirectory);
    
    // assign values
    globalDataValues *globDataVals = NULL;
    globDataVals = assignValues(modelVersion, location, surfSubModNames, surfDepsGlob, outputDirectory);
    
    // write profile to file
    writeIndividualProfile(globDataVals, location, outputDirectory);
    
    // free allocated memory
    free(surfDepsGlob);
    free(globDataVals);
    free(location);
    
    
    
}

void writeBasinSurfaceDepths(globalBasinData *basinData, int basinNum, gridStruct *location, char *outputDirectory)
{
    FILE *fp;
    char fName[128];
    sprintf(fName,"%s/Basin%iSurfaceDepths.txt",outputDirectory,basinNum);
    fp = fopen(fName, "w");
    
    fprintf(fp,"Basin Surface Depths at Lat: %lf Lon: %lf\n",location->Lat[0][0], location->Lon[0][0]);
    
    for(int i = 0; i < basinData->nSurf[basinNum]; i++)
    {
        if(isnan(basinData->surfVals[basinNum][0][0][i]) !=1)
        {
            fprintf(fp,"%s \t %lf\n",basinData->surf[basinNum][i], basinData->surfVals[basinNum][0][0][i]);
        }
    }
    fclose(fp);
    printf("Completed write of surface depths at the location.\n");

}

void writeAllBasinSurfaceDepths(globalBasinData *basinData, gridStruct *location, char *outputDirectory)
{
    char sliceDir[128];
    sprintf(sliceDir,"%s/Slices",outputDirectory);
    
    struct stat st = {0};
    if (stat(sliceDir, &st) == -1)
    {
        // create output directory and the velocity model
        mkdir(sliceDir, 0700);
    }
    
    static int sliceCount = 0;
    sliceCount += 1;
    
	FILE *fp;
    char fName[64];
    sprintf(fName,"%s/SliceSurfaceDepthsBasin%i.txt",sliceDir,sliceCount);
    fp = fopen(fName, "w");
    fprintf(fp, "Lat\tLon\t");
    for(int k = 0; k < basinData->nSurf[0]; k++)
    {
        fprintf(fp, "%s\t", basinData->surf[0][k]);
    }
    fprintf(fp, "\n");
    for (int i = 0; i < location->nX; i++)
	{
		fprintf(fp, "%lf\t%lf\t", location->Lat[i][0], location->Lon[i][0]);
		for (int j = 0; j < basinData->nSurf[0]; j++) 
		{
				fprintf(fp, "%lf\t", basinData->surfVals[0][i][0][j]);
		}
		fprintf(fp, "\n");

	}
	fclose(fp);
	printf("Completed write of surface depths at the all locations.\n");

}

void writeGlobalSurfaceDepths(surfaceDepthsGlobal *surfDepGlob, gridStruct *location, surfNames surfSubModNames, char *outputDirectory)
{
    FILE *fp;
    char fName[128];
    sprintf(fName,"%s/GlobalSurfaceDepths.txt",outputDirectory);
    fp = fopen(fName, "w");
    
    fprintf(fp,"Global Surface Depths at Lat: %lf Lon: %lf\n",location->Lat[0][0], location->Lon[0][0]);
    
    for(int i = 0; i < surfSubModNames.nSurf; i++)
    {
        fprintf(fp,"%s \t %lf\n",surfSubModNames.surf[i], surfDepGlob->dep[i][0][0]);
    }
    fclose(fp);
    printf("Completed write of surface depths at the location.\n");

}


void writeAllGlobalSurfaceDepths(surfaceDepthsGlobal *surfDepGlob, gridStruct *location, surfNames surfSubModNames, char *outputDirectory)
{
    char sliceDir[128];
    sprintf(sliceDir,"%s/Slices",outputDirectory);
    
    struct stat st = {0};
    if (stat(sliceDir, &st) == -1)
    {
        // create output directory and the velocity model
        mkdir(sliceDir, 0700);
    }
    
    static int sCount = 0;
    sCount += 1;
    
    FILE *fp;
    char fName[64];
    sprintf(fName,"%s/SliceSurfaceDepthsGlobal%i.txt",sliceDir,sCount);
    fp = fopen(fName, "w");
    fprintf(fp, "Lat\tLon\t");
    for(int k = 0; k < surfSubModNames.nSurf; k++)
    {
        fprintf(fp, "%s\t", surfSubModNames.surf[k]);
    }
    fprintf(fp, "\n");
    for (int i = 0; i < location->nX; i++)
    {
        fprintf(fp, "%lf\t%lf\t", location->Lat[i][0], location->Lon[i][0]);
        for (int j = 0; j < surfSubModNames.nSurf; j++)
        {
            fprintf(fp, "%lf\t", surfDepGlob->dep[j][i][0]);
        }
        fprintf(fp, "\n");
        
    }
    fclose(fp);
    printf("Completed write of surface depths at the all locations.\n");
    
}


void writeIndividualProfile(globalDataValues *globalValues, gridStruct *location, char outputDirectory[])
{
    FILE *fp;
    char fName[64];
    sprintf(fName,"%s/ProfileAtIndividualLocation.txt",outputDirectory);
    fp = fopen(fName, "w");
    fprintf(fp,"Properties at Lat: %lf Lon: %lf\n",location->Lat[0][0], location->Lon[0][0]);
    fprintf(fp,"Depth \t Vp \t Vs \t Rho\n");

    
    for(int i = 0; i < location->nZ; i++)
    {
        fprintf(fp,"%lf \t %lf \t %lf \t %lf\n",location->Z[i], globalValues->Vp[0][0][i],globalValues->Vs[0][0][i],globalValues->Rho[0][0][i]);
    }
    fclose(fp);
    printf("Completed write of profiles at the location\n");

}


