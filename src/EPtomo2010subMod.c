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
    
    double valAbove, valBelow;
    double depAbove, depBelow;
    double val;
    
    // find the adjscent points for interpolatin from the first surface (assume all surfaces utilise the same grid)
    ADJACENT_POINTS = malloc(sizeof(adjacent_points));
    findGlobalAdjacentPoints(NZ_TOMOGRAPHY_DATA->surf[0][0], *MESH_VECTOR->Lat, *MESH_VECTOR->Lon,ADJACENT_POINTS);
    
//    printf("Grid point (%lf %lf %lf) \n",*MESH_VECTOR->Lon,*MESH_VECTOR->Lat, dep);


    for( int i = 0; i < 3; i++)
    {

        SURFACE_POINTER_ABOVE = NZ_TOMOGRAPHY_DATA->surf[i][indAbove];
        
        SURFACE_POINTER_BELOW = NZ_TOMOGRAPHY_DATA->surf[i][indBelow];
        
        valAbove = interpolateGlobalSurface(SURFACE_POINTER_ABOVE,*MESH_VECTOR->Lat, *MESH_VECTOR->Lon, ADJACENT_POINTS);
        valBelow = interpolateGlobalSurface(SURFACE_POINTER_BELOW,*MESH_VECTOR->Lat, *MESH_VECTOR->Lon, ADJACENT_POINTS);
        

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
    
    double elyTaperDepth;
    relativeDepth = PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[1] - dep; // DEM minus the depth of the point
    //printf("%lf \n",MESH_VECTOR->distFromShoreline);

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
    else if (GLOBAL_MODEL_PARAMETERS->GTL == 1 &&  NZ_TOMOGRAPHY_DATA->specialOffshoreTapering == 1)
    {   // if less than 1km and offshore (vs30 in offshore = 50m/s, allow some variability for interpolation)
        if (MESH_VECTOR->Vs30 < 100 && inAnyBasinLatLon == 0 && onBoundary != 1 && 0 < MESH_VECTOR->distFromShoreline) 
        {
           offShoreBasinModel(MESH_VECTOR->distFromShoreline, dep, QUALITIES_VECTOR, zInd, NZ_TOMOGRAPHY_DATA->offshoreBasinModel1D);
            
        }
        else if (relativeDepth <= 350) // if not offshore, apply regular taper
        {
            elyTaperDepth = 350; // onshore smoothing ely taper depth (350m)
            v30gtl(MESH_VECTOR->Vs30, QUALITIES_VECTOR->Vs[zInd], relativeDepth, elyTaperDepth, QUALITIES_VECTOR, zInd);
            
        }
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
    varNames[0] = "vp";
    varNames[1] = "vs";
    varNames[2] = "rho";
    int elev[MAX_NUM_TOMO_SURFACES];
    int nElev = 0;
    char vs30fileName[MAX_FILENAME_STRING_LEN];
    char offshorefileName[MAX_FILENAME_STRING_LEN];
    char offshore1DModel[MAX_FILENAME_STRING_LEN];
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
    else if (strcmp(tomoType, "2020_NZ_OFFSHORE") == 0)
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
        
        sprintf(tomoDirectory,"2020_NZ");
        NZ_TOMOGRAPHY_DATA->specialOffshoreTapering = 1;

    }
    else if (strcmp(tomoType, "2020_NZ_OFFSHORE_NO_BASIN") == 0)
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
        
        sprintf(tomoDirectory,"2020_NZ");
        NZ_TOMOGRAPHY_DATA->specialOffshoreTapering = 0;

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
    else if (strcmp(tomoType, "2020_Chow_NI") == 0)
    {
        nElev = 143; // read in only the necessary surfaces
        elev[0] = 15;
        elev[1] = 3;
        elev[2] = 2;
        elev[3] = 1;
        elev[4] = 0;
        elev[5] = -1;
        elev[6] = -2;
        elev[7] = -3;
        elev[8] = -4;
        elev[9] = -5;
        elev[10] = -6;
        elev[11] = -7;
        elev[12] = -8;
        elev[13] = -9;
        elev[14] = -10;
        elev[15] = -11;
        elev[16] = -12;
        elev[17] = -13;
        elev[18] = -14;
        elev[19] = -15;
        elev[20] = -16;
        elev[21] = -17;
        elev[22] = -18;
        elev[23] = -19;
        elev[24] = -20;
        elev[25] = -21;
        elev[26] = -22;
        elev[27] = -23;
        elev[28] = -24;
        elev[29] = -25;
        elev[30] = -26;
        elev[31] = -27;
        elev[32] = -28;
        elev[33] = -29;
        elev[34] = -30;
        elev[35] = -31;
        elev[36] = -32;
        elev[37] = -33;
        elev[38] = -34;
        elev[39] = -35;
        elev[40] = -36;
        elev[41] = -37;
        elev[42] = -38;
        elev[43] = -39;
        elev[44] = -40;
        elev[45] = -41;
        elev[46] = -42;
        elev[47] = -43;
        elev[48] = -44;
        elev[49] = -45;
        elev[50] = -46;
        elev[51] = -47;
        elev[52] = -48;
        elev[53] = -52;
        elev[54] = -56;
        elev[55] = -60;
        elev[56] = -64;
        elev[57] = -68;
        elev[58] = -72;
        elev[59] = -76;
        elev[60] = -80;
        elev[61] = -84;
        elev[62] = -88;
        elev[63] = -92;
        elev[64] = -96;
        elev[65] = -100;
        elev[66] = -104;
        elev[67] = -108;
        elev[68] = -112;
        elev[69] = -116;
        elev[70] = -120;
        elev[71] = -124;
        elev[72] = -128;
        elev[73] = -132;
        elev[74] = -136;
        elev[75] = -140;
        elev[76] = -144;
        elev[77] = -148;
        elev[78] = -152;
        elev[79] = -156;
        elev[80] = -160;
        elev[81] = -164;
        elev[82] = -168;
        elev[83] = -172;
        elev[84] = -176;
        elev[85] = -180;
        elev[86] = -184;
        elev[87] = -188;
        elev[88] = -192;
        elev[89] = -196;
        elev[90] = -200;
        elev[91] = -204;
        elev[92] = -208;
        elev[93] = -212;
        elev[94] = -216;
        elev[95] = -220;
        elev[96] = -224;
        elev[97] = -228;
        elev[98] = -232;
        elev[99] = -236;
        elev[100] = -240;
        elev[101] = -244;
        elev[102] = -248;
        elev[103] = -252;
        elev[104] = -256;
        elev[105] = -260;
        elev[106] = -264;
        elev[107] = -268;
        elev[108] = -272;
        elev[109] = -276;
        elev[110] = -280;
        elev[111] = -284;
        elev[112] = -288;
        elev[113] = -292;
        elev[114] = -296;
        elev[115] = -300;
        elev[116] = -304;
        elev[117] = -308;
        elev[118] = -312;
        elev[119] = -316;
        elev[120] = -320;
        elev[121] = -324;
        elev[122] = -328;
        elev[123] = -332;
        elev[124] = -336;
        elev[125] = -340;
        elev[126] = -344;
        elev[127] = -348;
        elev[128] = -352;
        elev[129] = -356;
        elev[130] = -360;
        elev[131] = -364;
        elev[132] = -368;
        elev[133] = -372;
        elev[134] = -376;
        elev[135] = -380;
        elev[136] = -384;
        elev[137] = -388;
        elev[138] = -392;
        elev[139] = -396;
        elev[140] = -400;
        elev[141] = -620;
        elev[142] = -750;
        printf("Loading NZ Tomography.\n");
        
        // load in Vs30 NZ surface
        sprintf(vs30fileName,"Data/Global_Surfaces/NZ_Vs30_HD_With_Offshore.in");
        NZ_TOMOGRAPHY_DATA->Vs30 = loadGlobalSurface(vs30fileName);
        
        sprintf(tomoDirectory,"2020_Chow_NI");
        NZ_TOMOGRAPHY_DATA->specialOffshoreTapering = 1;
        
    }
       
    else if (strcmp(tomoType, "2020_Korea") == 0)
    {
        nElev = 19; // read in only the necessary surfaces
        elev[0] = 5;
        elev[1] = -4;
        elev[2] = -8;
        elev[3] = -12;
        elev[4] = -16;
        elev[5] = -20;
        elev[6] = -24;
        elev[7] = -28;
        elev[8] = -32;
        elev[9] = -36;
        elev[10] = -40;
        elev[11] = -44;
        elev[12] = -48;
        elev[13] = -52;
        elev[14] = -56;
        elev[15] = -60;
        elev[16] = -64;
        elev[17] = -68;
        elev[18] = -680;
        printf("Loading Korean Tomography.\n");
        
        // load in Vs30 NZ surface
        sprintf(vs30fileName,"Data/Global_Surfaces/NZ_Vs30_HD_With_Offshore.in");
        NZ_TOMOGRAPHY_DATA->Vs30 = loadGlobalSurface(vs30fileName);
        
        sprintf(tomoDirectory,"2020_Korea");
        NZ_TOMOGRAPHY_DATA->specialOffshoreTapering = 0;

    }
    else 
    {
        printf("Tomography type: %s, not recognized.\n", tomoType);
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


    // load in offshore distance surface file
    sprintf(offshorefileName,"Data/Global_Surfaces/shoreline_distance_2k.in");
    NZ_TOMOGRAPHY_DATA->offshoreDistance = loadGlobalSurface(offshorefileName);
    
    NZ_TOMOGRAPHY_DATA->offshoreBasinModel1D = malloc(sizeof(velo_mod_1d_data));
    if (NZ_TOMOGRAPHY_DATA->offshoreBasinModel1D == NULL)
    {
        printf("Memory allocation of VELO_MOD_1D_DATA for tomography offshore basin failed.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(offshore1DModel,"Cant1D_v2.fd_modfile");
    load1dVeloSubModel(offshore1DModel, NZ_TOMOGRAPHY_DATA->offshoreBasinModel1D);
    NZ_TOMOGRAPHY_DATA->tomography_loaded = 1; 
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
    free(NZ_TOMOGRAPHY_DATA->offshoreDistance);
    free(NZ_TOMOGRAPHY_DATA->smooth_boundary);
}

void calculateVs30FromTomoVs30Surface(mesh_vector *MESH_VECTOR, nz_tomography_data *NZ_TOMOGRAPHY_DATA)
{
    adjacent_points *ADJACENT_POINTS;
    global_surf_read *GLOBAL_SURF_READ;
    GLOBAL_SURF_READ = NZ_TOMOGRAPHY_DATA->Vs30;
    
    // Vs30 calculation
    ADJACENT_POINTS = malloc(sizeof(adjacent_points));
    findGlobalAdjacentPoints(GLOBAL_SURF_READ, *MESH_VECTOR->Lat, *MESH_VECTOR->Lon, ADJACENT_POINTS);
    
    MESH_VECTOR->Vs30 = interpolateGlobalSurface(GLOBAL_SURF_READ,*MESH_VECTOR->Lat, *MESH_VECTOR->Lon, ADJACENT_POINTS);
    free(ADJACENT_POINTS);
}

void calculateShorelineDist(mesh_vector *MESH_VECTOR, nz_tomography_data *NZ_TOMOGRAPHY_DATA)
{
    adjacent_points *ADJACENT_POINTS;
    global_surf_read *GLOBAL_SURF_READ;
    GLOBAL_SURF_READ = NZ_TOMOGRAPHY_DATA->offshoreDistance;
    
    // shoreline distance calculation
    ADJACENT_POINTS = malloc(sizeof(adjacent_points));
    findGlobalAdjacentPoints(GLOBAL_SURF_READ, *MESH_VECTOR->Lat, *MESH_VECTOR->Lon, ADJACENT_POINTS);
    
    MESH_VECTOR->distFromShoreline = interpolateGlobalSurface(GLOBAL_SURF_READ,*MESH_VECTOR->Lat, *MESH_VECTOR->Lon, ADJACENT_POINTS);
    free(ADJACENT_POINTS);
}



