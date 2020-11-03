//
//  perturbationubMod.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 02/06/20.
//  Copyright (c) 2020 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"

void perturbationSubMod(int zInd, double dep, mesh_vector *MESH_VECTOR, qualities_vector *QUALITIES_VECTOR, nz_tomography_data *NZ_TOMOGRAPHY_DATA, nz_tomography_data *PERTURBTION_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, int inAnyBasinLatLon, int onBoundary)
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
    EPtomo2010subMod(zInd, dep, MESH_VECTOR, QUALITIES_VECTOR, NZ_TOMOGRAPHY_DATA, GLOBAL_MODEL_PARAMETERS, PARTIAL_GLOBAL_SURFACE_DEPTHS, inAnyBasinLatLon, onBoundary);

    int count = 0;
    double relativeDepth;
    // find the indice of the first "surface" above the data point in question
    while(dep < PERTURBTION_DATA->surfDeps[count]*1000)
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
    
    double valAbove, valBelow;
    double depAbove, depBelow;
    double val;
    
    // find the adjscent points for interpolation from the first surface (assume all surfaces utilise the same grid)
    ADJACENT_POINTS = malloc(sizeof(adjacent_points));
    findGlobalAdjacentPoints(PERTURBTION_DATA->surf[0][0], *MESH_VECTOR->Lat, *MESH_VECTOR->Lon,ADJACENT_POINTS);
    
//    printf("Grid point (%lf %lf %lf) \n",*MESH_VECTOR->Lon,*MESH_VECTOR->Lat, dep);


    for( int i = 0; i < 3; i++)
    {
        SURFACE_POINTER_ABOVE = PERTURBTION_DATA->surf[i][indAbove];
        
        SURFACE_POINTER_BELOW = PERTURBTION_DATA->surf[i][indBelow];
        
        valAbove = interpolateGlobalSurface(SURFACE_POINTER_ABOVE,*MESH_VECTOR->Lat, *MESH_VECTOR->Lon, ADJACENT_POINTS);
        valBelow = interpolateGlobalSurface(SURFACE_POINTER_BELOW,*MESH_VECTOR->Lat, *MESH_VECTOR->Lon, ADJACENT_POINTS);
        depAbove = PERTURBTION_DATA->surfDeps[indAbove]*1000;
        depBelow = PERTURBTION_DATA->surfDeps[indBelow]*1000;
        val = linearInterpolation(depAbove, depBelow, valAbove, valBelow, dep);
        if (i == 0)
        {
            QUALITIES_VECTOR->Vp[zInd] = QUALITIES_VECTOR->Vp[zInd] * val;
                
        }
        else if ( i == 1)
        {
            QUALITIES_VECTOR->Vs[zInd] = QUALITIES_VECTOR->Vs[zInd] * val; 
                
        }
        else if (i == 2)
        {
            QUALITIES_VECTOR->Rho[zInd] = QUALITIES_VECTOR->Rho[zInd] * val;
                
        }
    }
    
    free(ADJACENT_POINTS);

}

