//
//  posInfSurf.c
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


surfDepValues *posInfSurf(gridStruct *location)
/*
 Purpose:   assign the upper limit surface for all lat lon values
 
 Input variables:
 location  - structure containing lat lon grid
 
 Output variables:
 surfDep   - structure containing the upper limit positive surface depth
 */
{
    surfDepValues *surfDep;
    surfDep = malloc(sizeof(surfDepValues));
    double infValue = 100000; // positive value in m
    // assign a constant large value for the entire surface
    for(int i = 0; i < location->nX; i++)
    {
        for(int j = 0; j < location->nY; j++)
        {
            surfDep->dep[i][j] = infValue;
        }
    }
    return surfDep;
    
}
