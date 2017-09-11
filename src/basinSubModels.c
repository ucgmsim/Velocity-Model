//
//  basinSubModels.c
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


void assignBasinQualities(global_model_parameters *GLOBAL_MODEL_PARAMETERS, basin_data *BASIN_DATA, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, qualities_vector *QUALITIES_VECTOR, double depth, int basinNum, int zInd)
/*
 Purpose: assign Vp Vs and Rho to the individual grid point (determined by finding the surfaces adjacent to the grid point then using applicable sub-velocity model)
 
 Input variables:
    *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
    *BASIN_DATA - struct containing basin data (surfaces submodels etc)
    *PARTIAL_BASIN_SURFACE_DEPTHS - struct containing depths for all applicable basin surfaces at one lat - lon location
    *QUALITIES_VECTOR - struct housing Vp Vs and Rho for one Lat Lon value and one or more depths
    depth - the depth of the gridpoint to determine the properties at
    basinNum - the basin number pertaining to the basin of interest
    zInd - the depth indice of the single grid point (related to storing Vp Vs and Rho values in QUALITIES_VECTOR)
 
 Output variables:
    n.a.
 */
{
    // determine submodelind
    int indAbove, indBelow;
    indAbove = determineBasinSurfaceAbove(GLOBAL_MODEL_PARAMETERS, PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    indBelow = determineBasinSurfaceBelow(GLOBAL_MODEL_PARAMETERS, PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    
//    printf("%i %i %i %lf.\n",indAbove, basinNum, zInd, depth);
    callBasinSubVelocityModels(GLOBAL_MODEL_PARAMETERS, BASIN_DATA, PARTIAL_BASIN_SURFACE_DEPTHS, QUALITIES_VECTOR,depth, indAbove, basinNum, zInd);
    
}

void callBasinSubVelocityModels(global_model_parameters *GLOBAL_MODEL_PARAMETERS, basin_data *BASIN_DATA, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, qualities_vector *QUALITIES_VECTOR, double depth, int basinSubModelInd, int basinNum, int zInd)
/*
 Purpose: assign Vp Vs and Rho to the individual grid point (by calling on the appropriate sub-velocity model)
 
 Input variables:
    *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
    *BASIN_DATA - struct containing basin data (surfaces submodels etc)
    *PARTIAL_BASIN_SURFACE_DEPTHS - struct containing depths for all applicable basin surfaces at one lat - lon location
    *QUALITIES_VECTOR - struct housing Vp Vs and Rho for one Lat Lon value and one or more depths
    depth - the depth of the gridpoint to determine the properties at
    basinSubModelInd - the indicie of the basin sub velocity model to be used
    basinNum - the basin number pertaining to the basin of interest
    zInd - the depth indice of the single grid point (related to storing Vp Vs and Rho values in QUALITIES_VECTOR)
 
 Output variables:
    n.a.
 */
{
    // 1D sub models
    if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "Cant1D_v1") == 0)
    {
        v1DsubMod(zInd, depth, QUALITIES_VECTOR, &BASIN_DATA->BASIN_SUBMODEL_DATA[basinNum].VELO_MOD_1D_DATA);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "Cant1D_v2") == 0)
    {
        v1DsubMod(zInd, depth, QUALITIES_VECTOR, &BASIN_DATA->BASIN_SUBMODEL_DATA[basinNum].VELO_MOD_1D_DATA);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "Cant1D_v2_Pliocene_Enforced") == 0)
    {
        v1DsubMod(zInd, depth, QUALITIES_VECTOR, &BASIN_DATA->BASIN_SUBMODEL_DATA[basinNum].VELO_MOD_1D_DATA);
    }
    
    // Pre-quaternary models
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "PaleogeneSubMod_v1") == 0)
    {
        paleogeneSubModelv1(zInd, QUALITIES_VECTOR);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "MioceneSubMod_v1") == 0)
    {
        mioceneSubModelv1(zInd, QUALITIES_VECTOR);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "PlioceneSubMod_v1") == 0)
    {
        plioceneSubModelv1(zInd, QUALITIES_VECTOR);
    }
    
    
    // BPV models
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "BPVSubMod_v1") == 0)
    {
        BPVSubModelv1(zInd, QUALITIES_VECTOR);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "BPVSubMod_v2") == 0)
    {
        BPVSubModelv2(zInd, QUALITIES_VECTOR);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "BPVSubMod_v3") == 0)
    {
        BPVSubModelv3(zInd, QUALITIES_VECTOR, PARTIAL_BASIN_SURFACE_DEPTHS, basinNum, depth);
    }
    
    // Quaternary models
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "ChristchurchSubMod_v1") == 0)
    {
        marineSubModel(zInd, QUALITIES_VECTOR,PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "RiccartonSubMod_v1") == 0)
    {
        gravelSubModel(zInd, QUALITIES_VECTOR,PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "BromleySubMod_v1") == 0)
    {
        marineSubModel(zInd, QUALITIES_VECTOR,PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "LinwoodSubMod_v1") == 0)
    {
        gravelSubModel(zInd, QUALITIES_VECTOR,PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "HeathcoteSubMod_v1") == 0)
    {
        marineSubModel(zInd, QUALITIES_VECTOR,PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "BurwoodSubMod_v1") == 0)
    {
        gravelSubModel(zInd, QUALITIES_VECTOR,PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "ShirleySubMod_v1") == 0)
    {
        marineSubModel(zInd, QUALITIES_VECTOR,PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    }
    else if(strcmp(GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd], "WainoniSubMod_v1") == 0)
    {
        gravelSubModel(zInd, QUALITIES_VECTOR,PARTIAL_BASIN_SURFACE_DEPTHS, depth, basinNum);
    }

    
    
    
    else
    {
        printf("%s.\n",GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][basinSubModelInd]);
        printf("Error, invalid basin sub model name.\n");
    }


}


