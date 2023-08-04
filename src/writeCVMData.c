//
//  writeCVMData.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 24/06/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"

void writeGlobalQualities(char *OUTPUT_DIR, partial_global_mesh *PARTIAL_GLOBAL_MESH, partial_global_qualities *PARTIAL_GLOBAL_QUALITIES, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG, int latInd)

/*
 Purpose:   write the full velocity model to file
 
 Input variables:
 PARTIAL_GLOBAL_MESH        - pointer to structure containing lat lon grid
 globalValues    - pointer to structure containing vp vs and rho for all gridpoints
 
 Output variables:
 N/A.
 */
{
    int writeModelTextFile = 0; // set as (0) for no text file, and (1) to write the full model in text file format
    
    // perform endian check
    int endianInt;
    endianInt = endian();
    
    FILE *fvp, *fvs, *frho, *fmask;
    char vp3dfile[MAX_FILENAME_STRING_LEN];
    sprintf(vp3dfile,"%s/Velocity_Model/vp3dfile.p",OUTPUT_DIR);
    
    char vs3dfile[MAX_FILENAME_STRING_LEN];
    sprintf(vs3dfile,"%s/Velocity_Model/vs3dfile.s",OUTPUT_DIR);
    
    char rho3dfile[MAX_FILENAME_STRING_LEN];
    sprintf(rho3dfile,"%s/Velocity_Model/rho3dfile.d",OUTPUT_DIR);
    
    char inBasinMaskFile[MAX_FILENAME_STRING_LEN];
    sprintf(inBasinMaskFile,"%s/Velocity_Model/in_basin_mask.b",OUTPUT_DIR);
    
    float *vp, *vs, *rho, *inbasin;
    float vpTemp, vsTemp, rhoTemp, inbasinTemp;
    float vpWrite, vsWrite, rhoWrite, inbasinWrite;
    
    char fullMod[MAX_FILENAME_STRING_LEN];
    FILE *fullModTxt = NULL;
    sprintf(fullMod,"%s/Velocity_Model/fullMod.txt",OUTPUT_DIR);
    
    int bsize;
    
    
    if( latInd == 0) // if first time, generate binary files
    {
        fvp = fopen(vp3dfile,"w");
        fvs = fopen(vs3dfile,"w");
        frho = fopen(rho3dfile,"w");
        fmask = fopen(inBasinMaskFile,"w");
        if (fvp == NULL)
        {
            printf("Unable to generate binary files for write.\n");
            exit(EXIT_FAILURE);
        }
        if (writeModelTextFile == 1)
        {
            fullModTxt = fopen(fullMod, "w");
            fprintf(fullModTxt,"Lat\tLon\tX(km)\tY(km)\tDepth(km)\tVp (km/s)\tVs (km/s)\tRho\t\n");
        }
        
    }
    else // append to existing binary files
    {
        fvp = fopen(vp3dfile,"a");
        fvs = fopen(vs3dfile,"a");
        frho = fopen(rho3dfile,"a");
        fmask = fopen(inBasinMaskFile,"a");
        
        if (fvp == NULL)
        {
            printf("Unable to reopen binary files for write.\n");
            exit(EXIT_FAILURE);
        }
        if (writeModelTextFile == 1)
        {
            fullModTxt = fopen(fullMod,"a");
        }
        
    }
    
    bsize = PARTIAL_GLOBAL_MESH->nX*PARTIAL_GLOBAL_MESH->nZ*sizeof(float);
    vp = (float*) malloc(bsize);
    vs = (float*) malloc(bsize);
    rho = (float*) malloc(bsize);
    inbasin = (float*) malloc(bsize);
    
    for(int iz = 0; iz < PARTIAL_GLOBAL_MESH->nZ; iz++)
    {
        for (int ix = 0; ix < PARTIAL_GLOBAL_MESH->nX; ix++)
        {
            //				ip = ix + iz * PARTIAL_GLOBAL_MESH->nX;  //index counter
            if (PARTIAL_GLOBAL_QUALITIES->Vs[ix][iz] <= GEN_EXTRACT_VELO_MOD_CALL.MIN_VS) // enforce min Vs
            {
                vsTemp = GEN_EXTRACT_VELO_MOD_CALL.MIN_VS;
                CALCULATION_LOG->nPointsExceedingMinVelo += 1;
            }
            else
            {
                vsTemp = PARTIAL_GLOBAL_QUALITIES->Vs[ix][iz]; // else assign from global structure
            }
            vpTemp = PARTIAL_GLOBAL_QUALITIES->Vp[ix][iz];
            rhoTemp = PARTIAL_GLOBAL_QUALITIES->Rho[ix][iz];
            inbasinTemp = PARTIAL_GLOBAL_QUALITIES->inbasin[ix][iz];
            
            
            if (endianInt == 1) // big endian
            {
                vsWrite = float_swap(vsTemp);
                vpWrite = float_swap(vpTemp);
                rhoWrite = float_swap(rhoTemp);
                inbasinWrite = float_swap(inbasinTemp);
            }
            else if (endianInt == 0) // little endian
            {
                vsWrite = vsTemp;
                vpWrite = vpTemp;
                rhoWrite = rhoTemp;
                inbasinWrite = inbasinTemp;
            }
            
            fwrite(&vpWrite,sizeof(float),1,fvp);
            fwrite(&vsWrite,sizeof(float),1,fvs);
            fwrite(&rhoWrite,sizeof(float),1,frho);
            fwrite(&inbasinWrite,sizeof(float),1,fmask);
            
            if (writeModelTextFile == 1)
            {
                fprintf(fullModTxt,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",PARTIAL_GLOBAL_MESH->Lat[ix],PARTIAL_GLOBAL_MESH->Lon[ix],PARTIAL_GLOBAL_MESH->X[ix],PARTIAL_GLOBAL_MESH->Y, PARTIAL_GLOBAL_MESH->Z[iz]*1.0/1000,vpTemp,vsTemp,rhoTemp);
            }
        }
    }
    
    
    free(vp);
    free(vs);
    free(rho);
    fclose(fvp);
    fclose(fvs);
    fclose(frho);
    fclose(fmask);
    if (writeModelTextFile == 1)
    {
        fclose(fullModTxt);
    }
    
    
}


void writeIndividualProfile(qualities_vector *QUALITIES_VECTOR, gen_profile_call GEN_PROFILE_CALL, mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG)
{
    FILE *fp;
    char fName[MAX_FILENAME_STRING_LEN];
    sprintf(fName,"%s/Profile/Profile.txt",OUTPUT_DIR);
    fp = fopen(fName, "w");
    if (fp == NULL)
    {
        printf("Unable to open text file to write profile to.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"Properties at Lat: %lf Lon: %lf\n",*MESH_VECTOR->Lat, *MESH_VECTOR->Lon);
    fprintf(fp,"Depth (km) \t Vp (km/s) \t Vs (km/s) \t Rho (t/m^3)\n");
    
    
    for(int i = 0; i < MESH_VECTOR->nZ; i++)
    {
        if(QUALITIES_VECTOR->Vs[i] <= GEN_PROFILE_CALL.PROFILE_MIN_VS)
        {
            QUALITIES_VECTOR->Vs[i] = GEN_PROFILE_CALL.PROFILE_MIN_VS;
        }
        fprintf(fp,"%lf \t %lf \t %lf \t %lf\n",MESH_VECTOR->Z[i]/1000, QUALITIES_VECTOR->Vp[i],QUALITIES_VECTOR->Vs[i],QUALITIES_VECTOR->Rho[i]);
    }
    fclose(fp);
    printf("Profile text file write complete.\n");
    
}

void writeMultipleProfiles(qualities_vector *QUALITIES_VECTOR, gen_multi_profiles_call GEN_MULTI_PROFILES_CALL, mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG, multi_profile_parameters *MULTI_PROFILE_PARAMETERS, int profileNum)
{
    if (strcmp(GEN_MULTI_PROFILES_CALL.OUTPUT_TYPE, "1D_SITE_RESPONSE") == 0)
    {
        FILE *fp;
        char fName[MAX_FILENAME_STRING_LEN];
        sprintf(fName,"%s/Profiles/%s.1d",OUTPUT_DIR,&MULTI_PROFILE_PARAMETERS->codes[profileNum]);
        fp = fopen(fName, "w");
        if (fp == NULL)
        {
            printf("Unable to open text file to write profile to.\n");
            exit(EXIT_FAILURE);
        }
        fprintf(fp,"%i",MESH_VECTOR->nZ);
        float deltaDepth;
        
        float depBot = 0;
        for(int i = 0; i < MESH_VECTOR->nZ; i++)
        {
            if(QUALITIES_VECTOR->Vs[i] <= GEN_MULTI_PROFILES_CALL.PROFILE_MIN_VS)
            {
                QUALITIES_VECTOR->Vs[i] = GEN_MULTI_PROFILES_CALL.PROFILE_MIN_VS;
            }
            if( i == (MESH_VECTOR->nZ -1))
            {
                deltaDepth = -999999;
            }
            else if( i == 0)
            {
                deltaDepth = 2 * MESH_VECTOR->Z[i]; // shifts the gridpoint to give layers in written file
                depBot = deltaDepth;
            }
            else
            {
                deltaDepth = 2 * (MESH_VECTOR->Z[i] - depBot) ; // shifts the gridpoint to give layers in written file
                depBot = depBot + deltaDepth;

            }
            fprintf(fp,"\n%1.3lf \t %1.3lf \t %1.3lf \t %1.3lf \t %3.3lf \t %3.3lf",-deltaDepth/1000, QUALITIES_VECTOR->Vp[i],QUALITIES_VECTOR->Vs[i],QUALITIES_VECTOR->Rho[i], 2.0*(41.0 + (34.0 * QUALITIES_VECTOR->Vs[i])),(41.0 + (34.0 * QUALITIES_VECTOR->Vs[i])));
        }
        fclose(fp);
        printf("1D Site response profile text file write complete.\n");
    }
    else if (strcmp(GEN_MULTI_PROFILES_CALL.OUTPUT_TYPE, "STANDARD") == 0)
    {
        FILE *fp;
        char fName[MAX_FILENAME_STRING_LEN];
        sprintf(fName,"%s/Profiles/Profile%s.txt",OUTPUT_DIR,&MULTI_PROFILE_PARAMETERS->codes[profileNum]);
        fp = fopen(fName, "w");
        if (fp == NULL)
        {
            printf("Unable to open text file to write profile to.\n");
            exit(EXIT_FAILURE);
        }
        fprintf(fp,"Properties at Lat: %lf Lon: %lf\n",*MESH_VECTOR->Lat, *MESH_VECTOR->Lon);
        fprintf(fp,"Depth (km) \t Vp (km/s) \t Vs (km/s) \t Rho (t/m^3)\n");
        
        
        for(int i = 0; i < MESH_VECTOR->nZ; i++)
        {
            if(QUALITIES_VECTOR->Vs[i] <= GEN_MULTI_PROFILES_CALL.PROFILE_MIN_VS)
            {
                QUALITIES_VECTOR->Vs[i] = GEN_MULTI_PROFILES_CALL.PROFILE_MIN_VS;
            }
            fprintf(fp,"%lf \t %lf \t %lf \t %lf\n",MESH_VECTOR->Z[i]/1000, QUALITIES_VECTOR->Vp[i],QUALITIES_VECTOR->Vs[i],QUALITIES_VECTOR->Rho[i]);
        }
        fclose(fp);
        printf("Standard profile text file write complete.\n");
    }
    else
    {
        printf("Incorrect OUTPUT_TYPE set, see readme.\n");
        exit(EXIT_FAILURE);
    }
    
}

void writeMultipleProfileSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS, basin_data *BASIN_DATA, partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, in_basin *IN_BASIN,mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG, multi_profile_parameters *MULTI_PROFILE_PARAMETERS, int profileNum)
{
    FILE *fp;
    char fName[MAX_FILENAME_STRING_LEN];
    sprintf(fName,"%s/Profiles/ProfileSurfaceDepths%s.txt", OUTPUT_DIR,&MULTI_PROFILE_PARAMETERS->codes[profileNum]);
    
    fp = fopen(fName, "w");
    if (fp == NULL)
    {
        printf("Unable to open text file to write profile surface depths to.\n");
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp,"Surface Depths (in m) at Lat: %lf Lon: %lf\n\n",*MESH_VECTOR->Lat,*MESH_VECTOR->Lon);
    fprintf(fp,"Global surfaces\n");
    fprintf(fp,"Surface_name \t Depth (m)\n");
    
    for ( int i = 0 ; i < GLOBAL_MODEL_PARAMETERS->nSurf; i++)
    {
        fprintf(fp,"%s\t%lf\n",GLOBAL_MODEL_PARAMETERS->surf[i],PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i]);
    }
    
    fprintf(fp,"\nBasin surfaces (if applicable)\n");
    
    for ( int i = 0 ; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
    {
        if(IN_BASIN->inBasinLatLon[i][0])
        {
            fprintf(fp,"\n%s\n",GLOBAL_MODEL_PARAMETERS->basin[i]);
            for(int j = 0; j < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[i]; j++)
            {
                fprintf(fp,"%s\t%lf\n",GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[i][j],PARTIAL_BASIN_SURFACE_DEPTHS->dep[i][j]);
            }
        }
    }
    
    fclose(fp);
    printf("Completed write of surface depths at the location.\n");
    
}

void writeProfileSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS, basin_data *BASIN_DATA, partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, in_basin *IN_BASIN,mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG)
{
    FILE *fp;
    char fName[MAX_FILENAME_STRING_LEN];
    sprintf(fName,"%s/Profile/ProfileSurfaceDepths.txt", OUTPUT_DIR);
    fp = fopen(fName, "w");
    if (fp == NULL)
    {
        printf("Unable to open text file to write profile surface depths to.\n");
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp,"Surface Depths (in m) at Lat: %lf Lon: %lf\n\n",*MESH_VECTOR->Lat,*MESH_VECTOR->Lon);
    fprintf(fp,"Global surfaces\n");
    fprintf(fp,"Surface_name \t Depth (m)\n");
    
    for ( int i = 0 ; i < GLOBAL_MODEL_PARAMETERS->nSurf; i++)
    {
        fprintf(fp,"%s\t%lf\n",GLOBAL_MODEL_PARAMETERS->surf[i],PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i]);
    }
    
    fprintf(fp,"\nBasin surfaces (if applicable)\n");
    
    for ( int i = 0 ; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
    {
        if(IN_BASIN->inBasinLatLon[i][0])
        {
            fprintf(fp,"\n%s\n",GLOBAL_MODEL_PARAMETERS->basin[i]);
            for(int j = 0; j < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[i]; j++)
            {
                fprintf(fp,"%s\t%lf\n",GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[i][j],PARTIAL_BASIN_SURFACE_DEPTHS->dep[i][j]);
            }
        }
    }
    
    fclose(fp);
    printf("Completed write of surface depths at the location.\n");
    
}

void writeSliceSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS,partial_global_mesh *PARTIAL_GLOBAL_MESH, char *OUTPUT_DIR, slice_surface_depths *SLICE_SURFACE_DEPTHS)
{
    for( int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
    {
        writeAllBasinSurfaceDepths(GLOBAL_MODEL_PARAMETERS, PARTIAL_GLOBAL_MESH, i, OUTPUT_DIR, SLICE_SURFACE_DEPTHS);
    }
    
    writeAllGlobalSurfaceDepths(SLICE_SURFACE_DEPTHS, PARTIAL_GLOBAL_MESH, GLOBAL_MODEL_PARAMETERS, OUTPUT_DIR);
    
}

void writeAllBasinSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_global_mesh *PARTIAL_GLOBAL_MESH, int basinNum, char *OUTPUT_DIR,slice_surface_depths *SLICE_SURFACE_DEPTHS)
{
    char sliceDir[MAX_FILENAME_STRING_LEN];
    sprintf(sliceDir,"%s/Generated_Slices",OUTPUT_DIR);
    
    static int sliceCount = 1;
    
    static int sCountBasin = 0;
    sCountBasin += 1;
    
    
    if (sCountBasin == GLOBAL_MODEL_PARAMETERS->nBasins-1)
    {
        sliceCount += 1;
        sCountBasin = sCountBasin - (GLOBAL_MODEL_PARAMETERS->nBasins);
        
    }
    
    
    //printf("%i %i %i %i\n",sliceCount,basinNum,sCountBasin,GLOBAL_MODEL_PARAMETERS->nBasins);
    
    FILE *fp;
    char fName[MAX_FILENAME_STRING_LEN];
    sprintf(fName,"%s/SliceSurfaceDepthsSlice%iBasin%i.txt",sliceDir,sliceCount,basinNum);
    fp = fopen(fName, "w");
    fprintf(fp, "Lat\tLon\t");
    for(int k = 0; k < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum]; k++)
    {
        fprintf(fp, "%s\t", GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[basinNum][k]);
    }
    fprintf(fp, "\n");
    for (int i = 0; i < PARTIAL_GLOBAL_MESH->nX; i++)
    {
        fprintf(fp, "%lf\t%lf\t", PARTIAL_GLOBAL_MESH->Lat[i],PARTIAL_GLOBAL_MESH->Lon[i]);
        for (int j = 0; j < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum]; j++)
        {
            fprintf(fp, "%lf\t", SLICE_SURFACE_DEPTHS->basinSurfdep[basinNum][j][i]);
        }
        fprintf(fp, "\n");
        
    }
    fclose(fp);
}


