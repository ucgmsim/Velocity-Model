//
//  basementRockSurf.c
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


surfDepValues *basementRockSurf(gridStruct *location)
/*
 Purpose:   obtain the surface data for the basesment rock
 
 Input variables:
 location  - structure containing lat lon grid
 
 Output variables:
 surfDep   - (malloc'd) pointer to structure containing the surface data
 */
{
    char *fileName = "Data/Canterbury_Basin/Pre_Quaternary/BasementTop.in";
    surfDepValues *surfDep;
    surfDep = determineSurfaceDepths(location, fileName);

    return surfDep;
    
}
