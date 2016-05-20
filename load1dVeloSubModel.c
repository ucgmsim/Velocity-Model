//
//  load1dVeloSubModel.c
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

velo1D *load1dVeloSubModel(char *fileName)
/*
 Purpose:   load a 1D velocity submodel into memory
 
 Input variables:
 *fileName  - pointer to the filename to open and read
 
 Output variables:
 velo1D     - (malloc'd) pointer to the velocity sub-model
 */
{
    velo1D *veloMod1D = NULL;
    veloMod1D = malloc(sizeof(velo1D));
    FILE *file;
    file = fopen(fileName, "r");
    char tempA[10], tempB[10];
    fscanf(file, "%s %s", tempA, tempB); // header line (discard)
    int i = 0;
    while(!feof(file))
    {
        fscanf(file, "%lf %lf %lf %s %s %lf", &veloMod1D->Vp[i], &veloMod1D->Vs[i], &veloMod1D->Rho[i], tempA, tempB, &veloMod1D->Dep[i]);
        i += 1;
    }
    veloMod1D->nDep = i;
    fclose(file);
    return veloMod1D;
}
