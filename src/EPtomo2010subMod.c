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
    else if (strcmp(tomoType, "2023_Chow_NI") == 0)
    {

        nElev = 175;
        elev[0] = 15.0;
        elev[1] = 2.25;
        elev[2] = 2.0;
        elev[3] = 1.75;
        elev[4] = 1.5;
        elev[5] = 1.25;
        elev[6] = 1.0;
        elev[7] = 0.75;
        elev[8] = 0.5;
        elev[9] = 0.25;
        elev[10] = 0.0;
        elev[11] = -0.25;
        elev[12] = -0.5;
        elev[13] = -0.75;
        elev[14] = -1.0;
        elev[15] = -1.25;
        elev[16] = -1.5;
        elev[17] = -1.75;
        elev[18] = -2.0;
        elev[19] = -2.25;
        elev[20] = -2.5;
        elev[21] = -2.75;
        elev[22] = -3.0;
        elev[23] = -3.25;
        elev[24] = -3.5;
        elev[25] = -3.75;
        elev[26] = -4.0;
        elev[27] = -4.25;
        elev[28] = -4.5;
        elev[29] = -4.75;
        elev[30] = -5.0;
        elev[31] = -5.25;
        elev[32] = -5.5;
        elev[33] = -5.75;
        elev[34] = -6.0;
        elev[35] = -6.25;
        elev[36] = -6.5;
        elev[37] = -6.75;
        elev[38] = -7.0;
        elev[39] = -7.25;
        elev[40] = -7.5;
        elev[41] = -7.75;
        elev[42] = -8.0;
        elev[43] = -9.0;
        elev[44] = -10.0;
        elev[45] = -11.0;
        elev[46] = -12.0;
        elev[47] = -13.0;
        elev[48] = -14.0;
        elev[49] = -15.0;
        elev[50] = -16.0;
        elev[51] = -17.0;
        elev[52] = -18.0;
        elev[53] = -19.0;
        elev[54] = -20.0;
        elev[55] = -21.0;
        elev[56] = -22.0;
        elev[57] = -23.0;
        elev[58] = -24.0;
        elev[59] = -25.0;
        elev[60] = -26.0;
        elev[61] = -27.0;
        elev[62] = -28.0;
        elev[63] = -29.0;
        elev[64] = -30.0;
        elev[65] = -31.0;
        elev[66] = -32.0;
        elev[67] = -33.0;
        elev[68] = -34.0;
        elev[69] = -35.0;
        elev[70] = -36.0;
        elev[71] = -37.0;
        elev[72] = -38.0;
        elev[73] = -39.0;
        elev[74] = -40.0;
        elev[75] = -41.0;
        elev[76] = -42.0;
        elev[77] = -43.0;
        elev[78] = -44.0;
        elev[79] = -45.0;
        elev[80] = -46.0;
        elev[81] = -47.0;
        elev[82] = -48.0;
        elev[83] = -49.0;
        elev[84] = -50.0;
        elev[85] = -52.0;
        elev[86] = -56.0;
        elev[87] = -60.0;
        elev[88] = -64.0;
        elev[89] = -68.0;
        elev[90] = -72.0;
        elev[91] = -76.0;
        elev[92] = -80.0;
        elev[93] = -84.0;
        elev[94] = -88.0;
        elev[95] = -92.0;
        elev[96] = -96.0;
        elev[97] = -100.0;
        elev[98] = -104.0;
        elev[99] = -108.0;
        elev[100] = -112.0;
        elev[101] = -116.0;
        elev[102] = -120.0;
        elev[103] = -124.0;
        elev[104] = -128.0;
        elev[105] = -132.0;
        elev[106] = -136.0;
        elev[107] = -140.0;
        elev[108] = -144.0;
        elev[109] = -148.0;
        elev[110] = -152.0;
        elev[111] = -156.0;
        elev[112] = -160.0;
        elev[113] = -164.0;
        elev[114] = -168.0;
        elev[115] = -172.0;
        elev[116] = -176.0;
        elev[117] = -180.0;
        elev[118] = -184.0;
        elev[119] = -188.0;
        elev[120] = -192.0;
        elev[121] = -196.0;
        elev[122] = -200.0;
        elev[123] = -204.0;
        elev[124] = -208.0;
        elev[125] = -212.0;
        elev[126] = -216.0;
        elev[127] = -220.0;
        elev[128] = -224.0;
        elev[129] = -228.0;
        elev[130] = -232.0;
        elev[131] = -236.0;
        elev[132] = -240.0;
        elev[133] = -244.0;
        elev[134] = -248.0;
        elev[135] = -252.0;
        elev[136] = -256.0;
        elev[137] = -260.0;
        elev[138] = -264.0;
        elev[139] = -268.0;
        elev[140] = -272.0;
        elev[141] = -276.0;
        elev[142] = -280.0;
        elev[143] = -284.0;
        elev[144] = -288.0;
        elev[145] = -292.0;
        elev[146] = -296.0;
        elev[147] = -300.0;
        elev[148] = -304.0;
        elev[149] = -308.0;
        elev[150] = -312.0;
        elev[151] = -316.0;
        elev[152] = -320.0;
        elev[153] = -324.0;
        elev[154] = -328.0;
        elev[155] = -332.0;
        elev[156] = -336.0;
        elev[157] = -340.0;
        elev[158] = -344.0;
        elev[159] = -348.0;
        elev[160] = -352.0;
        elev[161] = -356.0;
        elev[162] = -360.0;
        elev[163] = -364.0;
        elev[164] = -368.0;
        elev[165] = -372.0;
        elev[166] = -376.0;
        elev[167] = -380.0;
        elev[168] = -384.0;
        elev[169] = -388.0;
        elev[170] = -392.0;
        elev[171] = -396.0;
        elev[172] = -400.0;
        elev[173] = -620.0;
        elev[174] = -750.0;

        printf("Loading NZ Tomography based on EP2020 and Chow NI.\n");
        
        // load in Vs30 NZ surface
        sprintf(vs30fileName,"Data/Global_Surfaces/NZ_Vs30_HD_With_Offshore.in");
        NZ_TOMOGRAPHY_DATA->Vs30 = loadGlobalSurface(vs30fileName);
        
        sprintf(tomoDirectory,"2023_Chow_NI");
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



