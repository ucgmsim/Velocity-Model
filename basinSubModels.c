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

// Christchurch formation sub-model
valStructLocal *christchurchSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double Z, totalStress, effectiveStress, Vs;
    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81; // kPa
    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
    values->Vs = Vs; // in km/s
    
    values->Rho = 1.6971; // in kg/m3
    values->Vp = 1.6; // in km/s
//    values->Vs = 0.17; // in km/s
    
    return values;
}

// Riccarton gravels sub-model
valStructLocal *riccartonSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double Z, totalStress, effectiveStress, Vs;
    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81; // kPa
    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
    values->Vs = Vs; // in km/s
    
    values->Rho = 1.6971;
    values->Vp = 1.6;
//    values->Vs = 0.4;
    
    return values;
}

// Bromley formation sub-model
valStructLocal *bromleySubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double Z, totalStress, effectiveStress, Vs;
    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81; // kPa
    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
    values->Vs = Vs; // in km/s
    
    values->Rho = 1.6971;
    values->Vp = 1.6;
//    values->Vs = 0.3;
    
    return values;
}

// Linwood gravel sub-model
valStructLocal*linwoodSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double Z, totalStress, effectiveStress, Vs;
    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81; // kPa
    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
    values->Vs = Vs; // in km/s
    
    values->Rho = 1.6971;
    values->Vp = 1.6;
//    values->Vs = 0.5;
    
    return values;
}

// Heathcote formation sub-model
valStructLocal *heathcoteSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double Z, totalStress, effectiveStress, Vs;
    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81; // kPa
    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
    values->Vs = Vs; // in km/s
    
    values->Rho = 1.6971;
    values->Vp = 1.6;
//    values->Vs = 0.4;
    
    return values;
}

// Burwood gravel sub-model
valStructLocal *burwoodSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double Z, totalStress, effectiveStress, Vs;
    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81; // kPa
    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
    values->Vs = Vs; // in km/s
    
    values->Rho = 1.6971;
    values->Vp = 1.6;
//    values->Vs = 0.6;
    
    return values;
}

// Shirley formation sub-model
valStructLocal *shirleySubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double Z, totalStress, effectiveStress, Vs;
    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81; // kPa
    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
    values->Vs = Vs; // in km/s
    
    values->Rho = 1.6971;
    values->Vp = 1.6;
//    values->Vs = 0.4;
    
    return values;
}

// Wainoni gravels sub-model
valStructLocal *wainoniSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
    valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double Z, totalStress, effectiveStress, Vs;
    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81; // kPa
    Vs = As_gravel * pow((effectiveStress/P_a),ns_gravel);
    values->Vs = Vs; // in km/s
    
    values->Rho = 1.6971;
    values->Vp = 1.6;
//    values->Vs = 0.6;
    
    return values;
}

//  DEM to pliocene surface sub-model (same as shirley formation)
valStructLocal *DEMtoPlioceneSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double Z, totalStress, effectiveStress, Vs;
    Z = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd]; // zero indice represents the first surface, should always be the DEM
    totalStress = Z * rho_const; // kPa
    effectiveStress = totalStress - Z * 9.81; // kPa
    Vs = As_silt * pow((effectiveStress/P_a),ns_marine);
    values->Vs = Vs; // in km/s
    
    values->Rho = 1.6971;
    values->Vp = 1.6;
//    values->Vs = 0.4;
    
    return values;
}

//  DEM to pliocene surface sub-model 1D
valStructLocal *v1DBasinSubMod(gridStruct *location, int zInd, velo1D *subModel1D)
{
    valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    // loop over depth values and assign qualities from the sub-model
    for(int j = 0; j < subModel1D->nDep; j++)
    {
        if(location->Z[zInd] >= subModel1D->Dep[j]*-1000) // convert to meters, -ve being downwards
        {
            values->Rho = subModel1D->Rho[j];
            values->Vp = subModel1D->Vp[j];
            values->Vs = subModel1D->Vs[j];
            break;
        }
        if(j == subModel1D->nDep)
        {
            printf("Error: Depth point below the extent represented in the 1D velocity model file.\n");
        }
    }
    
    return values;
}


//=================================================================

//          PRE QUATERNARY MODELS

//=================================================================

// Pliocene sub-model
valStructLocal *plioceneSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));

