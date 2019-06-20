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

void EPtomo2010subMod(int zInd, double dep, mesh_vector *MESH_VECTOR, qualities_vector *QUALITIES_VECTOR, nz_tomography_data *NZ_TOMOGRAPHY_DATA,global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, int inAnyBasinLatLon, int onBoundary)
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
    double relativeDepth;
    // find the indice of the first "surface" above the data point in question
    while(dep < NZ_TOMOGRAPHY_DATA->surfDeps[count]*1000)
    {
        count += 1;
    }
    int indAbove = count - 1;
    int indBelow = count;
    
    
    
    // loop over the depth points and obtain the vp vs and rho values using interpolation between "surfaces"
    adjacent_points *ADJACENT_POINTS;
    global_surf_read *SURFACE_POINTER_ABOVE;
    global_surf_read *SURFACE_POINTER_BELOW;
    
//    char *quality = NULL;
    
    double X1b, X2b, Y1b, Y2b, Q11b, Q12b, Q21b, Q22b, X, Y;
    double X1a, X2a, Y1a, Y2a, Q11a, Q12a, Q21a, Q22a;
    double valAbove, valBelow;
    double depAbove, depBelow;
    double val;
    
    // find the adjscent points for interpolatin from the first surface (assume all surfaces utilise the same grid)
    ADJACENT_POINTS = findGlobalAdjacentPoints(NZ_TOMOGRAPHY_DATA->surf[0][0], *MESH_VECTOR->Lat, *MESH_VECTOR->Lon);
    
