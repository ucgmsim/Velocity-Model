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


void assignQualities(global_model_parameters *GLOBAL_MODEL_PARAMETERS, velo_mod_1d_data *VELO_MOD_1D_DATA, nz_tomography_data *NZ_TOMOGRAPHY_DATA, global_surfaces *GLOBAL_SURFACES, basin_data *BASIN_DATA, mesh_vector *MESH_VECTOR,partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS,in_basin *IN_BASIN, qualities_vector *QUALITIES_VECTOR, calculation_log *CALCULATION_LOG, char *TOPO_TYPE)
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
    double dZ;
    double depthChange;

    mesh_vector *SHIFTED_MESH_VECTOR = NULL;

    if(strcmp(TOPO_TYPE, "SQUASHED") == 0)
    {
        dZ = (MESH_VECTOR->Z[0] -  MESH_VECTOR->Z[1]);

        SHIFTED_MESH_VECTOR = malloc(sizeof(mesh_vector));

        if (SHIFTED_MESH_VECTOR == NULL)
        {
            printf("Memory allocation of SHIFTED_MESH_VECTOR failed.\n");
            exit(EXIT_FAILURE);
        }
        SHIFTED_MESH_VECTOR->Lat = MESH_VECTOR->Lat;
        SHIFTED_MESH_VECTOR->Lon = MESH_VECTOR->Lon;
        SHIFTED_MESH_VECTOR->nZ = MESH_VECTOR->nZ;

        for (int k = 0; k < MESH_VECTOR->nZ; k++)
        {
            depthChange = - MESH_VECTOR->Z[k];
            SHIFTED_MESH_VECTOR->Z[k] = PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[1] - depthChange;
//            printf("%lf %lf %lf.\n",depthChange,SHIFTED_MESH_VECTOR->Z[k],MESH_VECTOR->Z[k]);
        }
        interpolateBasinSurfaceDepths(BASIN_DATA, GLOBAL_MODEL_PARAMETERS, IN_BASIN, PARTIAL_BASIN_SURFACE_DEPTHS, SHIFTED_MESH_VECTOR);

    }
    else if(strcmp(TOPO_TYPE, "SQUASHED_TAPERED") == 0)
    {
        dZ = (MESH_VECTOR->Z[0] -  MESH_VECTOR->Z[1]);
        //printf("%lf %lf %lf.\n",MESH_VECTOR->Z[0], MESH_VECTOR->Z[1],dZ);
        double TAPER_DIST = 1.0; //1.0 times the value of the DEM
        double TAPER_VAL;

        SHIFTED_MESH_VECTOR = malloc(sizeof(mesh_vector));

        if (SHIFTED_MESH_VECTOR == NULL)
        {
            printf("Memory allocation of SHIFTED_MESH_VECTOR failed.\n");
            exit(EXIT_FAILURE);
        }
        SHIFTED_MESH_VECTOR->Lat = MESH_VECTOR->Lat;
        SHIFTED_MESH_VECTOR->Lon = MESH_VECTOR->Lon;
        SHIFTED_MESH_VECTOR->nZ = MESH_VECTOR->nZ;
        for (int k = 0; k < MESH_VECTOR->nZ; k++)
        {
            depthChange = - MESH_VECTOR->Z[k];
            if(depthChange == 0)
            {
                TAPER_VAL = 1.0;
            }
            else
            {
                TAPER_VAL = 1.0 - (depthChange/(PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[1]*TAPER_DIST));
                if(TAPER_VAL < 0 )
                {
                    TAPER_VAL = 0;
                }
            }
            SHIFTED_MESH_VECTOR->Z[k] = PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[1]*TAPER_VAL - depthChange;
        }
        interpolateBasinSurfaceDepths(BASIN_DATA, GLOBAL_MODEL_PARAMETERS, IN_BASIN, PARTIAL_BASIN_SURFACE_DEPTHS, SHIFTED_MESH_VECTOR);
    }
    else if(strcmp(TOPO_TYPE, "BULLDOZED") == 0 || (strcmp(TOPO_TYPE, "TRUE") == 0))
    {
        interpolateBasinSurfaceDepths(BASIN_DATA, GLOBAL_MODEL_PARAMETERS, IN_BASIN, PARTIAL_BASIN_SURFACE_DEPTHS, MESH_VECTOR);
    }
    else
    {
        printf("User specified TOPO_TYPE not recognised, see readme.\n");
        exit(EXIT_FAILURE);
    }



    int basinFlag = 0;
    double Z = 0;

    for (int k = 0; k < MESH_VECTOR->nZ; k++)
    {
        if(strcmp(TOPO_TYPE, "BULLDOZED") == 0)
        {
            Z = MESH_VECTOR->Z[k];
        }
        else if(strcmp(TOPO_TYPE, "TRUE") == 0)
        {
            Z = MESH_VECTOR->Z[k];
        }
        else if(strcmp(TOPO_TYPE, "SQUASHED") == 0)
        {
            Z = SHIFTED_MESH_VECTOR->Z[k];
        }
        else if(strcmp(TOPO_TYPE, "SQUASHED_TAPERED") == 0)
        {
            Z = SHIFTED_MESH_VECTOR->Z[k];
        }

        for(int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
        {
            if (IN_BASIN->inBasinDep[i][k] == 1)
            {
                basinFlag = 1;
                assignBasinQualities(GLOBAL_MODEL_PARAMETERS, BASIN_DATA, PARTIAL_BASIN_SURFACE_DEPTHS, QUALITIES_VECTOR, Z, i, k);

            }
        }

        if (basinFlag == 0)
        {
            // determine which sub velocity model the point lies within
            nVeloModInd = findGlobalSubVeloModelInd(Z, PARTIAL_GLOBAL_SURFACE_DEPTHS);

            // call the respective sub velocity model

            if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[nVeloModInd], "v1DsubMod") == 0)
            {
                v1DsubMod(k, Z, QUALITIES_VECTOR, VELO_MOD_1D_DATA);
            }
            else if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[nVeloModInd], "EPtomo2010subMod") == 0)
            {
                EPtomo2010subMod(k, Z, MESH_VECTOR, QUALITIES_VECTOR, NZ_TOMOGRAPHY_DATA);
            }
            else if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[nVeloModInd], "NaNsubMod") == 0)
            {
                NaNsubMod(k, QUALITIES_VECTOR);
            }
            else if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[nVeloModInd], "GenericSubModA") == 0)
            {
                GenericSubModA(k, QUALITIES_VECTOR);
            }
            else if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[nVeloModInd], "GenericSubModB") == 0)
            {
                GenericSubModB(k, QUALITIES_VECTOR);
            }
            else if(strcmp(GLOBAL_MODEL_PARAMETERS->veloSubMod[nVeloModInd], "GenericSubModC") == 0)
            {
                GenericSubModC(k, QUALITIES_VECTOR);
            }
        }
        else if(Z > PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[1]) // if Z is above the DEM (always the surf number 1) set vals as NAN
        {
            NaNsubMod(k, QUALITIES_VECTOR);
        }
        basinFlag = 0;

    }
    if(strcmp(TOPO_TYPE, "BULLDOZED") == 0)
    {
        for (int k = 0; k < MESH_VECTOR->nZ; k++) // write over values if BULLDOZED
        {
            if (MESH_VECTOR->Z[k] > 0)
            {
                NaNsubMod(k, QUALITIES_VECTOR);
            }
        }
    }

    free(SHIFTED_MESH_VECTOR);
}


