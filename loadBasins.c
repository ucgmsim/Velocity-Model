//
//  loadBasins.c
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

void loadBasin(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    assert(basinData->nBasinSubMod[basinNum]<=MAX_NUM_VELOSUBMOD);
    assert(basinData->nSurf[basinNum]<=MAX_NUM_VELOSUBMOD);
    
//    printf("%i %i\n",location->nY,location->nX);
    loadBoundary(basinData, basinNum);
    determineIfWithinBasinLatLon(location, basinNum, basinData);
    loadBasinSurfaces(location, basinNum, basinData);
    enforceBasinSurfaceDepths(location, basinNum, basinData);
    assignBasinProperties(location, basinNum, basinData);
    
}

void loadBasinSurfaces(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    // load and interpolate basin surfaces for all points

    surfDepValues *surfDeps = NULL;
    char *fileName = NULL;
    for(int i = 0; i < basinData->nSurf[basinNum]; i++)
    {
        if(strcmp(basinData->surf[basinNum][i], "DEM")==0)
        {
            fileName = "Data/DEM/DEM.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "DEM_1D")==0)
        {
            fileName = "Data/DEM/DEM.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "RiccartonTop")==0)
        {
            fileName = "Data/Canterbury_Basin/Quaternary/RiccartonTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "BromleyTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Quaternary/BromleyTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "LinwoodTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Quaternary/LinwoodTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "HeathcoteTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Quaternary/HeathcoteTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "BurwoodTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Quaternary/BurwoodTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "ShirleyTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Quaternary/ShirleyTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "WainoniTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Quaternary/WainoniTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "PlioceneTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Pre_Quaternary/PlioceneTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "MioceneTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "PaleogeneTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "BasementTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/Pre_Quaternary/BasementTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "BPVTop") == 0)
        {
            fileName = "Data/Canterbury_Basin/BPV/BPVTop.in";
        }
        else if(strcmp(basinData->surf[basinNum][i], "BPVTopWheathered") == 0)
        {
            fileName = "Data/Canterbury_Basin/BPV/BPVTop.in";
        }
        else
        {
            printf("Error.\n");
        }
        surfDeps = determineSurfaceDepthsBasin(basinData, location, fileName, basinNum, i);
    
        // write individual surface depths into the global file
        for(int j = 0; j < location->nX; j++)
        {
            for(int k = 0; k < location->nY; k++)
            {
                basinData->surfVals[basinNum][j][k][i] = surfDeps->dep[j][k];
            }
        }
        free(surfDeps);
        printf("Completed calculation of basin surface depths %i of %i.\n", i+1, basinData->nSurf[basinNum]);
    }
    
    printf("Basin surfaces successfully loaded.\n");
}



void determineBasinProperties(globalBasinData *basinData, int basinNum, int xInd, int yInd, int zInd, gridStruct *location, velo1D *subModel1D)
{
    
    //find surfaces adjacent to the point
    
    double upperSurf, lowerSurf;
    char *upperSurfName = NULL, *lowerSurfName = NULL;
    for(int i = 0; i < basinData->nSurf[basinNum]; i++)
    {
        lowerSurf = basinData->surfVals[basinNum][xInd][yInd][i];
        if(lowerSurf<=location->Z[zInd] && isnan(lowerSurf) !=1)
        {
            lowerSurfName = basinData->surf[basinNum][i];
            break;
        }
        if (i == basinData->nSurf[basinNum])
        {
            printf("Error\n");
        }
    }
    
    for(int i = basinData->nSurf[basinNum]; i > 0; i--)
    {
        upperSurf = basinData->surfVals[basinNum][xInd][yInd][i-1];
        //printf("%lf %lf %i\n",upperSurf,location->Z[zInd],i-1);
        if(upperSurf>location->Z[zInd] && isnan(upperSurf) !=1)
        {
            upperSurfName = basinData->surf[basinNum][i-1];
            break;
        }
        if( (i-1) == 0)
        {
            printf("Error\n");
        }
    }
    

    // call upon the respective layer specific velocity model
    
    valStructLocal *values = NULL;
    if((strcmp(upperSurfName, "DEM") == 0) && strcmp(lowerSurfName, "RiccartonTop") == 0)
    {
        values = christchurchSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "RiccartonTop") == 0) && strcmp(lowerSurfName, "BromleyTop") == 0)
    {
        values = riccartonSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "BromleyTop") == 0) && strcmp(lowerSurfName, "LinwoodTop") == 0)
    {
        values = bromleySubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "LinwoodTop") == 0) && strcmp(lowerSurfName, "HeathcoteTop") == 0)
    {
        values = linwoodSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "HeathcoteTop") == 0) && strcmp(lowerSurfName, "BurwoodTop") == 0)
    {
        values = heathcoteSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "BurwoodTop") == 0) && strcmp(lowerSurfName, "ShirleyTop") == 0)
    {
        values = burwoodSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "ShirleyTop") == 0) && strcmp(lowerSurfName, "WainoniTop") == 0)
    {
        values = shirleySubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "WainoniTop") == 0) && strcmp(lowerSurfName, "PlioceneTop") == 0)
    {
        values = wainoniSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "DEM") == 0) && strcmp(lowerSurfName, "PlioceneTop") == 0)
    {
        values = DEMtoPlioceneSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "DEM_1D") == 0) && strcmp(lowerSurfName, "PlioceneTop") == 0)
    {
        values = v1DBasinSubMod(location, zInd, subModel1D);
    }
    else if((strcmp(upperSurfName, "PlioceneTop") == 0) && strcmp(lowerSurfName, "MioceneTop") == 0)
    {
        values = plioceneSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "MioceneTop") == 0) && strcmp(lowerSurfName, "PaleogeneTop") == 0)
    {
        values = mioceneSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "PaleogeneTop") == 0) && strcmp(lowerSurfName, "BasementTop") == 0)
    {
        values = paleogeneSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "BPVTop") == 0) && strcmp(lowerSurfName, "MioceneTop") == 0)
    {
        values = BPVSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else if((strcmp(upperSurfName, "BPVTopWheathered") == 0) && strcmp(lowerSurfName, "MioceneTop") == 0)
    {
        values = WheatheredBPVSubModel(location, basinData, xInd, yInd, zInd, basinNum);
    }
    else
    {
        printf("error\n");
    }
    
    basinData->Vp[basinNum][xInd][yInd][zInd] = values->Vp;
    basinData->Vs[basinNum][xInd][yInd][zInd] = values->Vs;
    basinData->Rho[basinNum][xInd][yInd][zInd] = values->Rho;
    free(values);
}



