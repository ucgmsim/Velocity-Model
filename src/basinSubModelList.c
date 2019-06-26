//
//  basinSubModelList.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 10/06/19.
//  Copyright (c) 2019 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"

void loadBasinResources(global_model_parameters *GLOBAL_MODEL_PARAMETERS)
{
    // loop over basin nums and load

    for (int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
    {
//        printf("%s %i\n",GLOBAL_MODEL_PARAMETERS->basin[i],i);
         // ============
         // v19p1 models
         // ============
         if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Canterbury_Pre_Quaternary_v19p1") == 0)
         {
             load_Canterbury_Pre_Quaternary_v19p1(GLOBAL_MODEL_PARAMETERS,i);
         }
         else if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Cantebury_North_v19p1") == 0)
         {
             load_Cantebury_North_v19p1(GLOBAL_MODEL_PARAMETERS,i);
         }
         else if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Banks_Peninsula_Volcanics_v19p1") == 0)
         {
             load_Banks_Peninsula_Volcanics_v19p1(GLOBAL_MODEL_PARAMETERS,i);
         }
         else if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Kaikoura_v19p1") == 0)
         {
             load_Kaikoura_v19p1(GLOBAL_MODEL_PARAMETERS,i);
         }
         else if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Cheviot_v19p1") == 0)
         {
             load_Cheviot_v19p1(GLOBAL_MODEL_PARAMETERS,i);
         }
         else if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Hanmer_v19p1") == 0)
         {
             load_Hanmer_v19p1(GLOBAL_MODEL_PARAMETERS,i);
         }
         else if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Marlborough_v19p1") == 0)
         {
             load_Marlborough_v19p1(GLOBAL_MODEL_PARAMETERS,i);
         }
         else if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Nelson_v19p1") == 0)
         {
             load_Nelson_v19p1(GLOBAL_MODEL_PARAMETERS,i);
         }
         else if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Wellington_v19p1") == 0)
         {
             load_Wellington_v19p1(GLOBAL_MODEL_PARAMETERS,i);
         }
        // ============
        // v19p1 models
        // ============
         else if (strcmp(GLOBAL_MODEL_PARAMETERS->basin[i],"Wellington_v19p6") == 0)
         {
             load_Wellington_v19p6(GLOBAL_MODEL_PARAMETERS,i);
         }
         else
         {
             printf("Basin %s not found.\n",GLOBAL_MODEL_PARAMETERS->basin[i]);
             exit(EXIT_FAILURE);
         }

        
    }
}

void load_Wellington_v19p1(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 2;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/NI_BASINS/Wellington_Polygon_WGS84.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "DEM";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/DEM/NZ_DEM_HD.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "Cant1D_v2";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "WellingtonBasement";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/NI_BASINS/Wellington_Basement_WGS84_v0p0.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
}

void load_Wellington_v19p6(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 2;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/NI_BASINS/Wellington_Polygon_19p6.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "DEM";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/DEM/NZ_DEM_HD.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "Cant1D_v2";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "WellingtonBasement";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/NI_BASINS/Wellington_Grid_WGS84_v19p5p6_100_hybrid10.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
}

void load_Nelson_v19p1(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 2;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/SI_BASINS/Nelson_Polygon_WGS84.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "DEM";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/DEM/NZ_DEM_HD.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "Cant1D_v2";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "NelsonBasement";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/SI_BASINS/Nelson_Basement_WGS84_v0p0.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
}


void load_Marlborough_v19p1(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 2;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/SI_BASINS/Marlborough_Polygon_WGS84_v0p1.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "DEM";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/DEM/NZ_DEM_HD.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "Cant1D_v2";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "MarlboroughBasement";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/SI_BASINS/Marlborough_Basement_WGS84_v0p1.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
}

void load_Hanmer_v19p1(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 2;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/SI_BASINS/Hanmer_Polygon_WGS84.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "DEM";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/DEM/NZ_DEM_HD.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "Cant1D_v2";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "HanmerBasement";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/SI_BASINS/Hanmer_Basement_WGS84_v0p0.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
}



void load_Cheviot_v19p1(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 2;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/SI_BASINS/Cheviot_Polygon_WGS84.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "DEM";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/DEM/NZ_DEM_HD.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "Cant1D_v2";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "CheviotBasement";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/SI_BASINS/Cheviot_Basement_WGS84_v0p0.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
}

void load_Kaikoura_v19p1(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 2;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/SI_BASINS/Kaikoura_Polygon_WGS84.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "DEM";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/DEM/NZ_DEM_HD.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "Cant1D_v2";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "KaikouraBasement";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/SI_BASINS/Kaikoura_Basement_WGS84_v0p0.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
}

void load_Banks_Peninsula_Volcanics_v19p1(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 2;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/Boundaries/BPVBoundary.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "BPVTop";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/Canterbury_Basin/BPV/BPVTop.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "BPVSubMod_v4";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "MioceneTop";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
    
}

void load_Cantebury_North_v19p1(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 2;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/SI_BASINS/NorthCanterbury_Polygon_WGS84.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "DEM";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/DEM/NZ_DEM_HD.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "Cant1D_v2";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "NorthCanterburyBasement";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/SI_BASINS/NorthCanterbury_Basement_WGS84_v0p0.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
}


void load_Canterbury_Pre_Quaternary_v19p1(global_model_parameters *GLOBAL_MODEL_PARAMETERS, int basinNum)
{
    // CANTERBURY Basin (1D above basement)
    GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum] = 5;
    GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum] = 1;
    GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][0] = "Data/Boundaries/NewCanterburyBasinBoundary_WGS84_1m.txt";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][0] = "DEM";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][0] = "Data/DEM/CantDEM.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][0] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][0] = "Cant1D_v2_Pliocene_Enforced";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][1] = "PlioceneTop";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][1] = "Data/Canterbury_Basin/Pre_Quaternary/Pliocene_46_v8p9p18.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][1] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][1] = "PlioceneSubMod_v1";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][2] = "MioceneTop";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][2] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][2] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][2] = "MioceneSubMod_v1";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][3] = "PaleogeneTop";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][3] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][3] = 0;
    GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][3] = "PaleogeneSubMod_v1";
    
    GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][4] = "BasementTopSurf";
    GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][4] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
    GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[basinNum][4] = 0;
}