//    double depth, age, k_interp;
//
//    depth = -basinData->surfVals[basinNum][xInd][yInd][2];
//    
//    age = linearInterpolation(basinData->surfVals[basinNum][xInd][yInd][1], basinData->surfVals[basinNum][xInd][yInd][2], age_1_pliocene, age_2_pliocene, location->Z[zInd]);
//    k_interp = linearInterpolation(basinData->surfVals[basinNum][xInd][yInd][1], basinData->surfVals[basinNum][xInd][yInd][2], k_pliocene_top, k_pliocene_bot, location->Z[zInd]);
////    printf("%lf %lf %lf\n",depth, age, k_interp);
//    values->Vp = k_interp*pow(depth*age,0.16666);
//    values->Rho = rhoFromVpBrocher(values->Vp);
//    values->Vs = vsFromVpBrocher(values->Vp);
    
    values->Rho = 1.905;
    values->Vp = 2;
    values->Vs = 0.6086;
    
    return values;
}

// Miocene sub-model
valStructLocal *mioceneSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
//    double depth, age, k_interp;
//    
//    depth = -basinData->surfVals[basinNum][xInd][yInd][3];
//    
//    age = linearInterpolation(basinData->surfVals[basinNum][xInd][yInd][2], basinData->surfVals[basinNum][xInd][yInd][3], age_1_miocene, age_2_miocene, location->Z[zInd]);
//    k_interp = linearInterpolation(basinData->surfVals[basinNum][xInd][yInd][1], basinData->surfVals[basinNum][xInd][yInd][2], k_miocene_top, k_miocene_bot, location->Z[zInd]);
//    values->Vp = k_interp*pow(depth*age,0.16666);
//    values->Rho = rhoFromVpBrocher(values->Vp);
//    values->Vs = vsFromVpBrocher(values->Vp);
    
    values->Rho = 2.0931;
    values->Vp = 2.5;
    values->Vs = 0.98355;
    
    return values;
}

// Paleogene sub-model
valStructLocal *paleogeneSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
	valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
//    double depth, age, k_interp;
//    
//    depth = -basinData->surfVals[basinNum][xInd][yInd][4];
//    
//    age = linearInterpolation(basinData->surfVals[basinNum][xInd][yInd][3], basinData->surfVals[basinNum][xInd][yInd][4], age_1_paleogene, age_2_paleogene, location->Z[zInd]);
//    k_interp = linearInterpolation(basinData->surfVals[basinNum][xInd][yInd][1], basinData->surfVals[basinNum][xInd][yInd][2], k_paleogene_top, k_paleogene_bot, location->Z[zInd]);
//    values->Vp = k_interp*pow(depth*age,0.16666);
//    values->Rho = rhoFromVpBrocher(values->Vp);
//    values->Vs = vsFromVpBrocher(values->Vp);
    
    values->Rho = 2.151;
    values->Vp = 2.7;
    values->Vs = 1.1511;
    
    return values;
}

//=================================================================

//          BPV MODEL

//=================================================================

// BPV sub-model
valStructLocal *BPVSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
    valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
// values for 1.61
//    values->Vp = 4.0;
//    values->Rho = 2.393;
//    values->Vs = 2.2818;
//
    // values for 1.62
    values->Vp = 3.60;
    values->Rho = 2.334;
    values->Vs = 1.9428;
    return values;
}

// BPV sub-model
valStructLocal *WheatheredBPVSubModel(gridStruct *location, globalBasinData *basinData, int xInd, int yInd, int zInd, int basinNum)
{
    valStructLocal *values = NULL;
    values = malloc(sizeof(valStructLocal));
    
    double pointDepth, wheatherDepth, Vp0, VpFull, Rho0, RhoFull, Vs0, VsFull;
    wheatherDepth = 100;
    pointDepth = basinData->surfVals[basinNum][xInd][yInd][0] - location->Z[zInd];
    
    Vp0 = 3.2; // velocity at the top of the BPV
    VpFull = 4.0; // velocity at the full
    
    Rho0 = 2.265;
    RhoFull = 2.393;
    
    Vs0 = 1.59;
    VsFull = 1.9428;
        
    if (pointDepth < wheatherDepth)
    {
        // wheathering function for top 100m
        values->Vp = linearInterpolation(0, wheatherDepth, Vp0, VpFull, pointDepth);
        values->Rho = linearInterpolation(0, wheatherDepth, Rho0, RhoFull, pointDepth);
        values->Vs = linearInterpolation(0, wheatherDepth, Vs0, VsFull, pointDepth);
    }
    else
    {
        values->Vp = 4.0;
        values->Rho = 2.393;
        values->Vs = 2.2818;
    }

    return values;
}



