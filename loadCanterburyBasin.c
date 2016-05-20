//
//  loadCanterburyBasin.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 28/11/14.
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

// FULL CANTERBURY BASIN
void loadCanterburyBasinData(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    basinData->surf[basinNum][0] = "DEM";
    basinData->surf[basinNum][1] = "RiccartonTop";
    basinData->surf[basinNum][2] = "BromleyTop";
    basinData->surf[basinNum][3] = "LinwoodTop";
    basinData->surf[basinNum][4] = "HeathcoteTop";
    basinData->surf[basinNum][5] = "BurwoodTop";
    basinData->surf[basinNum][6] = "ShirleyTop";
    basinData->surf[basinNum][7] = "WainoniTop";
    basinData->surf[basinNum][8] = "PlioceneTop";
    basinData->surf[basinNum][9] = "MioceneTop";
    basinData->surf[basinNum][10] = "PaleogeneTop";
    basinData->surf[basinNum][11] = "BasementTop";
    basinData->nSurf[basinNum] = 12;
    
    
    // basin boundaries must be completely encompassed within higher level boundaries
    // ie basin [0] must be completely within basin [0] in order to enforce the depths properly
    
    basinData->nBoundaries[basinNum] = 2;
    basinData->boundaryFileName[basinNum][0] = "Data/Boundaries/QuaternarySurfacesBoundary.txt";
    basinData->boundaryFileName[basinNum][1] = "Data/Boundaries/CanterburyBasinBoundary.txt";
    
    basinData->boundaryType[basinNum][0] = 1; // dem utilizes the largest boundary
    basinData->boundaryType[basinNum][1] = 0;
    basinData->boundaryType[basinNum][2] = 0;
    basinData->boundaryType[basinNum][3] = 0;
    basinData->boundaryType[basinNum][4] = 0;
    basinData->boundaryType[basinNum][5] = 0;
    basinData->boundaryType[basinNum][6] = 0; // 7 surfaces utilize boundary #0 (small boundary)
    basinData->boundaryType[basinNum][7] = 0; // other surfaces utilize boundary #1
    basinData->boundaryType[basinNum][8] = 1;
    basinData->boundaryType[basinNum][9] = 1;
    basinData->boundaryType[basinNum][10] = 1;
    basinData->boundaryType[basinNum][11] = 1;

    
    // load the basin data
    loadBasin(location, basinNum, basinData);

    
}