int determineBasinSurfaceBelow(global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double depth, int basinNum)
/*
 Purpose: determine the indice of the basin surface directly below the grid point in question
 
 Input variables:
    *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
    *PARTIAL_BASIN_SURFACE_DEPTHS - struct containing depths for all applicable basin surfaces at one lat - lon location
    depth - the depth of the gridpoint to determine the properties at
    basinNum - the basin number pertaining to the basin of interest
 
 Output variables:
    lowerSurfInd - indicie of the surface directly below the grid point
 */
{
    double lowerSurf;
    int lowerSurfInd;
    for( int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum]; i++)
    {
        lowerSurf = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][i];
        if((isnan(lowerSurf) == 0) && (lowerSurf <= depth))
        {
            lowerSurfInd = i;
            break;
        }
    }
    return lowerSurfInd;
}

int determineBasinSurfaceAbove(global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double depth, int basinNum)
/*
 Purpose: determine the indice of the basin surface directly above the grid point in question
 
 Input variables:
    *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
    *PARTIAL_BASIN_SURFACE_DEPTHS - struct containing depths for all applicable basin surfaces at one lat - lon location
    depth - the depth of the gridpoint to determine the properties at
    basinNum - the basin number pertaining to the basin of interest
 
 Output variables:
    lowerSurfInd - indicie of the surface directly above the grid point
 */
{
    double upperSurf;
    int upperSurfInd;
    for( int i = GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum]-1; i >= 0; i--)
    {
        upperSurf = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][i];
        if(isnan(upperSurf) == 0 && upperSurf >= depth)
        {
            upperSurfInd = i;
            break;
        }
        
    }
    return upperSurfInd;
}

//=================================================================

//          QUATERNARY MODELS

//=================================================================


// Gravel sub-model
void gravelSubModel(int zInd, qualities_vector *QUALITIES_VECTOR, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double depth, int basinNum)
{
    double Z, totalStress, effectiveStress, Vs;
    Z = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][0] - depth; // first indice represents the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81;
    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
    QUALITIES_VECTOR->Vs[zInd] = Vs;
    QUALITIES_VECTOR->Rho[zInd] = 1.6971;
    QUALITIES_VECTOR->Vp[zInd] = 1.6;
    
    //// Burwood gravel sub-model
    //valStructLocal *burwoodSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
    //{
    //	valStructLocal *values = NULL;
    //    values = malloc(sizeof(valStructLocal));
    //
    //    double Z, totalStress, effectiveStress, Vs;
    //    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    //    totalStress = Z * rho_const; // kPa
    //    effectiveStress = totalStress - Z * 9.81; // kPa
    //    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
    //    values->Vs = Vs; // in km/s
    //
    //    values->Rho = 1.6971;
    //    values->Vp = 1.6;
    ////    values->Vs = 0.6;
    //    
    //    return values;
    //}
}

// Marine sub-model
void marineSubModel(int zInd, qualities_vector *QUALITIES_VECTOR, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double depth, int basinNum)
{
    double Z, totalStress, effectiveStress, Vs;
    Z = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][0] - depth; // first indice represents the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81;
    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
    QUALITIES_VECTOR->Vs[zInd] = Vs;
    QUALITIES_VECTOR->Rho[zInd] = 1.6971;
    QUALITIES_VECTOR->Vp[zInd] = 1.6;
    
    //// Bromley formation sub-model
    //valStructLocal *bromleySubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
    //{
    //	valStructLocal *values = NULL;
    //    values = malloc(sizeof(valStructLocal));
    //
    //    double Z, totalStress, effectiveStress, Vs;
    //    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    //    totalStress = Z * rho_const; // kPa
    //    effectiveStress = totalStress - Z * 9.81; // kPa
    //    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
    //    values->Vs = Vs; // in km/s
    //
    //    values->Rho = 1.6971;
    //    values->Vp = 1.6;
    ////    values->Vs = 0.3;
    //    
    //    return values;
    //}
    
}
//=================================================================

