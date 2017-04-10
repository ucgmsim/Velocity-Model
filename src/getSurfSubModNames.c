//
//  getGLOBAL_MODEL_PARAMETERS->c
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

global_model_parameters *getGlobalModelParameters(char *modelVersion)

/*
 Purpose: based on the model version number, obtain the names of the surfaces, sub velocity models, basins to be used
 
 Input variables:  
    modelVersion -  double containing the version of the model for generation
 
 Output variables:
    *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 */
{
    global_model_parameters *GLOBAL_MODEL_PARAMETERS;
    GLOBAL_MODEL_PARAMETERS = malloc(sizeof(global_model_parameters));

    // versions <1.0 are for testing only, versions >1.0 are finalized and ready for use. refer to readme document for descriptions of the different model versions
    if (strcmp(modelVersion,"0.01") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";

        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloMod1dFileName[0] = "Cant1D_v1.fd_modfile";

        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;

    }
    else if(strcmp(modelVersion,"0.02") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        
        
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;
        
    }
    else if(strcmp(modelVersion,"0.03") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[2] = "Data/Global_Surfaces/negInf.in";
        
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloMod1dFileName[0] = "Cant1D_v1.fd_modfile";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        
        
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;
        
    }
    
    else if(strcmp(modelVersion,"0.04") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloMod1dFileName[0] = "Cant1D_v1.fd_modfile";
        
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 1;
        
        // CANTERBURY Basin
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 12;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 2;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/QuaternarySurfacesBoundary.txt";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][1] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        //  CHANGE THE BOUNDARYS SO THAT BOUNDARY 0 HS THE LARGER BOUND
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Above_RIC";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "RiccartonTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Quaternary/RiccartonTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "Riccarton";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "BromleyTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Quaternary/BromleyTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "Bromley";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "LinwoodTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Quaternary/LinwoodTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][3] = "Linwood";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][4] = "HeathcoteTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][4] = "Data/Canterbury_Basin/Quaternary/HeathcoteTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][5] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][4] = "Heathcote";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][5] = "BurwoodTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][5] = "Data/Canterbury_Basin/Quaternary/BurwoodTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][5] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][5] = "Burwood";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][6] = "ShirleyTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][6] = "Data/Canterbury_Basin/Quaternary/ShirleyTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][6] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][6] = "Shirley";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][7] = "WainoniTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][7] =  "Data/Canterbury_Basin/Quaternary/WainoniTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][7] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][7] = "Wainoni";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][8] = "PlioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][8] = "Data/Canterbury_Basin/Pre_Quaternary/PlioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][8] = 1;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][8] = "Pliocene";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][9] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][9] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][9] = 1;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][9] = "Miocene";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][10] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][10] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][10] = 1;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][10] = "Paleogene";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][11] = "BasementTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][11] = "Data/Canterbury_Basin/Pre_Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][11] = 1;

        
        
    }
    // Model Version 0.1, 1D velocity sub Model
    else if(strcmp(modelVersion,"FULL_DOMAIN_PLOTTING") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 4;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 3;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "demSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "basementSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[2] = "Data/Canterbury_Basin/Pre_Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->surf[3] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[3] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "GenericSubModA";
        
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "GenericSubModB";
        
        GLOBAL_MODEL_PARAMETERS->veloSubMod[2] = "GenericSubModC";
        
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;
    }

    // Model Version 0.1, 1D velocity sub Model
    else if(strcmp(modelVersion,"Test_0.1") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;

        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "DEM";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[2] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "NaNsubMod";

        
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 1;

        // CANTERBURY Basin
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_Q";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 9;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/QuaternarySurfacesBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "ChristchurchSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "RiccartonTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Quaternary/RiccartonTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "RiccartonSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "BromleyTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Quaternary/BromleyTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "BromleySubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "LinwoodTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Quaternary/LinwoodTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][3] = "LinwoodSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][4] = "HeathcoteTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][4] = "Data/Canterbury_Basin/Quaternary/HeathcoteTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][4] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][4] = "HeathcoteSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][5] = "BurwoodTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][5] = "Data/Canterbury_Basin/Quaternary/BurwoodTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][5] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][5] = "BurwoodSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][6] = "ShirleyTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][6] = "Data/Canterbury_Basin/Quaternary/ShirleyTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][6] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][6] = "ShirleySubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][7] = "WainoniTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][7] =  "Data/Canterbury_Basin/Quaternary/WainoniTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][7] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][7] = "WainoniSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][8] = "PlioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][8] = "Data/Canterbury_Basin/Pre_Quaternary/Pliocene_46_v8p9p18.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][8] = 0;



        
        
    }
    else if(strcmp(modelVersion,"NI_TOMO") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_North_Island";
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;
        
    }
    // Model Version 0.2, EP Tomography
    else if(strcmp(modelVersion,"0.2") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        
//        GLOBAL_MODEL_PARAMETERS->nBasin = 0;
        
    }
    
    //  Model Version 0.3  CHCH BASIN
    else if(strcmp(modelVersion,"0.3") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "demSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "NaNsubMod";
        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 1;
        
    }
    //  Model Version 0.4  w/ CHCH BASIN and 1D velocity model
    else if(strcmp(modelVersion,"0.4") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "demSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "v1DsubMod";
        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 1;
        
    }
    //  Model Version 0.5  w/ CHCH BASIN and EP Tomo
    else if(strcmp(modelVersion,"0.5") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "demSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "v1DsubMod";
        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 1;
        
    }
    
    //  Model Version 0.6  w/ CHCH BASIN of preQ
    else if(strcmp(modelVersion,"0.6") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "basementRockSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "NaNsubMod";
        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "PREQ_CANTERBURY_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 1;
        
    }
    
    
    //  Model Version 0.11  w/ CHCH BASIN of Q
    else if(strcmp(modelVersion,"0.11") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "basementRockSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "NaNsubMod";
        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "Q_CANTERBURY_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 1;
        
    }
    
    //  Model Version 0.12  w/ CHCH BASIN of preQ
    else if(strcmp(modelVersion,"0.12") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "basementRockSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "NaNsubMod";
        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "PREQ_CANTERBURY_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 1;
    }
    
    //  Model Version 0.13  w/ CHCH BASIN
    else if(strcmp(modelVersion,"0.13") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "basementRockSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "NaNsubMod";
        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 1;
        
    }
    //  Model Version 0.141  w/ PREQ + Q + BPV
    else if(strcmp(modelVersion,"0.141") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 4;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 3;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "demSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "basementRockSurf";
        GLOBAL_MODEL_PARAMETERS->surf[3] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[2] = "EPtomo2010subMod";
        
        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "BPV_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 1;
        
    }
    else if(strcmp(modelVersion,"0.14") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 4;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 3;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "demSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "basementRockSurf";
        GLOBAL_MODEL_PARAMETERS->surf[3] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[2] = "EPtomo2010subMod";

        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "BPV_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 1;
        
    }
    //  Model Version 0.15  w/ PREQ + Q + BPV
    else if(strcmp(modelVersion,"0.15") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 4;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 3;
        
        // insert surface surface keywords
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "demSurf";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "basementRockSurf";
        GLOBAL_MODEL_PARAMETERS->surf[3] = "negInfSurf";
        
        // insert velocity submodel names
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[2] = "EPtomo2010subMod";
        
        
        // insert basin name / number
        GLOBAL_MODEL_PARAMETERS->basin[0] = "PREQ_CANTERBURY_BASIN";
        GLOBAL_MODEL_PARAMETERS->basin[1] = "BPV_BASIN";
//        GLOBAL_MODEL_PARAMETERS->nBasin = 2;
        
    }
    else if(strcmp(modelVersion,"1.64_DEM_Pliocene_Enforced") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "DEM";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/DEM/SI_DEM.in";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[2] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 5;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v2_Pliocene_Enforced";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "PlioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/PlioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "PlioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "MioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][3] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][4] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][4] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][4] = 0;
        
        
        // BPV Basin
        GLOBAL_MODEL_PARAMETERS->basin[1] = "BPV_BASIN";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[1] = 2;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[1] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[1][0] = "Data/Boundaries/BPVBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][0] = "BPVTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][0] = "Data/Canterbury_Basin/BPV/BPVTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[1][0] = "BPVSubMod_v3";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][1] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 2;
    }
    //  Model Version 1.1  w/ Tomography
    else if(strcmp(modelVersion,"1.11_DEM") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "DEM";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/DEM/SI_DEM.in";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[2] = "Data/Global_Surfaces/negInf.in";
        
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;
    }

    else if(strcmp(modelVersion,"1D_CUSTOM") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;

        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";


        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloMod1dFileName[0] = "1D_Custom.txt";


        GLOBAL_MODEL_PARAMETERS->nBasins = 0;
    }
    
    //==========================================================
    
    //          FINALIZED VERSIONS
    
    //==========================================================
    
    //  Model Version 1.01  w/ 1D velo mod (Cant. 1D v1)
    else if(strcmp(modelVersion,"1.01") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloMod1dFileName[0] = "Cant1D_v1.fd_modfile";
        
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;
    }
    //  Model Version 1.02  w/ 1D velo mod (Cant. 1D v2)
    else if(strcmp(modelVersion,"1.02") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloMod1dFileName[0] = "Cant1D_v2.fd_modfile";
        
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;
    }
    //  Model Version 1.1  w/ Tomography
    else if(strcmp(modelVersion,"1.11") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;
    }
    //  Model Version 1.2  w/ 1D to basement w/ Tomography
    else if(strcmp(modelVersion,"1.21") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[2] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "v1DsubMod";
        GLOBAL_MODEL_PARAMETERS->veloMod1dFileName[0] = "Cant1D_v2.fd_modfile";
        
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 0;

    }
    else if(strcmp(modelVersion,"1.22") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1 ] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 2;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v2";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 1;

        
        
    }
    //  Model Version 1.3  w/ 1D to basement w/ Tomography w/ Pliocene Basin layer
    else if(strcmp(modelVersion,"1.31") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 3;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "PaleogeneSubMod_v1";

        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;

        
        GLOBAL_MODEL_PARAMETERS->nBasins = 1;
    }
    
    else if(strcmp(modelVersion,"1.32") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 3;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v2";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "Paleogene";
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 1;
    }
    //  Model Version 1.4  w/ 1D to basement w/ Tomography w/ Paleogene + Miocene Basin layers
    else if(strcmp(modelVersion,"1.41") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 4;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "MioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 1;
    }
    //  Model Version 1.5  w/ 1D to basement w/ Tomography w/ Paleogene + Miocene Basin layers w/ BPV Basin
    else if(strcmp(modelVersion,"1.51") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 4;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "MioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        
        
        // BPV Basin
        GLOBAL_MODEL_PARAMETERS->basin[1] = "BPV_BASIN";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[1] = 2;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[1] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[1][0] = "Data/Boundaries/BPVBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][0] = "BPVTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][0] = "Data/Canterbury_Basin/BPV/BPVTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[1][0] = "BPVSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][1] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 2;
    }
    
    //  Model Version 1.6  w/ 1D to basement w/ Tomography w/ Paleogene + Miocene + Pliocene Basin layers (full PreqQ basin)
    else if(strcmp(modelVersion,"1.61") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 5;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "PlioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/PlioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "PlioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "MioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][3] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][4] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][4] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][4] = 0;
        
        
        // BPV Basin
        GLOBAL_MODEL_PARAMETERS->basin[1] = "BPV_BASIN";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[1] = 2;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[1] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[1][0] = "Data/Boundaries/BPVBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][0] = "BPVTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][0] = "Data/Canterbury_Basin/BPV/BPVTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[1][0] = "BPVSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][1] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 2;
    }
    else if(strcmp(modelVersion,"1.62") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 5;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "PlioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/PlioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "PlioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "MioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][3] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][4] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][4] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][4] = 0;
        
        
        // BPV Basin
        GLOBAL_MODEL_PARAMETERS->basin[1] = "BPV_BASIN";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[1] = 2;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[1] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[1][0] = "Data/Boundaries/BPVBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][0] = "BPVTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][0] = "Data/Canterbury_Basin/BPV/BPVTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[1][0] = "BPVSubMod_v2";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][1] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 2;
    }

    else if(strcmp(modelVersion,"1.63") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 5;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "PlioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/PlioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "PlioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "MioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][3] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][4] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][4] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][4] = 0;
        
        
        // BPV Basin
        GLOBAL_MODEL_PARAMETERS->basin[1] = "BPV_BASIN";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[1] = 2;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[1] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[1][0] = "Data/Boundaries/BPVBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][0] = "BPVTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][0] = "Data/Canterbury_Basin/BPV/BPVTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[1][0] = "BPVSubMod_v3";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][1] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 2;
    }
    else if(strcmp(modelVersion,"1.64") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 2;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 1;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 5;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v2";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "PlioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/PlioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "PlioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "MioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][3] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][4] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][4] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][4] = 0;
        
        
        // BPV Basin
        GLOBAL_MODEL_PARAMETERS->basin[1] = "BPV_BASIN";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[1] = 2;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[1] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[1][0] = "Data/Boundaries/BPVBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][0] = "BPVTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][0] = "Data/Canterbury_Basin/BPV/BPVTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[1][0] = "BPVSubMod_v3";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][1] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 2;
    }
    else if(strcmp(modelVersion,"1.65") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "DEM";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/DEM/NZ_DEM.in";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[2] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_NZ";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 5;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v2_Pliocene_Enforced";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "PlioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/PlioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "PlioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "MioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][3] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][4] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][4] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][4] = 0;
        
        
        // BPV Basin
        GLOBAL_MODEL_PARAMETERS->basin[1] = "BPV_BASIN";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[1] = 2;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[1] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[1][0] = "Data/Boundaries/BPVBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][0] = "BPVTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][0] = "Data/Canterbury_Basin/BPV/BPVTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[1][0] = "BPVSubMod_v3";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][1] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 2;
    }
    else if(strcmp(modelVersion,"1.66") == 0)
    {
        // define the number of surfaces and sub models
        GLOBAL_MODEL_PARAMETERS->nSurf = 3;
        GLOBAL_MODEL_PARAMETERS->nVeloSubMod = 2;
        
        // insert surface surface keywords and filenames
        GLOBAL_MODEL_PARAMETERS->surf[0] = "posInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[0] = "Data/Global_Surfaces/posInf.in";
        GLOBAL_MODEL_PARAMETERS->surf[1] = "DEM";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[1] = "Data/DEM/NZ_DEM.in";
        GLOBAL_MODEL_PARAMETERS->surf[2] = "negInfSurf";
        GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[2] = "Data/Global_Surfaces/negInf.in";
        
        // insert velocity submodel keywords and filenames (if necessary)
        GLOBAL_MODEL_PARAMETERS->veloSubMod[0] = "NaNsubMod";
        GLOBAL_MODEL_PARAMETERS->veloSubMod[1] = "EPtomo2010subMod";
        GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_NZ";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Full_South_Island";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Canterbury";
        //GLOBAL_MODEL_PARAMETERS->tomographyName = "2010_Update_Hybrid";
        
        
        // CANTERBURY Basin (1D above basement)
        GLOBAL_MODEL_PARAMETERS->basin[0] = "CANTERBURY_BASIN_1D";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[0] = 5;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[0] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[0][0] = "Data/Boundaries/CanterburyBasinBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][0] = "DEM";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][0] = "Data/DEM/DEM.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][0] = "Cant1D_v2_Pliocene_Enforced";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][1] = "PlioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][1] = "Data/Canterbury_Basin/Pre_Quaternary/Pliocene_46_v8p9p18.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][1] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][1] = "PlioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][2] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][2] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][2] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][2] = "MioceneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][3] = "PaleogeneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][3] = "Data/Canterbury_Basin/Pre_Quaternary/PaleogeneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][3] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[0][3] = "PaleogeneSubMod_v1";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[0][4] = "BasementTopSurf";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[0][4] = "Data/Canterbury_Basin/Quaternary/BasementTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[0][4] = 0;
        
        
        // BPV Basin
        GLOBAL_MODEL_PARAMETERS->basin[1] = "BPV_BASIN";
        GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[1] = 2;
        GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[1] = 1;
        GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[1][0] = "Data/Boundaries/BPVBoundary.txt";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][0] = "BPVTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][0] = "Data/Canterbury_Basin/BPV/BPVTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][0] = 0;
        GLOBAL_MODEL_PARAMETERS->basinSubModelNames[1][0] = "BPVSubMod_v3";
        
        GLOBAL_MODEL_PARAMETERS->basinSurfaceNames[1][1] = "MioceneTop";
        GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[1][1] = "Data/Canterbury_Basin/Pre_Quaternary/MioceneTop.in";
        GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[1][1] = 0;
        
        GLOBAL_MODEL_PARAMETERS->nBasins = 2;
    }
    else
    {
        printf("Invalid velocity model version number.\n");
        exit(EXIT_FAILURE);
    }

    // Assertions
    assert(GLOBAL_MODEL_PARAMETERS->nSurf==(GLOBAL_MODEL_PARAMETERS->nVeloSubMod+1));
    assert(GLOBAL_MODEL_PARAMETERS->nSurf<=(MAX_NUM_GLOBAL_SURFACES));
    assert(GLOBAL_MODEL_PARAMETERS->nBasins<=(MAX_NUM_BASINS));
    for (int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
    {
        assert(GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[i]<=(MAX_NUM_BASIN_SURFACES));
        assert(GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[i] <=(MAX_NUM_BASIN_BOUNDARIES)) ;
    }
    printf("Global Model Parameters Obtained.\n");
    return GLOBAL_MODEL_PARAMETERS;
    
}
