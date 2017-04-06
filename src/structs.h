//
//  structs.h
//  CVMversions
//
//  Created by Ethan M. Thomson on 20/06/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//

#ifndef structs_h
#define structs_h

// log struct
typedef struct{
    int nPointsExceedingMinVelo;
    int nPointsInGlobalLatSurfaceExtensionZone;
    int nPointsInGlobalLonSurfaceExtensionZone;
    int nPointsInGlobalCornerSurfaceExtensionZone;
    int nPointsInBasinSurfaceExtensionZone;
    int nPointsGlobalSurfacesEnforced;
    double minVs;
    char *outputDirectory;
    char *sliceParametersDirectory;
    double latC1, latC2, latC3, latC4;
    double lonC1, lonC2, lonC3, lonC4;

}calculation_log;

// grid points struct three column vectors
typedef struct{
    double Z[DEP_GRID_DIM_MAX];
    double Lon[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
    double Lat[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
    double maxLat, minLat, maxLon, minLon;
    int nX, nY, nZ;
    double X[LON_GRID_DIM_MAX];
    double Y[LAT_GRID_DIM_MAX];
}global_mesh;

// grid points struct partial - stripped out y (lat)
typedef struct{
    double Z[DEP_GRID_DIM_MAX];
    double Lon[LON_GRID_DIM_MAX];
    double Lat[LON_GRID_DIM_MAX];
    double X[LON_GRID_DIM_MAX];
    double Y;
    int nX, nY, nZ;
}partial_global_mesh;

// individual point v
typedef struct{
    double *Lon;
    double *Lat;
    double Z[DEP_GRID_DIM_MAX];
    int nZ;
//    double referenceDepth;
}mesh_vector;




// global values
typedef struct{
    double Vp[DEP_GRID_DIM_MAX];
    double Vs[DEP_GRID_DIM_MAX];
    double Rho[DEP_GRID_DIM_MAX];
}qualities_vector;

typedef struct{
    double Vp[LON_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
    double Vs[LON_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
    double Rho[LON_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
}partial_global_qualities;

//typedef struct{
//    double Z[DEP_GRID_DIM_MAX];
//    double Lon[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
//    double Lat[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
//    double maxLat, minLat, maxLon, minLon;
//    double X[LON_GRID_DIM_MAX];
//    double Y[LAT_GRID_DIM_MAX];
//    int nX;
//    int nY;
//    int nZ;
//    int currentXind;
//    int saveSurfaceDepths;
//}gridStruct;



// surface read
typedef struct{
    int nLat;
    int nLon;
    double maxLat, minLat, maxLon, minLon;
    double lati[GLOBAL_SURF_IN_DIM_MAX];
    double loni[GLOBAL_SURF_IN_DIM_MAX];
    double raster[GLOBAL_SURF_IN_DIM_MAX][GLOBAL_SURF_IN_DIM_MAX];
}global_surf_read;

typedef struct{
    int nLat;
    int nLon;
    double maxLat, minLat, maxLon, minLon;
    double lati[BASIN_SURF_IN_DIM_MAX];
    double loni[BASIN_SURF_IN_DIM_MAX];
    double raster[BASIN_SURF_IN_DIM_MAX][BASIN_SURF_IN_DIM_MAX];
}basin_surf_read;

//typedef struct{
//    int nLat[NUM_SURF_DIM_MAX][3];
//    int nLon[NUM_SURF_DIM_MAX][3];
//    double lati[NUM_SURF_DIM_MAX][3][SURF_IN_DIM_MAX];
//    double loni[NUM_SURF_DIM_MAX][3][SURF_IN_DIM_MAX];
//    double raster[NUM_SURF_DIM_MAX][3][SURF_IN_DIM_MAX][SURF_IN_DIM_MAX];
//}surfReadGlobal;


// output struct from GenerateSurfaceVectors
//typedef struct{
//    double Vec[SURF_VEC_DIM_MAX][3];
//    int lengthCount;
//}surfVec;

// output struct from calculateSurfaceLatLonIndex
typedef struct{
    int X;
    int Y;
}indexStruct;

//output struct from enforceSurfaceDepths
typedef struct{
    double enforced[2];
}surfDep;

// output struct from determineLocationSurface
//typedef struct{
//    double Vs;
//    double Vp;
//    double Rho;
//}valStructLocal;

//model extent and origin
typedef struct{
    double Ymax;
    double Xmax;
    double Zmax;
    double Zmin;
    double hLatLon;
    double hDep;
    double originLat;
    double originLon;
    double originRot;
    char *version;
}model_extent;




typedef struct{
    double dep[MAX_NUM_GLOBAL_SURFACES];
    int nSurfDep;
}partial_global_surface_depths;

typedef struct{
    double globSurfdep[MAX_NUM_GLOBAL_SURFACES][MAX_NUM_SLICE_GRIDPTS];
    double basinSurfdep[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES][MAX_NUM_SLICE_GRIDPTS];
}slice_surface_depths;


// get surface values
//typedef struct{
//    double dep[NUM_SURF_DIM_MAX][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
//}surfaceDepthsGlobal;

typedef struct{
    int nSurf;
//    double surfDeps[MAX_NUM_GLOBAL_SURFACES];
//    int nLat[MAX_NUM_GLOBAL_SURFACES];
//    int nLon[MAX_NUM_GLOBAL_SURFACES];
//    double maxLat[MAX_NUM_GLOBAL_SURFACES], minLat[MAX_NUM_GLOBAL_SURFACES], maxLon[MAX_NUM_GLOBAL_SURFACES], minLon[MAX_NUM_GLOBAL_SURFACES];
//    double lati[MAX_NUM_GLOBAL_SURFACES][GLOBAL_SURF_IN_DIM_MAX];
//    double loni[MAX_NUM_GLOBAL_SURFACES][GLOBAL_SURF_IN_DIM_MAX];
//    double dep[MAX_NUM_GLOBAL_SURFACES][GLOBAL_SURF_IN_DIM_MAX][GLOBAL_SURF_IN_DIM_MAX];
    global_surf_read *surf[MAX_NUM_GLOBAL_SURFACES];
}global_surfaces;

//typedef struct{
//    int *nLat;
//    int *nLon;
//    double *maxLat, *minLat, *maxLon, *minLon;
//    double *lati[GLOBAL_SURF_IN_DIM_MAX];
//    double *loni[GLOBAL_SURF_IN_DIM_MAX];
//    double *dep[GLOBAL_SURF_IN_DIM_MAX][GLOBAL_SURF_IN_DIM_MAX];
//}surface_pointer;





// 1d velo sub mod read
typedef struct{
    double Vp[SUB_MOD_DIM_MAX];
    double Vs[SUB_MOD_DIM_MAX];
    double Rho[SUB_MOD_DIM_MAX];
    double Dep[SUB_MOD_DIM_MAX];
    int nDep;
}velo_mod_1d_data;

// depth vectors of values between surfaces
//typedef struct{
//    double vec[NUM_SURF_DIM_MAX][DEP_GRID_DIM_MAX];
//    int ind[NUM_SURF_DIM_MAX][DEP_GRID_DIM_MAX];
//    int nDeps[NUM_SURF_DIM_MAX];
//}depthVector;

// lat long housing when performing grid generation
typedef struct{
    double lat;
    double lon;
    double X;
    double Y;
}singleGridPoint;

// surface filenames
typedef struct{
    char *surf[MAX_NUM_GLOBAL_SURFACES];
    int nSurf;
    char *globalSurfFilenames[MAX_NUM_GLOBAL_SURFACES];
    char *veloSubMod[MAX_NUM_GLOBAL_SURFACES];
    int nVeloSubMod;
    char *veloMod1dFileName[MAX_NUM_1D_VELOCITY_MODELS];
    char *tomographyName;
    
    // basin related parameters
    int nBasins;
    char *basin[MAX_NUM_BASINS];
    char *basinSurfaceFilenames[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
    char *basinSurfaceNames[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
    int nBasinSurfaces[MAX_NUM_BASINS];
    int nBasinBoundaries[MAX_NUM_BASINS];
    char *basinBoundaryFilenames[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES];
    char *basinBoundaryNames[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES];
    int basinBoundaryNumber[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
    char *basinSubModelNames[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
}global_model_parameters;

typedef struct{
    velo_mod_1d_data VELO_MOD_1D_DATA;
    
}basin_submodel_data;

// basin data
typedef struct{
//    char *surf[MAX_NUM_BASINS][MAX_NUM_BASIN_SURF];
//    int boundaryType[MAX_NUM_BASINS][MAX_NUM_BASIN_SURF];
//    int nBoundaries[MAX_NUM_BASINS];
//    char *boundaryFileName[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
//    double boundaryLat[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES][MAX_DIM_BOUNDARY_FILE];
//    double boundaryLon[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES][MAX_DIM_BOUNDARY_FILE];
//    double minLon[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
//    double maxLon[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
//    double minLat[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
//    double maxLat[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
//    int boundaryNumPoints[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
//    int nSurf[MAX_NUM_BASINS];
//    char *basinSubMod[MAX_NUM_BASINS][NUM_SURF_DIM_MAX];
//    int nBasinSubMod[MAX_NUM_BASINS];
//    int inBasinLatLon[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
//    int inBasinDep[MAX_NUM_BASINS][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
//    double surfVals[MAX_NUM_BASINS][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][NUM_SURF_DIM_MAX];

//    int nSurf[MAX_NUM_BASINS];
//    double surfDeps[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
//    int nLat[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
//    int nLon[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
//    double maxLat[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
//    double minLat[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
//    double maxLon[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
//    double minLon[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
//    double lati[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES][BASIN_SURF_IN_DIM_MAX];
//    double loni[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES][BASIN_SURF_IN_DIM_MAX];
//    double dep[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES][BASIN_SURF_IN_DIM_MAX][BASIN_SURF_IN_DIM_MAX];
    
    basin_surf_read *surf[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
    
    // Boundaries
    int boundaryNumPoints[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES];
    double boundaryLat[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES][MAX_DIM_BOUNDARY_FILE];
    double boundaryLon[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES][MAX_DIM_BOUNDARY_FILE];
    double minLonBoundary[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES];
    double maxLonBoundary[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES];
    double minLatBoundary[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES];
    double maxLatBoundary[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES];
    
    basin_submodel_data BASIN_SUBMODEL_DATA[MAX_NUM_BASINS];
    
}basin_data;

typedef struct{
    int inBasinLatLon[MAX_NUM_BASINS][MAX_NUM_BASIN_BOUNDARIES];
    int inBasinDep[MAX_NUM_BASINS][DEP_GRID_DIM_MAX];
}in_basin;

typedef struct{
    double dep[MAX_NUM_BASINS][MAX_NUM_BASIN_SURFACES];
}partial_basin_surface_depths;

typedef struct{
    double ORIGIN_LAT;
    double ORIGIN_LON;
    double ORIGIN_ROT;
    double EXTENT_X;
    double EXTENT_Y;
    double EXTENT_ZMAX;
    double EXTENT_ZMIN;
    double EXTENT_Z_SPACING;
    double EXTENT_LATLON_SPACING;
    double MIN_VS;
    char *TOPO_TYPE;
    char *EXTRACTED_SLICE_PARAMETERS_TEXTFILE;
    char *VS_TYPE; // for vs_500 etc.
}gen_extract_velo_mod_call;


typedef struct{
    char *GENERATED_SLICE_PARAMETERS_TEXTFILE;
    double MIN_VS_SLICE;
    char *TOPO_TYPE;
}gen_velo_slices_call;

typedef struct{
    double PROFILE_LAT;
    double PROFILE_LON;
    double PROFILE_ZMAX;
    double PROFILE_ZMIN;
    double PROFILE_MIN_VS;
    double EXTENT_Z_SPACING_PROFILE;
    char *TOPO_TYPE;
}gen_profile_call;


typedef struct{
    char *COORDINATES_TEXTFILE;
    double PROFILE_MIN_VS;
    char *TOPO_TYPE;
    char *SPACING_TYPE;
    double PROFILE_ZMAX;
    double PROFILE_ZMIN;
    double SPACING_PROFILE;
    char *PROFILE_DEPTHS_TEXTFILE;
}gen_multi_profiles_call;

typedef struct{
    int nProfiles;
    double lats[MAX_NUM_GEN_MULTI_PROFILES];
    double lons[MAX_NUM_GEN_MULTI_PROFILES];
}multi_profile_parameters;

typedef struct{
    int nDep;
    double dep[DEP_GRID_DIM_MAX];
}variable_depth_points;

typedef struct {
    char *TOPO_TYPE;
    double MIN_VS;
    char *COORDINATES_TEXT_FILE;
}gen_extract_multi_gridpoint_vs_call;

typedef struct {
    int nPts;
    double lat[MAX_NUM_GRIDPOINTS];
    double lon[MAX_NUM_GRIDPOINTS];
    double dep[MAX_NUM_GRIDPOINTS];
    int grouping[MAX_NUM_GRIDPOINTS];
    int nGroupings;

}multi_gridpoint_parameters;

// struct to house the indices of adjacent points for bilinear interpolation
typedef struct{
    int inSurfaceBounds;
    int latInd[2];
    int lonInd[2];
    
    int inLatExtensionZone;
    int latExtensionType;
    int lonEdgeInd;
    
    int inLonExtensionZone;
    int lonExtensionType;
    int latEdgeInd;
    
    int inCornerZone;
    int cornerLatInd;
    int cornerLonInd;
}adjacent_points;



// struct to hold slice parameters
typedef struct{
    int nSlices;
    double latA[MAX_NUM_SLICES];
    double latB[MAX_NUM_SLICES];
    double lonA[MAX_NUM_SLICES];
    double lonB[MAX_NUM_SLICES];
    double depMax[MAX_NUM_SLICES];
    double depMin[MAX_NUM_SLICES];
    double DepRes[MAX_NUM_SLICES];
    double LatLonRes[MAX_NUM_SLICES];
}slice_parameters;

// lat lon vector housing for slice extraction
typedef struct{
    double latInds[4*MAX_NUM_SLICE_GRIDPTS];
    double lonInds[4*MAX_NUM_SLICE_GRIDPTS];
    double latPts[MAX_NUM_SLICE_GRIDPTS];
    double lonPts[MAX_NUM_SLICE_GRIDPTS];
    double xPts[MAX_NUM_SLICE_GRIDPTS];
    double yPts[MAX_NUM_SLICE_GRIDPTS];
    double Vp[MAX_NUM_SLICE_GRIDPTS][DEP_GRID_DIM_MAX];
    double Vs[MAX_NUM_SLICE_GRIDPTS][DEP_GRID_DIM_MAX];
    double Rho[MAX_NUM_SLICE_GRIDPTS][DEP_GRID_DIM_MAX];
    int  nPts;
}individual_slice_data;

//

//slice extent
typedef struct{
    int sliceExtract;
    int resXY;
    double resZ;
    double latPtsSlice[2];
    double lonPtsSlice[2];
    double zMax;
    double zMin;
    int saveSlices;
}individual_slice_parameters;

typedef struct{
    int requiredLatInds[MAX_NUM_SLICE_GRIDPTS*4*MAX_NUM_SLICES];
    int requiredLonInds[MAX_NUM_SLICE_GRIDPTS*4*MAX_NUM_SLICES];
    int sortedLatInds[MAX_NUM_SLICE_GRIDPTS*4*MAX_NUM_SLICES];
    int sortedLonInds[MAX_NUM_SLICE_GRIDPTS*4*MAX_NUM_SLICES];
    
    
    double Vp[MAX_NUM_SLICE_GRIDPTS*4*MAX_NUM_SLICES][DEP_GRID_DIM_MAX];
    double Vs[MAX_NUM_SLICE_GRIDPTS*4*MAX_NUM_SLICES][DEP_GRID_DIM_MAX];
    double Rho[MAX_NUM_SLICE_GRIDPTS*4*MAX_NUM_SLICES][DEP_GRID_DIM_MAX];

    int nPts;
    int sortedNPts;
    
    individual_slice_parameters *INDIVIDUAL_SLICE_PARAMETERS[MAX_NUM_SLICES];
    individual_slice_data *INDIVIDUAL_SLICE_DATA[MAX_NUM_SLICES];
    
}global_data_for_interpolation;

typedef struct{
    int xyReqFlag[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
    int xyIndFlag[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
}read_flags;


typedef struct{
    int nSurf;
    double surfDeps[MAX_NUM_TOMO_SURFACES];
    global_surf_read *surf[3][MAX_NUM_TOMO_SURFACES]; // 3 for Vp Vs and Rho

    
//    
//    int nLat[MAX_NUM_TOMO_SURFACES];
//    int nLon[MAX_NUM_TOMO_SURFACES];
//    double maxLat[MAX_NUM_TOMO_SURFACES], minLat[MAX_NUM_TOMO_SURFACES], maxLon[MAX_NUM_TOMO_SURFACES], minLon[MAX_NUM_TOMO_SURFACES];
//    double lati[MAX_NUM_TOMO_SURFACES][SURF_IN_DIM_MAX_TOMO];
//    double loni[MAX_NUM_TOMO_SURFACES][SURF_IN_DIM_MAX_TOMO];
//    double Vp[MAX_NUM_TOMO_SURFACES][SURF_IN_DIM_MAX_TOMO][SURF_IN_DIM_MAX_TOMO];
//    double Vs[MAX_NUM_TOMO_SURFACES][SURF_IN_DIM_MAX_TOMO][SURF_IN_DIM_MAX_TOMO];
//    double Rho[MAX_NUM_TOMO_SURFACES][SURF_IN_DIM_MAX_TOMO][SURF_IN_DIM_MAX_TOMO];
}nz_tomography_data;










#endif