//          PRE QUATERNARY MODELS

//=================================================================


// Paleogene sub-model
void paleogeneSubModelv1(int zInd, qualities_vector *QUALITIES_VECTOR)
{
    QUALITIES_VECTOR->Rho[zInd] =  2.151;
    QUALITIES_VECTOR->Vp[zInd] = 2.7;
    QUALITIES_VECTOR->Vs[zInd] = 1.1511;
}

// Miocene sub-model
void mioceneSubModelv1(int zInd, qualities_vector *QUALITIES_VECTOR)
{
    QUALITIES_VECTOR->Rho[zInd] = 2.0931;
    QUALITIES_VECTOR->Vp[zInd] = 2.5;
    QUALITIES_VECTOR->Vs[zInd] = 0.98355;

}

// Pliocene sub-model
void plioceneSubModelv1(int zInd, qualities_vector *QUALITIES_VECTOR)
{
    QUALITIES_VECTOR->Rho[zInd] = 1.905;
    QUALITIES_VECTOR->Vp[zInd] = 2;
    QUALITIES_VECTOR->Vs[zInd] = 0.6086;

}

//=================================================================

//          BPV MODELS

//=================================================================

// BPV sub-model
void BPVSubModelv1(int zInd, qualities_vector *QUALITIES_VECTOR)
{
    QUALITIES_VECTOR->Rho[zInd] = 2.393;
    QUALITIES_VECTOR->Vp[zInd] = 4.0;
    QUALITIES_VECTOR->Vs[zInd] = 2.2818;
}

void BPVSubModelv2(int zInd, qualities_vector *QUALITIES_VECTOR)
{
    QUALITIES_VECTOR->Rho[zInd] = 2.334;
    QUALITIES_VECTOR->Vp[zInd] = 3.60;
    QUALITIES_VECTOR->Vs[zInd] = 1.9428;

}

void BPVSubModelv3(int zInd, qualities_vector *QUALITIES_VECTOR, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, int basinNum, double depth)
{

    double pointDepth, wheatherDepth, Vp0, VpFull, Rho0, RhoFull, Vs0, VsFull;
    wheatherDepth = 100;
    pointDepth = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][0] - depth;

    Vp0 = 3.2; // velocity at the top of the BPV
    VpFull = 4.0; // velocity at the full

    Rho0 = 2.265;
    RhoFull = 2.393;

    Vs0 = 1.59;
    VsFull = 2.2818;

    if (pointDepth < wheatherDepth)
    {
        // wheathering function for top 100m
        QUALITIES_VECTOR->Vp[zInd] = linearInterpolation(0, wheatherDepth, Vp0, VpFull, pointDepth);
        QUALITIES_VECTOR->Rho[zInd] = linearInterpolation(0, wheatherDepth, Rho0, RhoFull, pointDepth);
        QUALITIES_VECTOR->Vs[zInd]  = linearInterpolation(0, wheatherDepth, Vs0, VsFull, pointDepth);
    }
    else
    {
        QUALITIES_VECTOR->Vp[zInd] = 4.0;
        QUALITIES_VECTOR->Rho[zInd] = 2.393;
        QUALITIES_VECTOR->Vs[zInd] = 2.2818;
    }
}





////  DEM to pliocene surface sub-model 1D
//valStructLocal *v1DBasinSubMod(gridStruct *location, int zInd, velo1D *subModel1D)
//{
//    valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//
//    // loop over depth values and assign qualities from the sub-model
//    for(int j = 0; j < subModel1D->nDep; j++)
//    {
//        if(location->Z[zInd] >= subModel1D->Dep[j]*-1000) // convert to meters, -ve being downwards
//        {
//            values->Rho = subModel1D->Rho[j];
//            values->Vp = subModel1D->Vp[j];
//            values->Vs = subModel1D->Vs[j];
//            break;
//        }
//        if(j == subModel1D->nDep)
//        {
//            printf("Error: Depth point below the extent represented in the 1D velocity model file.\n");
//        }
//    }
//
//    return values;
//}

