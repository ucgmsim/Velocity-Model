//
//  EPtomo2010subMod.c
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

void EPtomo2010subMod(int xInd, int yInd, int zInd, globalDataValues *globalValues, gridStruct *location, depInterpVals *EPtomoData)
/*
 Purpose:   calculate the rho vp and vs values at a single lat long point for all the depths within this velocity submodel
 
 Input variables:
 depths         - pointer to a struct containing the references to which points lie within this velocity sub-model layer
 veloSubModNum  - the reference number of this velocity sub-model
 xInd           - the indice of the longitude point
 yInd           - the indice of the latitude point
 EPtomoData     - pointer to the Eberhart-Phillips 2010 tomography data
 
 Output variables:
 values         - struct containing the vp vs and rho values for all points within this velo sub-model
 
 */
{
    int count = 0;
    double p1, p2;
    double Rho1, Rho2, Vp1, Vp2, Vs1, Vs2;
    
    // find the indice of the first "surface" above the data point in question
    while(location->Z[zInd]<EPtomoData->deps[count]*1000)
    {
        count += 1;
    }
    
    // loop over the depth points and obtain the vp vs and rho values using interpolation between "surfaces"

    p1 = EPtomoData->deps[count-1]*1000;
    p2 = EPtomoData->deps[count]*1000;
    Rho1 = EPtomoData->Rho[count-1][xInd][yInd];
    Rho2 = EPtomoData->Rho[count][xInd][yInd];
    Vs1 = EPtomoData->Vs[count-1][xInd][yInd];
    Vs2 = EPtomoData->Vs[count][xInd][yInd];
    Vp1 = EPtomoData->Vp[count-1][xInd][yInd];
    Vp2 = EPtomoData->Vp[count][xInd][yInd];
    
    globalValues->Rho[xInd][yInd][zInd] = linearInterpolation(p1, p2, Rho1, Rho2, location->Z[zInd]);
    globalValues->Vs[xInd][yInd][zInd] = linearInterpolation(p1, p2, Vs1, Vs2, location->Z[zInd]);
    globalValues->Vp[xInd][yInd][zInd] = linearInterpolation(p1, p2, Vp1, Vp2, location->Z[zInd]);
    
}

depInterpVals *loadEPtomo2010subMod(gridStruct *location)
/*
 Purpose:   read in the Eberhart-Phillips 2010 tomography dataset
 
 Input variables:
 location       - struct containing the lat lon and dep points
 
 Output variables:
 surfDepVals    - (malloc'd) pointer to a struct containing the values of the "surfaces" at each lat lon value 
 
 */
{
    const char *varNames[3];
    varNames[0] = "vp", varNames[1] = "vs", varNames[2] = "rho";
    int nElev = 14; // only read first 14 for efficiency
    int elev[17] = {15, 1, -3, -8, -15, -23, -30, -38, -48, -65, -85, -105, -130, -155, -185, -225, -275 };
//    int elev[21] = {15, 1, -3, -5, -8, -11, -15, -23, -30, -38, -48, -65, -85, -105, -130, -155, -185, -225, -275, -370, -630};
    //int elev[20] = {10, 1, -5, -8, -11, -15, -23, -30, -38, -48, -65, -85, -105, -130, -155, -185, -225, -275, -370, -620};
    char baseFilename[256];
    
    depInterpVals *surfDepVals = NULL;
    surfDepVals = malloc(sizeof(depInterpVals));
    surfDepVals->numSurf = nElev;
    surfRead *tempSurf;
    adjacentPointsStruct *points;
    
    double X1, X2, Y1, Y2, Q11, Q12, Q22, Q21, X, Y, interpVal;


    for(int i = 0; i < nElev; i++)
    {
        surfDepVals->deps[i] = elev[i];
        for(int j = 0; j < 3; j++)
        {
            sprintf(baseFilename,"Data/Tomography/surf_tomography_%s_elev%i.in",varNames[j],elev[i]);
            // read the surface
            tempSurf = loadSurface(baseFilename);
            // write a surface vector (for IDW)
            for(int k = 0; k < location->nX; k++)
            {
                for(int n = 0; n < location->nY; n++)
                {
                    points = findAdjacentPoints(tempSurf, location->Lat[k][n], location->Lon[k][n]); // a little inefficient as process is repeated for each of Vp Vs and Rho!
                    X1 = tempSurf->loni[points->lonInd[0]];
                    X2 = tempSurf->loni[points->lonInd[1]];
                    Y1 = tempSurf->lati[points->latInd[0]];
                    Y2 = tempSurf->lati[points->latInd[1]];
                    X = location->Lon[k][n];
                    Y = location->Lat[k][n];
                    Q11 = tempSurf->raster[points->lonInd[0]][points->latInd[0]];
                    Q12 = tempSurf->raster[points->lonInd[0]][points->latInd[1]];
                    Q21 = tempSurf->raster[points->lonInd[1]][points->latInd[0]];
                    Q22 = tempSurf->raster[points->lonInd[1]][points->latInd[1]];
                    
                    interpVal = biLinearInterpolation(X1, X2, Y1, Y2, Q11, Q12, Q21, Q22, X, Y);
                    if(j == 0)
                    {
                        surfDepVals->Vp[i][k][n] = interpVal;
                    }
                    else if(j == 1)
                    {
                        surfDepVals->Vs[i][k][n] = interpVal;
                    }
                    else if(j ==2)
                    {
                        surfDepVals->Rho[i][k][n] = interpVal;
                    }
                    free(points);
                }
            }
            free(tempSurf);
        }
        printf("\rReading tomography data %d%% complete.", i*100/nElev);
        fflush(stdout);
    }
    printf("\rReading tomography data 100%% complete.");
    fflush(stdout);
    printf("\n");

return surfDepVals;

}

