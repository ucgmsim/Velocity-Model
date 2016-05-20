//
//  loadBoundary.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 3/08/14.
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

void loadBoundary(globalBasinData *basinData, int basinNum)
/*
 Purpose:   load a boundary file into memory
 
 */
{
    for (int i = 0; i < basinData->nBoundaries[basinNum]; i++)
    {
        FILE *file;
        file = fopen(basinData->boundaryFileName[basinNum][i], "r");
        
        int count = 0;
        basinData->minLon[basinNum][i] = 180; // starting points to refine the encompassing rectangle
        basinData->maxLon[basinNum][i] = -180;
        basinData->minLat[basinNum][i] = 180;
        basinData->maxLat[basinNum][i] = -180;
        
        while(!feof(file))
        {
            fscanf(file, "%lf %lf", &basinData->boundaryLon[basinNum][i][count], &basinData->boundaryLat[basinNum][i][count]);
            
            basinData->minLon[basinNum][i] = fmin(basinData->minLon[basinNum][i],basinData->boundaryLon[basinNum][i][count]);
            basinData->minLat[basinNum][i] = fmin(basinData->minLat[basinNum][i],basinData->boundaryLat[basinNum][i][count]);
            
            basinData->maxLon[basinNum][i] = fmax(basinData->maxLon[basinNum][i],basinData->boundaryLon[basinNum][i][count]);
            basinData->maxLat[basinNum][i] = fmax(basinData->maxLat[basinNum][i],basinData->boundaryLat[basinNum][i][count]);
            
            count += 1;
        }
        fclose(file);
        basinData->boundaryNumPoints[basinNum][i] = count;
        assert(count<=MAX_DIM_BOUNDARY_FILE);
    }
}