// PRE QUATERNARY BASIN ONLY
void loadPreQCanterburyBasinData(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    basinData->surf[basinNum][0] = "PlioceneTop";
    basinData->surf[basinNum][1] = "MioceneTop";
    basinData->surf[basinNum][2] = "PaleogeneTop";
    basinData->surf[basinNum][3] = "BasementTop";
    basinData->nSurf[basinNum] = 4;
    
    
    // basin boundaries must be completely encompassed within higher level boundaries
    // ie basin [0] must be completely within basin [0] in order to enforce the depths properly
    
    basinData->nBoundaries[basinNum] = 1;
    basinData->boundaryFileName[basinNum][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
    
    basinData->boundaryType[basinNum][0] = 0;
    basinData->boundaryType[basinNum][1] = 0;
    basinData->boundaryType[basinNum][2] = 0;
    basinData->boundaryType[basinNum][3] = 0;

    
    // load the basin data
    loadBasin(location, basinNum, basinData);
    
    
}

// PRE QUATERNARY BASIN ONLY W/ 1D
void loadPreQCanterburyBasinData1D(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    basinData->surf[basinNum][0] = "DEM_1D";
    basinData->surf[basinNum][1] = "PlioceneTop";
    basinData->surf[basinNum][2] = "MioceneTop";
    basinData->surf[basinNum][3] = "PaleogeneTop";
    basinData->surf[basinNum][4] = "BasementTop";
    basinData->nSurf[basinNum] = 5;
    
    
    // basin boundaries must be completely encompassed within higher level boundaries
    // ie basin [0] must be completely within basin [0] in order to enforce the depths properly
    
    basinData->nBoundaries[basinNum] = 1;
    basinData->boundaryFileName[basinNum][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
    
    basinData->boundaryType[basinNum][0] = 0;
    basinData->boundaryType[basinNum][1] = 0;
    basinData->boundaryType[basinNum][2] = 0;
    basinData->boundaryType[basinNum][3] = 0;
    basinData->boundaryType[basinNum][4] = 0;

    
    
    // load the basin data
    loadBasin(location, basinNum, basinData);
    
    
}

// QUATERNARY BASIN ONLY
void loadQCanterburyBasinData(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    basinData->surf[basinNum][0] = "DEM";
    basinData->surf[basinNum][1] = "RiccartonTop";
    basinData->surf[basinNum][2] = "BromleyTop";
    basinData->surf[basinNum][3] = "LinwoodTop";
    basinData->surf[basinNum][4] = "HeathcoteTop";
    basinData->surf[basinNum][5] = "BurwoodTop";
    basinData->surf[basinNum][6] = "ShirleyTop";
    basinData->surf[basinNum][7] = "WainoniTop";
    basinData->surf[basinNum][8] = "PlioceneTop";
    basinData->nSurf[basinNum] = 9;
    
    
    // basin boundaries must be completely encompassed within higher level boundaries
    // ie basin [0] must be completely within basin [0] in order to enforce the depths properly
    
    basinData->nBoundaries[basinNum] = 1;
    basinData->boundaryFileName[basinNum][0] = "Data/Boundaries/QuaternarySurfacesBoundary.txt";
    
    basinData->boundaryType[basinNum][0] = 0;
    basinData->boundaryType[basinNum][1] = 0;
    basinData->boundaryType[basinNum][2] = 0;
    basinData->boundaryType[basinNum][3] = 0;
    basinData->boundaryType[basinNum][4] = 0;
    basinData->boundaryType[basinNum][5] = 0;
    basinData->boundaryType[basinNum][6] = 0;
    basinData->boundaryType[basinNum][7] = 0;
    basinData->boundaryType[basinNum][8] = 0;
    
    
    // load the basin data
    loadBasin(location, basinNum, basinData);
    
    
}

// PRE QUATERNARY BASIN WITH PALOEGENE ONLY
void loadPreQCanterburyBasinDataPaleogene(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    basinData->surf[basinNum][0] = "PaleogeneTop";
    basinData->surf[basinNum][1] = "BasementTop";
    basinData->nSurf[basinNum] = 2;
    
    
    // basin boundaries must be completely encompassed within higher level boundaries
    // ie basin [0] must be completely within basin [0] in order to enforce the depths properly
    
    basinData->nBoundaries[basinNum] = 1;
    basinData->boundaryFileName[basinNum][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
    
    basinData->boundaryType[basinNum][0] = 0;
    basinData->boundaryType[basinNum][1] = 0;
    
    
    // load the basin data
    loadBasin(location, basinNum, basinData);
    
    
}

// PRE QUATERNARY BASIN WITH PALOEGENE + MIOCENE ONLY
void loadPreQCanterburyBasinDataPaleogeneMiocene(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    basinData->surf[basinNum][0] = "MioceneTop";
    basinData->surf[basinNum][1] = "PaleogeneTop";
    basinData->surf[basinNum][2] = "BasementTop";
    basinData->nSurf[basinNum] = 3;
    
    
    // basin boundaries must be completely encompassed within higher level boundaries
    // ie basin [0] must be completely within basin [0] in order to enforce the depths properly
    
    basinData->nBoundaries[basinNum] = 1;
    basinData->boundaryFileName[basinNum][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
    
    basinData->boundaryType[basinNum][0] = 0;
    basinData->boundaryType[basinNum][1] = 0;
    basinData->boundaryType[basinNum][2] = 0;
    
    
    // load the basin data
    loadBasin(location, basinNum, basinData);
    
    
}

// BPV Basin
void loadBPVBasinData(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    basinData->surf[basinNum][0] = "BPVTop";
    basinData->surf[basinNum][1] = "MioceneTop";
    basinData->nSurf[basinNum] = 2;
    
    
    // basin boundaries must be completely encompassed within higher level boundaries
    // ie basin [0] must be completely within basin [0] in order to enforce the depths properly
    
    basinData->nBoundaries[basinNum] = 1;
    basinData->boundaryFileName[basinNum][0] = "Data/Boundaries/BPVBoundary.txt";
    
    basinData->boundaryType[basinNum][0] = 0;
    basinData->boundaryType[basinNum][1] = 0;
    
    
    // load the basin data
    loadBasin(location, basinNum, basinData);
    
    
}

// BPV Basin
void loadBPVWheatheredBasinData(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    basinData->surf[basinNum][0] = "BPVTopWheathered";
    basinData->surf[basinNum][1] = "MioceneTop";
    basinData->nSurf[basinNum] = 2;
    
    
    // basin boundaries must be completely encompassed within higher level boundaries
    // ie basin [0] must be completely within basin [0] in order to enforce the depths properly
    
    basinData->nBoundaries[basinNum] = 1;
    basinData->boundaryFileName[basinNum][0] = "Data/Boundaries/BPVBoundary.txt";
    
    basinData->boundaryType[basinNum][0] = 0;
    basinData->boundaryType[basinNum][1] = 0;
    
    
    // load the basin data
    loadBasin(location, basinNum, basinData);
    
    
}