//    printf("Grid point (%lf %lf %lf) \n",*MESH_VECTOR->Lon,*MESH_VECTOR->Lat, dep);


    for( int i = 0; i < 3; i++)
    {
//        if (i == 0)
//        {
//            quality = "Vp";
//
//        }
//        else if ( i == 1)
//        {
//            quality = "Vs";
//
//        }
//        else if (i == 2)
//        {
//            quality = "Rho";
//
//        };
        SURFACE_POINTER_ABOVE = NZ_TOMOGRAPHY_DATA->surf[i][indAbove];
        
        SURFACE_POINTER_BELOW = NZ_TOMOGRAPHY_DATA->surf[i][indBelow];
        
        if ( ADJACENT_POINTS->inSurfaceBounds == 0)
        {
            printf("Grid point (%lf %lf) lies outside of Tomography surface bounds.\n",*MESH_VECTOR->Lon,*MESH_VECTOR->Lat);
            exit(EXIT_FAILURE);
        }
        else
        {
            X1b = SURFACE_POINTER_BELOW->loni[ADJACENT_POINTS->lonInd[0]];
            X2b = SURFACE_POINTER_BELOW->loni[ADJACENT_POINTS->lonInd[1]];
            Y1b = SURFACE_POINTER_BELOW->lati[ADJACENT_POINTS->latInd[0]];
            Y2b = SURFACE_POINTER_BELOW->lati[ADJACENT_POINTS->latInd[1]];
            Q11b = SURFACE_POINTER_BELOW->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[0]];
            Q12b = SURFACE_POINTER_BELOW->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[1]];
            Q21b = SURFACE_POINTER_BELOW->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[0]];
            Q22b = SURFACE_POINTER_BELOW->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[1]];
            X = *MESH_VECTOR->Lon;
            Y = *MESH_VECTOR->Lat;
            
            
            X1a = SURFACE_POINTER_ABOVE->loni[ADJACENT_POINTS->lonInd[0]];
            X2a = SURFACE_POINTER_ABOVE->loni[ADJACENT_POINTS->lonInd[1]];
            Y1a = SURFACE_POINTER_ABOVE->lati[ADJACENT_POINTS->latInd[0]];
            Y2a = SURFACE_POINTER_ABOVE->lati[ADJACENT_POINTS->latInd[1]];
            Q11a = SURFACE_POINTER_ABOVE->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[0]];
            Q12a = SURFACE_POINTER_ABOVE->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[1]];
            Q21a = SURFACE_POINTER_ABOVE->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[0]];
            Q22a = SURFACE_POINTER_ABOVE->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[1]];
            
            valAbove = biLinearInterpolation(X1a, X2a, Y1a, Y2a, Q11a, Q12a, Q21a, Q22a, X, Y);
            valBelow = biLinearInterpolation(X1b, X2b, Y1b, Y2b, Q11b, Q12b, Q21b, Q22b, X, Y);
            
            
            depAbove = NZ_TOMOGRAPHY_DATA->surfDeps[indAbove]*1000;
            depBelow = NZ_TOMOGRAPHY_DATA->surfDeps[indBelow]*1000;
            val = linearInterpolation(depAbove, depBelow, valAbove, valBelow, dep);
            if (i == 0)
            {
                QUALITIES_VECTOR->Vp[zInd] = val;
                
            }
            else if ( i == 1)
            {
                QUALITIES_VECTOR->Vs[zInd] = val;
                
            }
            else if (i == 2)
            {
                QUALITIES_VECTOR->Rho[zInd] = val;
                
            }
        }
    }
    double elyTaperDepth;
    relativeDepth = PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[1] - dep;
    // if GTL and no special offshore smoothing
    if(GLOBAL_MODEL_PARAMETERS->GTL == 1 &&  NZ_TOMOGRAPHY_DATA->specialOffshoreTapering == 0)
    {
        if (relativeDepth <= 350)
        {
            elyTaperDepth = 350;
            v30gtl(MESH_VECTOR->Vs30, QUALITIES_VECTOR->Vs[zInd], relativeDepth, elyTaperDepth, QUALITIES_VECTOR, zInd);
            
        }
    }
    // if GTL AND special offshore smoothing
    else if(GLOBAL_MODEL_PARAMETERS->GTL == 1 &&  NZ_TOMOGRAPHY_DATA->specialOffshoreTapering == 1)
    {
        if (relativeDepth <= 1000 && MESH_VECTOR->Vs30 < 100 && inAnyBasinLatLon == 0 && onBoundary != 1) // if less than 1km and offshore (vs30 in offshore = 50m/s, allow some variability for interpolation)
        {
            elyTaperDepth = 1000;
            v30gtl(MESH_VECTOR->Vs30, QUALITIES_VECTOR->Vs[zInd], relativeDepth, elyTaperDepth, QUALITIES_VECTOR, zInd);
            
        }
        else if (relativeDepth <= 350) // if not offshore, apply regular taper
        {
            elyTaperDepth = 350;
            v30gtl(MESH_VECTOR->Vs30, QUALITIES_VECTOR->Vs[zInd], relativeDepth, elyTaperDepth, QUALITIES_VECTOR, zInd);
            
        }
//        else
//        {
//            printf("Point within seismic tomography offshore tapering failed. Exiting.\n");
//            exit(EXIT_FAILURE);
//        }
    }

    free(ADJACENT_POINTS);
}

