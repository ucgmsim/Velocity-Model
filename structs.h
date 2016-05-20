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
    int nPointsInGlobalSurfaceExtensionZone;
    int nPointsInBasinSurfaceExtensionZone;
}calculationLogStruct;

// grid points struct three column vectors
typedef struct{
    double Z[DEP_GRID_DIM_MAX];
    double Lon[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
    double Lat[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
    double maxLat, minLat, maxLon, minLon;
    double X[LON_GRID_DIM_MAX];
    double Y[LAT_GRID_DIM_MAX];
    int nX;
    int nY;
    int nZ;
    int saveSurfaceDepths;
}gridStruct;

// surface read
typedef struct{
    int nLat;
    int nLon;
    double maxLat, minLat, maxLon, minLon;
    double lati[SURF_IN_DIM_MAX];
    double loni[SURF_IN_DIM_MAX];
    double raster[SURF_IN_DIM_MAX][SURF_IN_DIM_MAX];
}surfRead;

typedef struct{
    int nLat[NUM_SURF_DIM_MAX][3];
    int nLon[NUM_SURF_DIM_MAX][3];
    double lati[NUM_SURF_DIM_MAX][3][SURF_IN_DIM_MAX];
    double loni[NUM_SURF_DIM_MAX][3][SURF_IN_DIM_MAX];
    double raster[NUM_SURF_DIM_MAX][3][SURF_IN_DIM_MAX][SURF_IN_DIM_MAX];
}surfReadGlobal;


// output struct from GenerateSurfaceVectors
typedef struct{
    double Vec[SURF_VEC_DIM_MAX][3];
    int lengthCount;
}surfVec;

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
typedef struct{
    double Vs;
    double Vp;
    double Rho;
}valStructLocal;

//model origin
typedef struct{
    double mlat;
    double mlon;
    double mrot;
}modOrigin;

//model extent
typedef struct{
    double Ymax;
    double Xmax;
    double Zmax;
    double Zmin;
    double hLatLon;
    double hDep;
}modExtent;

//slice extent
typedef struct{
    int sliceExtract;
    int nSections;
    int resXY;
    double resZ;
    double latPtsSlice[MAX_NUM_SLICE_PIVOTS];
    double lonPtsSlice[MAX_NUM_SLICE_PIVOTS];
    double zMax;
    double zMin;
    int saveSlices;
}sliceExtent;

//model version
typedef struct{
    double version;
}modVersion;

// struct to house interpolated Vs Vp and Rho for a given lat lon
typedef struct{
    double deps[NUM_SURF_DIM_MAX];
    double Vs[NUM_SURF_DIM_MAX][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
    double Vp[NUM_SURF_DIM_MAX][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
    double Rho[NUM_SURF_DIM_MAX][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
    int numSurf;
}depInterpVals;

// surface filenames
typedef struct{
    char *surf[NUM_SURF_DIM_MAX];
    int nSurf;
    char *veloSubMod[NUM_SURF_DIM_MAX];
    int nVeloSubMod;
    char *basin[NUM_SURF_DIM_MAX];
    int nBasin;
}surfNames;

// assign values
typedef struct{
    double dep[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
}surfDepValues;

// get surface values
typedef struct{
    double dep[NUM_SURF_DIM_MAX][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
}surfaceDepthsGlobal;

// global values
typedef struct{
    double Vp[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
    double Vs[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
    double Rho[LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
    double minVs;
    int nPtsBelowVsThreshold;

}globalDataValues;

// 1d velo sub mod read
typedef struct{
    double Vp[SUB_MOD_DIM_MAX];
    double Vs[SUB_MOD_DIM_MAX];
    double Rho[SUB_MOD_DIM_MAX];
    double Dep[SUB_MOD_DIM_MAX];
    int nDep;
}velo1D;

// depth vectors of values between surfaces
typedef struct{
    double vec[NUM_SURF_DIM_MAX][DEP_GRID_DIM_MAX];
    int ind[NUM_SURF_DIM_MAX][DEP_GRID_DIM_MAX];
    int nDeps[NUM_SURF_DIM_MAX];
}depthVector;

// lat long housing when performing grid generation
typedef struct{
    double lat;
    double lon;
    double X;
    double Y;
}singleGridPoint;


// basin data
typedef struct{
    char *surf[MAX_NUM_BASINS][NUM_SURF_DIM_MAX];
    int boundaryType[MAX_NUM_BASINS][NUM_SURF_DIM_MAX];
    int nBoundaries[MAX_NUM_BASINS];
    char *boundaryFileName[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
    double boundaryLat[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES][MAX_DIM_BOUNDARY_FILE];
    double boundaryLon[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES][MAX_DIM_BOUNDARY_FILE];
    double minLon[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
    double maxLon[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
    double minLat[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
    double maxLat[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
    int boundaryNumPoints[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES];
    int nSurf[MAX_NUM_BASINS];
    char *basinSubMod[MAX_NUM_BASINS][NUM_SURF_DIM_MAX];
    int nBasinSubMod[MAX_NUM_BASINS];
    int inBasinLatLon[MAX_NUM_BASINS][MAX_NUM_BOUNDARIES][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX];
    int inBasinDep[MAX_NUM_BASINS][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
    double surfVals[MAX_NUM_BASINS][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][NUM_SURF_DIM_MAX];
    double Vp[MAX_NUM_BASINS][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
    double Vs[MAX_NUM_BASINS][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
    double Rho[MAX_NUM_BASINS][LON_GRID_DIM_MAX][LAT_GRID_DIM_MAX][DEP_GRID_DIM_MAX];
}globalBasinData;


// lat lon vector housing for slice extraction
typedef struct{
    double xPts[MAX_NUM_SLICE_GRIDPTS];
    double yPts[MAX_NUM_SLICE_GRIDPTS];
    double latPts[MAX_NUM_SLICE_GRIDPTS];
    double lonPts[MAX_NUM_SLICE_GRIDPTS];
    double Vp[MAX_NUM_SLICE_GRIDPTS][DEP_GRID_DIM_MAX];
    double Vs[MAX_NUM_SLICE_GRIDPTS][DEP_GRID_DIM_MAX];
    double Rho[MAX_NUM_SLICE_GRIDPTS][DEP_GRID_DIM_MAX];
    int  nPts;
}sliceExtractData;

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
}adjacentPointsStruct;

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
}sliceParams;










#endif
