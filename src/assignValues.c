//
//  assignValues.c
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


void *assignQualities(global_model_parameters *GLOBAL_MODEL_PARAMETERS, velo_mod_1d_data *VELO_MOD_1D_DATA, nz_tomography_data *NZ_TOMOGRAPHY_DATA, global_surfaces *GLOBAL_SURFACES, basin_data *BASIN_DATA, mesh_vector *MESH_VECTOR,partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS,in_basin *IN_BASIN, qualities_vector *QUALITIES_VECTOR, calculation_log *CALCULATION_LOG)
/*
 Purpose:  to assign Vp Vs and Rho to a vector of depth points at a signle Lat Lon location
 
 Input variables:
    *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
    *VELO_MOD_1D_DATA - struct containing a 1D velocity model
    *NZ_TOMOGRAPHY_DATA - struct containing tomography sub velocity model data (tomography surfaces depths etc)
    *GLOBAL_SURFACES - struct containing pointers to global surfaces (whole domain surfaces which sub velocity models apply between)
    *BASIN_DATA - struct containing basin data (surfaces submodels etc)
    *MESH_VECTOR - struct containing a single lat lon point with one or more depths
    *CALCULATION_LOG - struct containing calculation data and output directory (tracks various parameters for error reporting etc)
 
 Output variables:
    *QUALITIES_VECTOR - struct housing Vp Vs and Rho for one Lat Lon value and one or more depths
 */
{
    
    int nVeloModInd;
    
    interpolateGlobalSurfaceDepths(GLOBAL_SURFACES, MESH_VECTOR, PARTIAL_GLOBAL_SURFACE_DEPTHS, CALCULATION_LOG);
    interpolateBasinSurfaceDepths(BASIN_DATA, GLOBAL_MODEL_PARAMETERS, IN_BASIN, PARTIAL_BASIN_SURFACE_DEPTHS, MESH_VECTOR);
    
    
    int basinFlag = 0;
    for (int k = 0; k < *MESH_VECTOR->nZ; k++)
    {
        
        for(int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
        {
            if (IN_BASIN->inBasinDep[i][k] == 1)
            {
                basinFlag = 1;
                assignBasinQualities(GLOBAL_MODEL_PARAMETERS, BASIN_DATA, PARTIAL_BASIN_SURFACE_DEPTHS, QUALITIES_VECTOR, *MESH_VECTOR->Z[k], i, k);
            }
        }
        
        if (basinFlag == 0)
        {
            // determine which sub velocity model the point lies within
            nVeloModInd = findGlobalSubVeloModelInd(*MESH_VECTOR->Z[k], PARTIAL_GLOBAL_SURFACE_DEPTHS);
            
            // call the respective sub velocity model
            if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[nVeloModInd], "v1DsubMod") == 0)
            {
                v1DsubMod(k, *MESH_VECTOR->Z[k], QUALITIES_VECTOR, VELO_MOD_1D_DATA);
            }
            else if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[nVeloModInd], "EPtomo2010subMod") == 0)
            {
                EPtomo2010subMod(k, *MESH_VECTOR->Z[k], MESH_VECTOR, QUALITIES_VECTOR, NZ_TOMOGRAPHY_DATA);
            }
            else if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[nVeloModInd], "NaNsubMod") == 0)
            {
                NaNsubMod(k, QUALITIES_VECTOR);
            }
        }
        basinFlag = 0;
        
    }
    return 0;
}