//// Christchurch formation sub-model
//valStructLocal *christchurchSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
//{
//	valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    double Z, totalStress, effectiveStress, Vs;
//    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
//    totalStress = Z * rho_const; // kPa
//    effectiveStress = totalStress - Z * 9.81; // kPa
//    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
//    values->Vs = Vs; // in km/s
//    
//    values->Rho = 1.6971; // in kg/m3
//    values->Vp = 1.6; // in km/s
////    values->Vs = 0.17; // in km/s
//    
//    return values;
//}
//
//// Riccarton gravels sub-model
//valStructLocal *riccartonSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
//{
//	valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    double Z, totalStress, effectiveStress, Vs;
//    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
//    totalStress = Z * rho_const; // kPa
//    effectiveStress = totalStress - Z * 9.81; // kPa
//    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
//    values->Vs = Vs; // in km/s
//    
//    values->Rho = 1.6971;
//    values->Vp = 1.6;
////    values->Vs = 0.4;
//    
//    return values;
//}
//
//// Bromley formation sub-model
//valStructLocal *bromleySubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
//{
//	valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    double Z, totalStress, effectiveStress, Vs;
//    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
//    totalStress = Z * rho_const; // kPa
//    effectiveStress = totalStress - Z * 9.81; // kPa
//    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
//    values->Vs = Vs; // in km/s
//    
//    values->Rho = 1.6971;
//    values->Vp = 1.6;
////    values->Vs = 0.3;
//    
//    return values;
//}
//
//// Linwood gravel sub-model
//valStructLocal*linwoodSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
//{
//	valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    double Z, totalStress, effectiveStress, Vs;
//    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
//    totalStress = Z * rho_const; // kPa
//    effectiveStress = totalStress - Z * 9.81; // kPa
//    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
//    values->Vs = Vs; // in km/s
//    
//    values->Rho = 1.6971;
//    values->Vp = 1.6;
////    values->Vs = 0.5;
//    
//    return values;
//}
//
//// Heathcote formation sub-model
//valStructLocal *heathcoteSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
//{
//	valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    double Z, totalStress, effectiveStress, Vs;
//    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
//    totalStress = Z * rho_const; // kPa
//    effectiveStress = totalStress - Z * 9.81; // kPa
//    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
//    values->Vs = Vs; // in km/s
//    
//    values->Rho = 1.6971;
//    values->Vp = 1.6;
////    values->Vs = 0.4;
//    
//    return values;
//}
//
//// Burwood gravel sub-model
//valStructLocal *burwoodSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
//{
//	valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    double Z, totalStress, effectiveStress, Vs;
//    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
//    totalStress = Z * rho_const; // kPa
//    effectiveStress = totalStress - Z * 9.81; // kPa
//    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
//    values->Vs = Vs; // in km/s
//    
//    values->Rho = 1.6971;
//    values->Vp = 1.6;
////    values->Vs = 0.6;
//    
//    return values;
//}
//
//// Shirley formation sub-model
//valStructLocal *shirleySubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
//{
//	valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    double Z, totalStress, effectiveStress, Vs;
//    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
//    totalStress = Z * rho_const; // kPa
//    effectiveStress = totalStress - Z * 9.81; // kPa
//    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
//    values->Vs = Vs; // in km/s
//    
//    values->Rho = 1.6971;
//    values->Vp = 1.6;
////    values->Vs = 0.4;
//    
//    return values;
//}
//
//// Wainoni gravels sub-model
//valStructLocal *wainoniSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
//{
//    valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    double Z, totalStress, effectiveStress, Vs;
//    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
//    totalStress = Z * rho_const; // kPa
//    effectiveStress = totalStress - Z * 9.81; // kPa
//    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
//    values->Vs = Vs; // in km/s
//    
//    values->Rho = 1.6971;
//    values->Vp = 1.6;
////    values->Vs = 0.6;
//    
//    return values;
//}
//
////  DEM to pliocene surface sub-model (same as shirley formation)
//valStructLocal *DEMtoPlioceneSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
//{
//	valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    double Z, totalStress, effectiveStress, Vs;
//    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
//    totalStress = Z * rho_const; // kPa
//    effectiveStress = totalStress - Z * 9.81; // kPa
//    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
//    values->Vs = Vs; // in km/s
//    
//    values->Rho = 1.6971;
//    values->Vp = 1.6;
////    values->Vs = 0.4;
//    
//    return values;
//}
//
////  DEM to pliocene surface sub-model 1D
//valStructLocal *v1DBasinSubMod(gridStruct *location, int zInd, velo1D *subModel1D)
//{
//    valStructLocal *values = NULL;
//    values = malloc(sizeof(valStructLocal));
//    
//    // loop over depth values and assign qualities from the sub-model
//    for(int j = 0; j < subModel1D->nDep; j++)
//    {
//        if(location->Z[zInd] >= subModel1D->Dep[j]*-1000) // convert to meters, -ve being downwards
//        {
//            values->Rho = subModel1D->Rho[j];
//            values->Vp = subModel1D->Vp[j];
//            values->Vs = subModel1D->Vs[j];
//            break;
//        }
//        if(j == subModel1D->nDep)
//        {
//            printf("Error: Depth point below the extent represented in the 1D velocity model file.\n");
//        }
//    }
//    
//    return values;
//}
//
//
//




