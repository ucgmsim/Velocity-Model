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


void v1DsubMod(int zInd, double dep, qualities_vector *QUALITIES_VECTOR, velo_mod_1d_data *VELO_MOD_1D_DATA)
/*
 Purpose:   calculate the rho vp and vs values at a single lat long depth point
 
 Input variables:
 zInd - the indicie of the grid point to store the data at
 dep - the depth of the grid point of interest
 *QUALITIES_VECTOR - struct housing Vp Vs and Rho for one Lat Lon value and one or more depths
 *VELO_MOD_1D_DATA - struct containing a 1D velocity model

 Output variables:
 n.a.
 */
{
    const double eps = 1e-6; // Tolerance for floating-point comparison to handle precision errors
    int found = 0; // Flag to track if a matching model depth is found

    // Loop over depth values to find the first model depth that matches or is below the input depth
    for (int j = 0; j < VELO_MOD_1D_DATA->nDep; j++)
    {
        // Convert model depth from positive kilometers to negative meters (downward)
        double model_depth = VELO_MOD_1D_DATA->Dep[j] * -1000;
        // Check if input depth (with tolerance) is greater than or equal to model depth
        // Adding eps ensures floating-point errors (e.g., -100.000000000001 vs. -100) don't skip the intended match
        // This modifies original strict comparison (dep >= model_depth) to handle precision issues
        if (dep + eps >= model_depth)
        {
            // Assign rho, vp, and vs values from the velocity model to the qualities vector
            QUALITIES_VECTOR->Rho[zInd] = VELO_MOD_1D_DATA->Rho[j];
            QUALITIES_VECTOR->Vp[zInd] = VELO_MOD_1D_DATA->Vp[j];
            QUALITIES_VECTOR->Vs[zInd] = VELO_MOD_1D_DATA->Vs[j];
            found = 1; // Mark that a match was found
            break; // Exit loop after finding the first matching depth
        }
    }

    // If no matching depth was found, log an error (e.g., dep is below the deepest model depth)
    if (!found)
    {
        fprintf(stderr, "Error: Depth point %g below the extent represented in the 1D velocity model file.\n", dep);
    }
}



void load1dVeloSubModel(char *fileName, velo_mod_1d_data *VELO_MOD_1D_DATA)
/*
 Purpose:   load a 1D velocity submodel into memory
 
 Input variables:
 *fileName  - pointer to the filename to open and read
 
 Output variables:
 *VELO_MOD_1D_DATA - struct containing a 1D velocity model
 */
{
    FILE *file;
    
    char fNameDir[MAX_FILENAME_STRING_LEN];
    sprintf(fNameDir,"Data/1D_Velocity_Model/%s",fileName);
    
    file = fopen(fNameDir, "r");
    if (file == NULL)
    {
        printf("Error 1D velocity model file %s not found.\n",fileName);
        exit(EXIT_FAILURE);
    }
    
    char tempA[10], tempB[10];
    fscanf(file, "%s %s", tempA, tempB); // header line (discard)
    int i = 0;
    while(!feof(file))
    {
        fscanf(file, "%lf %lf %lf %s %s %lf", &VELO_MOD_1D_DATA->Vp[i], &VELO_MOD_1D_DATA->Vs[i], &VELO_MOD_1D_DATA->Rho[i], tempA, tempB, &VELO_MOD_1D_DATA->Dep[i]);
        i += 1;
    }
    VELO_MOD_1D_DATA->nDep = i;
    fclose(file);

}


