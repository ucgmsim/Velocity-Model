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

void NaNsubMod(int zInd, qualities_vector *QUALITIES_VECTOR)
/*
 Purpose:   set all points above the ground surface to NaNs
 
 Input variables:
 depths         - pointer to a struct containing the references to which points lie within this velocity sub-model layer
 veloSubModNum  - the reference number of this velocity sub-model
 
 Output variables:
 values         - struct containing the vp vs and rho values for all points within this velo sub-model
 
 */
{
    QUALITIES_VECTOR->Rho[zInd] = NAN;
    QUALITIES_VECTOR->Vp[zInd] = NAN;
    QUALITIES_VECTOR->Vs[zInd] = NAN;
}

void GenericSubModA(int zInd, qualities_vector *QUALITIES_VECTOR)
{
    QUALITIES_VECTOR->Rho[zInd] = 1;
    QUALITIES_VECTOR->Vp[zInd] = 1;
    QUALITIES_VECTOR->Vs[zInd] = 1;
}

void GenericSubModB(int zInd, qualities_vector *QUALITIES_VECTOR)
{
    QUALITIES_VECTOR->Rho[zInd] = 2;
    QUALITIES_VECTOR->Vp[zInd] = 2;
    QUALITIES_VECTOR->Vs[zInd] = 2;
}

void GenericSubModC(int zInd, qualities_vector *QUALITIES_VECTOR)
{
    QUALITIES_VECTOR->Rho[zInd] = 3;
    QUALITIES_VECTOR->Vp[zInd] = 3;
    QUALITIES_VECTOR->Vs[zInd] = 3;
}


