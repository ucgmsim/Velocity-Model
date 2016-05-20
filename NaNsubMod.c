//
//  NaNsubMod.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 4/08/14.
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

void NaNsubMod(int xInd, int yInd, int zInd, globalDataValues *golbalValues)
/*
 Purpose:   set all points above the ground surface to NaNs
 
 Input variables:
 depths         - pointer to a struct containing the references to which points lie within this velocity sub-model layer
 veloSubModNum  - the reference number of this velocity sub-model
 
 Output variables:
 values         - struct containing the vp vs and rho values for all points within this velo sub-model
 
 */
{
    golbalValues->Rho[xInd][yInd][zInd] = NAN;
    golbalValues->Vp[xInd][yInd][zInd] = NAN;
    golbalValues->Vs[xInd][yInd][zInd] = NAN;
}

