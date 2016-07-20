//
//  loadAllData.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 20/10/15.
//  Copyright (c) 2015 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "constants.h"
#include "structs.h"
#include "functions.h"


void loadAllGlobalData(global_model_parameters *GLOBAL_MODEL_PARAMETERS,calculation_log *CALCULATION_LOG, velo_mod_1d_data *VELO_MOD_1D_DATA, nz_tomography_data *NZ_TOMOGRAPHY_DATA, global_surfaces *GLOBAL_SURFACES, basin_data *BASIN_DATA)
/*
 Purpose: load all data required to generate the velocity model, global surfaces, sub velocity models and all basin data
 
 Input variables:
    *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
    *CALCULATION_LOG - struct containing calculation data and output directory (tracks various parameters for error reporting etc)
    *VELO_MOD_1D_DATA - struct containing a 1D velocity model
    *NZ_TOMOGRAPHY_DATA - struct containing tomography sub velocity model data (tomography surfaces depths etc)
    *GLOBAL_SURFACES - struct containing pointers to global surfaces (whole domain surfaces which sub velocity models apply between)
    *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 
 Output variables:
    n.a.
 */
{
    // read in sub velocity models
    printf("Loading global velocity submodel data.\n");
    for( int i = 0; i < GLOBAL_MODEL_PARAMETERS->nVeloSubMod; i++)
    {
        if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[i], "v1DsubMod") == 0)
        {
            load1dVeloSubModel(GLOBAL_MODEL_PARAMETERS->veloMod1dFileName[0], VELO_MOD_1D_DATA);
            printf("Loaded 1D velocity model data.\n");
        }
        else if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[i], "EPtomo2010subMod") == 0)
        {
            loadEPtomoSurfaceData(GLOBAL_MODEL_PARAMETERS->tomographyName, NZ_TOMOGRAPHY_DATA );
            printf("Loaded tomography data.\n");
        }
        else if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[i], "NaNsubMod") == 0)
        {
            // no data required for NaN velocity sub model, leave as placeholder
        }
    }
    printf("Completed loading of global velocity submodel data.\n");

    // read in global surfaces
    loadGlobalSurfaceData(GLOBAL_SURFACES, GLOBAL_MODEL_PARAMETERS);
    printf("Completed loading of global surfaces.\n");

    // read in basin surfaces and boundaries
    printf("Loading basin data.\n");
    loadBasinData(BASIN_DATA, GLOBAL_MODEL_PARAMETERS);
    printf("Completed loading basin data.\n");
    printf("All global data loaded.\n");
    
}


