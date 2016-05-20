//
//  generateSurfaceVectors.c
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

surfVec *generateSurfaceVectors(surfRead *surfStruct)
/*
 Purpose:   generate the n * 3 surface vector for a given surface
 
 Input variables:
 surfStruct  - structure containing surface data
 
 Output variables:
 surfVec     - (malloc'd) pointer to n * 3 surface vector
 
 */
{
    surfVec *Vector;
    Vector = malloc(sizeof(surfVec));
    assert((surfStruct->nLat*surfStruct->nLon)<=SURF_VEC_DIM_MAX);
    int ind = 0;
    for(int i = 0; i < surfStruct->nLon; i++)
    {
        for(int j = 0; j < surfStruct->nLat; j++)
        {
            ind = i*surfStruct->nLat+j;
            Vector->Vec[ind][0] = surfStruct->lati[j]; // col 0 represents the latitude value
            Vector->Vec[ind][1] = surfStruct->loni[i]; // col 1 represents the longitude value
            Vector->Vec[ind][2] = surfStruct->raster[j][i]; // col 2 represents the value for the surface for a lat lon val
        }
    }
    Vector->lengthCount = ind + 1;
    return Vector;
}
