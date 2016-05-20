//
//  loadSurface.c
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

surfRead *loadSurface(char *fileName)
/*
 Purpose:   load a surface into memory
 
 Input variables:
 *fileName  - pointer to the filename to open and read
 
 Output variables:
 surfRead  - (malloc'd) pointer to loaded surface data
 */
{
    double firstLat;
    double firstLon;
    double lastLat;
    double lastLon;
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL) {
    	perror("Error");
    }
    surfRead *surfData;
    surfData = malloc(sizeof(surfRead));
    
    int nLat, nLon;
    // read and assign the number of latitude and longitude values
    fscanf(file, "%d %d", &nLat, &nLon);
    surfData->nLat = nLat;
    surfData->nLon = nLon;
    
    // assure the preallocation of the surface struct is not exceeded
    assert(nLat<=SURF_IN_DIM_MAX);
    assert(nLon<=SURF_IN_DIM_MAX);
    
    // read lat values
    for(int i = 0; i < nLat; i++)
    {
        fscanf(file, "%lf",&surfData->lati[i]);
    }
    
    // read lon values
    for(int i = 0; i < nLon; i++)
    {
        fscanf(file, "%lf",&surfData->loni[i]);
    }
    
    // read in the surface raster data
    for(int i = 0; i < nLat; i++)
    {
        for(int j = 0; j < nLon; j++)
        {
            fscanf(file, "%lf",&surfData->raster[j][i]);
        }
    }
    
    fclose(file);
    
    firstLat = surfData->lati[0];
    lastLat = surfData->lati[surfData->nLat-1];
    
    if (firstLat >= lastLat)
    {
        surfData->maxLat = firstLat;
        surfData->minLat = lastLat;
    }
    else if (lastLat >= firstLat)
    {
        surfData->maxLat = lastLat;
        surfData->minLat = firstLat;
    }
    else
    {
        printf("Error.\n");
    }
    
    firstLon = surfData->loni[0];
    lastLon = surfData->loni[surfData->nLon-1];

    if (firstLon >= lastLon)
    {
        surfData->maxLon = firstLon;
        surfData->minLon = lastLon;
    }
    else if (lastLon >= firstLon)
    {
        surfData->maxLon = lastLon;
        surfData->minLon = firstLon;
    }
    else
    {
        printf("Error.\n");
    }
//    printf("%lf %lf\n",surfData->minLon,surfData->maxLon);
//    printf("%lf %lf\n",surfData->minLat,surfData->maxLat);
    
    return surfData;
    
}