void assignBasinProperties(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    // load 1D velo model
    velo1D *velocityModel1DBasinData = NULL;
    char *fileName;
    fileName = "Data/1D_Velocity_Model/Cant1D_v2.fd_modfile";
    velocityModel1DBasinData = loadv1DsubMod(fileName);
    
    // calculate the properties for all points inside the basin
    for(int i = 0; i < location->nX; i++)
    {
        for(int j = 0; j < location->nY; j++)
        {
            if(basinData->inBasinLatLon[basinNum][basinData->nBoundaries[basinNum]-1][i][j] == 1) // in basin lat lon (wider boundary)
            {
                for(int k = 0; k < location->nZ; k++)
                {
                    if(basinData->inBasinDep[basinNum][i][j][k] == 1) // in basin Z limits
                    {
                        determineBasinProperties(basinData, basinNum, i, j, k, location, velocityModel1DBasinData);
                    }
                }
            }
        }
//        printf("Completed calculation of basin properties at latitude %i of %i.\n", i+1, location->nX);
        printf("\rAssigning basin properties %d%% complete.", i*100/location->nX);
        fflush(stdout);
    }
    printf("\rAssigning basin properties 100%% complete.");
    fflush(stdout);
    printf("\n");
    printf("Basin data successfully calculated.\n");
    free(velocityModel1DBasinData);
}


void enforceBasinSurfaceDepths(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    // enforce basin surface depths for all points in the within the lat lon poly (ignore others)
    double topLim, botLim;
    for(int i = 0; i < location->nX; i++)
    {
        for(int j = 0; j < location->nY; j++)
        {
            if(basinData->inBasinLatLon[basinNum][basinData->nBoundaries[basinNum]-1][i][j] == 1)
                // enforce depths based on the larger (or final) boundary
            {
                // enforce
                enforceSurfaceDepths(basinData, i, j, basinNum);
                
                // check if depth is within the top and bottom basin layers
                topLim = basinData->surfVals[basinNum][i][j][0];
                botLim = basinData->surfVals[basinNum][i][j][basinData->nSurf[basinNum]-1];
                for(int k  = 0; k < location->nZ; k++)
                {
                    if(location->Z[k] > topLim)
                    {
                        basinData->inBasinDep[basinNum][i][j][k] = 0;
                    }
                    else if (location->Z[k] < botLim)
                    {
                        basinData->inBasinDep[basinNum][i][j][k] = 0;
                    }
                    else
                    {
                        basinData->inBasinDep[basinNum][i][j][k] = 1; // in the basin Z limits
                    }
                }
            }
        }
    }
    printf("Basin surfaces depths enforced.\n");
}


