void loadEPtomoSurfaceData(char *tomoType, nz_tomography_data *NZ_TOMOGRAPHY_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS)
/*
 Purpose:   read in the Eberhart-Phillips 2010 tomography dataset
 
 Input variables:
 *tomoType - strimg containing the type of tomography to load
 *NZ_TOMOGRAPHY_DATA - struct containing tomography sub velocity model data (tomography surfaces depths etc)
 
 Output variables:
 n.a.
 */
{
    const char *varNames[3];
    varNames[0] = "vp", varNames[1] = "vs", varNames[2] = "rho";
    int elev[30];
    int nElev = 0;
    char vs30fileName[MAX_FILENAME_STRING_LEN];
    char tomoDirectory[MAX_FILENAME_STRING_LEN];

    if(strcmp(tomoType, "2010_Full_South_Island") == 0)
    {
        nElev = 20; // read in only the necessary surfaces
        elev[0] = 15;
        elev[1] = 1;
        elev[2] = -3;
        elev[3] = -8;
        elev[4] = -15;
        elev[5] = -23;
        elev[6] = -30;
        elev[7] = -38;
        elev[8] = -48;
        elev[9] = -65;
        elev[10] = -85;
        elev[11] = -105;
        elev[12] = -130;
        elev[13] = -155;
        elev[14] = -185;
        elev[15] = -225;
        elev[16] = -275;
        elev[17] = -370;
        elev[18] = -620;
        elev[19] = -750;
        
        sprintf(tomoDirectory,"2010_Full_South_Island");
        NZ_TOMOGRAPHY_DATA->specialOffshoreTapering = 0;
    }
    else if (strcmp(tomoType, "2010_Full_North_Island") == 0)
    {
        nElev = 20; // read in only the necessary surfaces
        elev[0] = 15;
        elev[1] = 1;
        elev[2] = -3;
        elev[3] = -8;
        elev[4] = -15;
        elev[5] = -23;
        elev[6] = -30;
        elev[7] = -38;
        elev[8] = -48;
        elev[9] = -65;
        elev[10] = -85;
        elev[11] = -105;
        elev[12] = -130;
        elev[13] = -155;
        elev[14] = -185;
        elev[15] = -225;
        elev[16] = -275;
        elev[17] = -370;
        elev[18] = -620;
        elev[19] = -750;
        printf("Loading North Island Tomography.\n");
        sprintf(tomoDirectory,"2010_Full_North_Island");
        NZ_TOMOGRAPHY_DATA->specialOffshoreTapering = 0;


        
    }
    else if (strcmp(tomoType, "2010_NZ") == 0)
    {
        nElev = 20; // read in only the necessary surfaces
        elev[0] = 15;
        elev[1] = 1;
        elev[2] = -3;
        elev[3] = -8;
        elev[4] = -15;
        elev[5] = -23;
        elev[6] = -30;
        elev[7] = -38;
        elev[8] = -48;
        elev[9] = -65;
        elev[10] = -85;
        elev[11] = -105;
        elev[12] = -130;
        elev[13] = -155;
        elev[14] = -185;
        elev[15] = -225;
        elev[16] = -275;
        elev[17] = -370;
        elev[18] = -620;
        elev[19] = -750;
        printf("Loading NZ Tomography.\n");
        
        // load in Vs30 NZ surface
        sprintf(vs30fileName,"Data/Global_Surfaces/NZ_Vs30.in");
        NZ_TOMOGRAPHY_DATA->Vs30 = loadGlobalSurface(vs30fileName);
        
        sprintf(tomoDirectory,"2010_NZ");
        NZ_TOMOGRAPHY_DATA->specialOffshoreTapering = 0;

    }
    
    else if (strcmp(tomoType, "2010_NZ_OFFSHORE") == 0)
    {
        nElev = 20; // read in only the necessary surfaces
        elev[0] = 15;
        elev[1] = 1;
        elev[2] = -3;
        elev[3] = -8;
        elev[4] = -15;
        elev[5] = -23;
        elev[6] = -30;
        elev[7] = -38;
        elev[8] = -48;
        elev[9] = -65;
        elev[10] = -85;
        elev[11] = -105;
        elev[12] = -130;
        elev[13] = -155;
        elev[14] = -185;
        elev[15] = -225;
        elev[16] = -275;
        elev[17] = -370;
        elev[18] = -620;
        elev[19] = -750;
        printf("Loading NZ Tomography.\n");
        
        // load in Vs30 NZ surface
        sprintf(vs30fileName,"Data/Global_Surfaces/NZ_Vs30_HD_With_Offshore.in");
        NZ_TOMOGRAPHY_DATA->Vs30 = loadGlobalSurface(vs30fileName);
        
        sprintf(tomoDirectory,"2010_NZ");
        NZ_TOMOGRAPHY_DATA->specialOffshoreTapering = 1;

    }
    else if (strcmp(tomoType, "2010_NZ_OFFSHORE_EXTENDED") == 0)
    {
        nElev = 20; // read in only the necessary surfaces
        elev[0] = 15;
        elev[1] = 1;
        elev[2] = -3;
        elev[3] = -8;
        elev[4] = -15;
        elev[5] = -23;
        elev[6] = -30;
        elev[7] = -38;
        elev[8] = -48;
        elev[9] = -65;
        elev[10] = -85;
        elev[11] = -105;
        elev[12] = -130;
        elev[13] = -155;
        elev[14] = -185;
        elev[15] = -225;
        elev[16] = -275;
        elev[17] = -370;
        elev[18] = -620;
        elev[19] = -750;
        printf("Loading NZ Tomography.\n");
        
        // load in Vs30 NZ surface
        sprintf(vs30fileName,"Data/Global_Surfaces/NZ_Vs30_HD_With_Offshore_extended.in");
        NZ_TOMOGRAPHY_DATA->Vs30 = loadGlobalSurface(vs30fileName);
        
        sprintf(tomoDirectory,"2010_NZ_EXTENDED");
        NZ_TOMOGRAPHY_DATA->specialOffshoreTapering = 1;
        
    }
    
    char baseFilename[MAX_FILENAME_STRING_LEN];
    
    NZ_TOMOGRAPHY_DATA->nSurf = nElev;
    assert(NZ_TOMOGRAPHY_DATA->nSurf<=MAX_NUM_TOMO_SURFACES);
    
    for(int i = 0; i < nElev; i++)
    {
        NZ_TOMOGRAPHY_DATA->surfDeps[i] = elev[i]; // depth in km
        for(int j = 0; j < 3; j++)
        {
            sprintf(baseFilename,"Data/Tomography/%s/surf_tomography_%s_elev%i.in",tomoDirectory,varNames[j],elev[i]);
            // read the surface
            NZ_TOMOGRAPHY_DATA->surf[j][i] = loadGlobalSurface(baseFilename);
            
        }
    }

    
    // load in vector containing basin 'wall-type' boundaries to apply smoothing near
    NZ_TOMOGRAPHY_DATA->smooth_boundary = malloc(sizeof(smoothing_boundary));
    loadSmoothBoundaries(NZ_TOMOGRAPHY_DATA,GLOBAL_MODEL_PARAMETERS);

}



