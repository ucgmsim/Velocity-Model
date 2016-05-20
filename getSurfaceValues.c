//
//  getSurfaceValues.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 20/06/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"

surfaceDepthsGlobal *getSurfaceValues(gridStruct *location, surfNames surfSubModNames, char *outputDirectory)
/*
 Purpose:   obtain the depths for all lat lon points for all desired surfaces
 
 Input variables:
 location        - structure containing lat lon grid
 surfSubModNames - structure containing all surface keywords
 
 Output variables:
 surfDep   - (malloc'd) pointer to structure containing surface depths for all lat lon points and surfaces
 */
{
    // place surface depths into a pointer
    surfaceDepthsGlobal *surfDepGlob;
    surfDepGlob = malloc(sizeof(surfaceDepthsGlobal));
    
    surfDepValues *surfDeps = NULL;
    for(int i = 0; i < surfSubModNames.nSurf; i++)
    {
        if(strcmp(surfSubModNames.surf[i], "posInfSurf")==0)
        {
            surfDeps = posInfSurf(location);
        }
        else if(strcmp(surfSubModNames.surf[i], "negInfSurf") == 0)
        {
            surfDeps = negInfSurf(location);
        }
        else if(strcmp(surfSubModNames.surf[i], "basementRockSurf") == 0)
        {
            surfDeps = basementRockSurf(location);
//            writeAllSurfaceDepths(location, surfDeps, outputDirectory);
        }
        else if(strcmp(surfSubModNames.surf[i], "demSurf") == 0)
        {
            char *fileName;
            fileName = "Data/DEM/DEM.in";
            surfDeps = determineSurfaceDepths(location, fileName);
        }
        else
        {
            printf("Error.\n");
        }
        
        // write individual surface depths into the global file
        for(int j = 0; j < location->nX; j++)
        {
            for(int k = 0; k < location->nY; k++)
            {
                surfDepGlob->dep[i][j][k] = surfDeps->dep[j][k];
            }
        }
        
        free(surfDeps);
    }
    
    if((location->nX == 1)&&(location->nY==1))
        // if grid only contains a single point save the surface depths to a text file
    {
        writeGlobalSurfaceDepths(surfDepGlob, location, surfSubModNames, outputDirectory);
    }
    if (location->saveSurfaceDepths == 1)
    {
        writeAllGlobalSurfaceDepths(surfDepGlob, location, surfSubModNames, outputDirectory);
    }
    printf("Depths of all surfaces at all grid points successfully calculated.\n");
    return surfDepGlob;
}

