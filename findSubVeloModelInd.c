//
//  divideDepths.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 23/06/14.
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

int findSubVeloModelInd(gridStruct *location, int lonInd, int latInd, int depInd, int nVeloSubMod, surfaceDepthsGlobal *surfDepsGlob)
/*
 Purpose:   divide the depths at a given lat lon point into processing by the various velocity sub-functions
 
 Input variables:
 location         - pointer to structure containing lat lon and dep gridpoints
 lonInd           - the indice of the longitude point
 latInd           - the indice of the latitude point
 depInd           - the indice of the depth point
 nVeloSubMod      - the number of velocity submodeld
 surfDepsGlob     - data structure containing the depths of the surfaces that divide the velocity submodels
 
 Output variables:
 nVeloInd   - the indice of the sub velocity model the point lies within
 */
{
    int nVeloInd = 1e6; // large value, sub model inds should be <10
    
    for(int j = 0; j < nVeloSubMod; j++)
    {
        if((location->Z[depInd] < surfDepsGlob->dep[j][lonInd][latInd]) && (location->Z[depInd] > surfDepsGlob->dep[j+1][lonInd][latInd])) // if point lies between adjacent
        {
            nVeloInd = j;
            break;
        }
    }
    
    if(nVeloInd == 1e6)
    {
        printf("Error.\n"); 
    }
    
    return nVeloInd;
    
}