void freeEPtomoSurfaceData(nz_tomography_data *NZ_TOMOGRAPHY_DATA)
/*
 Purpose:   free surfaces in the the Eberhart-Phillips 2010 tomography dataset
 
 Input variables:
 
 Output variables:
 n.a.
 */
{
    
    for(int i = 0; i < NZ_TOMOGRAPHY_DATA->nSurf; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            free(NZ_TOMOGRAPHY_DATA->surf[j][i]);
            
        }
    }
    free(NZ_TOMOGRAPHY_DATA->Vs30);
    free(NZ_TOMOGRAPHY_DATA->smooth_boundary);
}

void calculateVs30FromTomoVs30Surface(mesh_vector *MESH_VECTOR, nz_tomography_data *NZ_TOMOGRAPHY_DATA)
{
    double X1, X2, Y1, Y2, Q11, Q12, Q21, Q22, X, Y;
    adjacent_points *ADJACENT_POINTS;
    global_surf_read *GLOBAL_SURF_READ;
    GLOBAL_SURF_READ = NZ_TOMOGRAPHY_DATA->Vs30;
    
    // Vs30 calculation
    ADJACENT_POINTS = findGlobalAdjacentPoints(GLOBAL_SURF_READ, *MESH_VECTOR->Lat, *MESH_VECTOR->Lon);
    if(ADJACENT_POINTS->inSurfaceBounds == 0)
    {
        printf("Grid point (%lf %lf) lies outside of global Vs30 surface bounds.\n",*MESH_VECTOR->Lon,*MESH_VECTOR->Lat);
        exit(EXIT_FAILURE);
    }
    X1 = GLOBAL_SURF_READ->loni[ADJACENT_POINTS->lonInd[0]];
    X2 = GLOBAL_SURF_READ->loni[ADJACENT_POINTS->lonInd[1]];
    Y1 = GLOBAL_SURF_READ->lati[ADJACENT_POINTS->latInd[0]];
    Y2 = GLOBAL_SURF_READ->lati[ADJACENT_POINTS->latInd[1]];
    Q11 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[0]];
    Q12 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[1]];
    Q21 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[0]];
    Q22 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[1]];
    X = *MESH_VECTOR->Lon;
    Y = *MESH_VECTOR->Lat;
    MESH_VECTOR->Vs30 =  biLinearInterpolation(X1, X2, Y1, Y2, Q11, Q12, Q21, Q22, X, Y);
    free(ADJACENT_POINTS);
}