void writeAllGlobalSurfaceDepths(slice_surface_depths *SLICE_SURFACE_DEPTHS, partial_global_mesh *PARTIAL_GLOBAL_MESH, global_model_parameters *GLOBAL_MODEL_PARAMETERS,char *OUTPUT_DIR)
{
    char sliceDir[MAX_FILENAME_STRING_LEN];
    sprintf(sliceDir,"%s/Generated_Slices",OUTPUT_DIR);
    
    static int sCountBasin = 0;
    sCountBasin += 1;
    
    FILE *fp;
    char fName[MAX_FILENAME_STRING_LEN];
    sprintf(fName,"%s/SliceSurfaceDepthsGlobal%i.txt",sliceDir,sCountBasin);
    fp = fopen(fName, "w");
    fprintf(fp, "Lat\tLon\t");
    for(int k = 0; k < GLOBAL_MODEL_PARAMETERS->nSurf; k++)
    {
        fprintf(fp, "%s\t", GLOBAL_MODEL_PARAMETERS->surf[k]);
    }
    fprintf(fp, "\n");
    for (int i = 0; i < PARTIAL_GLOBAL_MESH->nX; i++)
    {
        fprintf(fp, "%lf\t%lf\t", PARTIAL_GLOBAL_MESH->Lat[i],PARTIAL_GLOBAL_MESH->Lon[i]);
        for (int j = 0; j < GLOBAL_MODEL_PARAMETERS->nSurf; j++)
        {
            fprintf(fp, "%lf\t",SLICE_SURFACE_DEPTHS->globSurfdep[j][i]);
        }
        fprintf(fp, "\n");
        
    }
    fclose(fp);
    
}

