//
//  v1DsubMod.c
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


void v1DsubMod(int xInd, int yInd, int zInd, globalDataValues *globalValues, gridStruct *location, velo1D *subModel1D)
/*
 Purpose:   calculate the rho vp and vs values at a single lat long point for all the depths within this velocity submodel
 
 Input variables:
 depths         - pointer to a struct containing the references to which points lie within this velocity sub-model layer
 veloSubModNum  - the reference number of this velocity sub-model
 subModel1D     - pointer to the 1D velocity sub-model data
 
 Output variables:
 values         - struct containing the vp vs and rho values for all points within this velo sub-model
 
 
 */
{
    // loop over depth values and assign qualities from the sub-model
    for(int j = 0; j < subModel1D->nDep; j++)
    {
        if(location->Z[zInd] >= subModel1D->Dep[j]*-1000) // convert to meters, -ve being downwards
        {
            globalValues->Rho[xInd][yInd][zInd] = subModel1D->Rho[j];
            globalValues->Vp[xInd][yInd][zInd] = subModel1D->Vp[j];
            globalValues->Vs[xInd][yInd][zInd] = subModel1D->Vs[j];
            break;
        }
        if(j == subModel1D->nDep)
        {
            printf("Error: Depth point below the extent represented in the 1D velocity model file.\n");
        }
    }
}

velo1D *loadv1DsubMod(char *fileName)
/*
 Purpose:   read in the 1D velocity model dataset
 
 Input variables:
 N.A.
 
 Output variables:
 subModel1D - (malloc'd) pointer to the 1D velocity sub-model data
 
 */
{
    velo1D *subModel1D = NULL;
    subModel1D = load1dVeloSubModel(fileName);
    printf("Completed Read of 1D Velocity Model Data\n");
    return subModel1D;
}