nz_tomography_data *loadPerturbationSurfaceData(char *tomoType, global_model_parameters *GLOBAL_MODEL_PARAMETERS)
/*
 Purpose:   read in the perturbation dataset
 
 Input variables:
 *tomoType - strimg containing the type of tomography to load
 
 Output variables:
 *NZ_TOMOGRAPHY_DATA - struct containing tomography sub velocity model data (tomography surfaces depths etc)

 */
{
    const char *varNames[3];
    varNames[0] = "vp";
    varNames[1] = "vs";
    varNames[2] = "rho";
    int elev[MAX_NUM_TOMO_SURFACES];
    int nElev = 0;

    nz_tomography_data *NZ_TOMOGRAPHY_DATA;
    NZ_TOMOGRAPHY_DATA = malloc(sizeof(nz_tomography_data));

    if (NZ_TOMOGRAPHY_DATA == NULL)
    {
        printf("Memory allocation of NZ_TOMOGRAPHY_DATA failed.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(tomoType, "v20p6") == 0)
    {
        
        nElev = 61; // read in only the necessary surface
        elev[0] = 15;
        elev[1] = -2;
        elev[2] = -6;
        elev[3] = -10;
        elev[4] = -14;
        elev[5] = -18;
        elev[6] = -22;
        elev[7] = -26;
        elev[8] = -30;
        elev[9] = -34;
        elev[10] = -38;
        elev[11] = -42;
        elev[12] = -46;
        elev[13] = -50;
        elev[14] = -54;
        elev[15] = -58;
        elev[16] = -62;
        elev[17] = -66;
        elev[18] = -70;
        elev[19] = -74;
        elev[20] = -78;
        elev[21] = -82;
        elev[22] = -86;
        elev[23] = -90;
        elev[24] = -94;
        elev[25] = -98;
        elev[26] = -102;
        elev[27] = -106;
        elev[28] = -110;
        elev[29] = -114;
        elev[30] = -118;
        elev[31] = -122;
        elev[32] = -126;
        elev[33] = -130;
        elev[34] = -134;
        elev[35] = -138;
        elev[36] = -142;
        elev[37] = -146;
        elev[38] = -150;
        elev[39] = -154;
        elev[40] = -158;
        elev[41] = -162;
        elev[42] = -166;
        elev[43] = -170;
        elev[44] = -174;
        elev[45] = -178;
        elev[46] = -182;
        elev[47] = -186;
        elev[48] = -190;
        elev[49] = -194;
        elev[50] = -198;
        elev[51] = -202;
        elev[52] = -206;
        elev[53] = -210;
        elev[54] = -214;
        elev[55] = -218;
        elev[56] = -222;
        elev[57] = -226;
        elev[58] = -230;
        elev[59] = -234;
        elev[60] = -238;

    }
    else if (strcmp(tomoType, "v20p10") == 0)
    {
        
        nElev = 61; // read in only the necessary surface
        elev[0] = 15;
        elev[1] = -2;
        elev[2] = -6;
        elev[3] = -10;
        elev[4] = -14;
        elev[5] = -18;
        elev[6] = -22;
        elev[7] = -26;
        elev[8] = -30;
        elev[9] = -34;
        elev[10] = -38;
        elev[11] = -42;
        elev[12] = -46;
        elev[13] = -50;
        elev[14] = -54;
        elev[15] = -58;
        elev[16] = -62;
        elev[17] = -66;
        elev[18] = -70;
        elev[19] = -74;
        elev[20] = -78;
        elev[21] = -82;
        elev[22] = -86;
        elev[23] = -90;
        elev[24] = -94;
        elev[25] = -98;
        elev[26] = -102;
        elev[27] = -106;
        elev[28] = -110;
        elev[29] = -114;
        elev[30] = -118;
        elev[31] = -122;
        elev[32] = -126;
        elev[33] = -130;
        elev[34] = -134;
        elev[35] = -138;
        elev[36] = -142;
        elev[37] = -146;
        elev[38] = -150;
        elev[39] = -154;
        elev[40] = -158;
        elev[41] = -162;
        elev[42] = -166;
        elev[43] = -170;
        elev[44] = -174;
        elev[45] = -178;
        elev[46] = -182;
        elev[47] = -186;
        elev[48] = -190;
        elev[49] = -194;
        elev[50] = -198;
        elev[51] = -202;
        elev[52] = -206;
        elev[53] = -210;
        elev[54] = -214;
        elev[55] = -218;
        elev[56] = -222;
        elev[57] = -226;
        elev[58] = -230;
        elev[59] = -234;
        elev[60] = -238;

    }
    else if (strcmp(tomoType, "v20p11") == 0)
    {
        nElev = 61; // read in only the necessary surface
        elev[0] = 15;
        elev[1] = -2;
        elev[2] = -6;
        elev[3] = -10;
        elev[4] = -14;
        elev[5] = -18;
        elev[6] = -22;
        elev[7] = -26;
        elev[8] = -30;
        elev[9] = -34;
        elev[10] = -38;
        elev[11] = -42;
        elev[12] = -46;
        elev[13] = -50;
        elev[14] = -54;
        elev[15] = -58;
        elev[16] = -62;
        elev[17] = -66;
        elev[18] = -70;
        elev[19] = -74;
        elev[20] = -78;
        elev[21] = -82;
        elev[22] = -86;
        elev[23] = -90;
        elev[24] = -94;
        elev[25] = -98;
        elev[26] = -102;
        elev[27] = -106;
        elev[28] = -110;
        elev[29] = -114;
        elev[30] = -118;
        elev[31] = -122;
        elev[32] = -126;
        elev[33] = -130;
        elev[34] = -134;
        elev[35] = -138;
        elev[36] = -142;
        elev[37] = -146;
        elev[38] = -150;
        elev[39] = -154;
        elev[40] = -158;
        elev[41] = -162;
        elev[42] = -166;
        elev[43] = -170;
        elev[44] = -174;
        elev[45] = -178;
        elev[46] = -182;
        elev[47] = -186;
        elev[48] = -190;
        elev[49] = -194;
        elev[50] = -198;
        elev[51] = -202;
        elev[52] = -206;
        elev[53] = -210;
        elev[54] = -214;
        elev[55] = -218;
        elev[56] = -222;
        elev[57] = -226;
        elev[58] = -230;
        elev[59] = -234;
        elev[60] = -238;

    }
    printf("Loading %s perturbation files.\n",tomoType);

    
    char baseFilename[MAX_FILENAME_STRING_LEN];
    
    NZ_TOMOGRAPHY_DATA->nSurf = nElev;
    assert(NZ_TOMOGRAPHY_DATA->nSurf<=MAX_NUM_TOMO_SURFACES);
    
    for(int i = 0; i < nElev; i++)
    {
        NZ_TOMOGRAPHY_DATA->surfDeps[i] = elev[i]; // depth in km
        for(int j = 0; j < 3; j++)
        {
            sprintf(baseFilename,"Data/Perturbations/%s/perturbation_surface_files/perturb_surf_%s_elev%i.in",tomoType,varNames[j],elev[i]);
            // read the surface
            NZ_TOMOGRAPHY_DATA->surf[j][i] = loadGlobalSurface(baseFilename);            
        }
    }
    printf("Completed Loading of perturbation files.\n");
    return(NZ_TOMOGRAPHY_DATA);
}



void freePerturbationSurfaceData(nz_tomography_data *NZ_TOMOGRAPHY_DATA)
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
}