void writeGridpointVelocities(qualities_vector *QUALITIES_VECTOR, gen_extract_multi_gridpoint_vs_call GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL, mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG, int groupingNum)
{
    FILE *fp;
    char fName[MAX_FILENAME_STRING_LEN];
    sprintf(fName,"%s/Velocities_At_Gridpoints/Velocities.txt",OUTPUT_DIR);
    if (groupingNum == 0)
    {
        fp = fopen(fName, "w");
        if (fp == NULL)
        {
            printf("Unable to open text file to write velocities to.\n");
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "Latitude\tLongitude\tDepth(km)\tVp(km/s)\tVs(km/s)\tRho(t/m^3)\n");
        fclose(fp);
    }
    fp = fopen(fName, "a");
    if (fp == NULL)
    {
        printf("Unable to open text file to write velocities to.\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < MESH_VECTOR->nZ; i++)
    {
        if(QUALITIES_VECTOR->Vs[i] <= GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL.MIN_VS)
        {
            QUALITIES_VECTOR->Vs[i] = GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL.MIN_VS;
        }
        //        printf("%lf \t %lf \n",MESH_VECTOR->Lat,MESH_VECTOR->Lon);
        fprintf(fp,"%lf \t %lf \t %lf \t %lf \t %lf \t %lf\n",*MESH_VECTOR->Lat,*MESH_VECTOR->Lon,-MESH_VECTOR->Z[i]/1000, QUALITIES_VECTOR->Vp[i],QUALITIES_VECTOR->Vs[i],QUALITIES_VECTOR->Rho[i]);
    }
    fclose(fp);
}




