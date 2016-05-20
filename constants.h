//
//  constants.h
//  CVMversions
//
//  Created by Ethan M. Thomson on 20/06/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//

#ifndef constants_h
#define constants_h

#define SURF_IN_DIM_MAX 1022 // maximum dimension of surface input files (used in main and here)
#define LON_GRID_DIM_MAX 1401 // X
#define LAT_GRID_DIM_MAX 1201 // Y
#define DEP_GRID_DIM_MAX 567 // Z
#define NUM_SURF_DIM_MAX 20 // the maximum number of tomography surfaces
#define SURF_VEC_DIM_MAX 8442 // maximum length of surface vector // obsolete
#define SUB_MOD_DIM_MAX 50 // maximum number of entries in the 1d sub model arrays
#define MAX_NUM_BASINS 2 // the maxinum number of basins
#define MAX_DIM_BOUNDARY_FILE 1500 // maximum number of points to define a basin boundary
#define MAX_NUM_VELOSUBMOD 15 // max number of velo sub models in the basin
#define MAX_NUM_SLICE_GRIDPTS 5000 // the maximum number of grid points for slice extraction
#define MAX_NUM_SLICE_PIVOTS 4
#define MAX_NUM_BOUNDARIES 3 // the maximum number of boundaries per velocity model
#define EARTH_RADIUS_MEAN 6371.0071
#define MAX_NUM_SLICES 15 // the maximum number of slices
#define MAX_LAT_SURFACE_EXTENSION 1 // value in degrees the surface files may be extended by
#define MAX_LON_SURFACE_EXTENSION 1 // value in degrees the surface files may be extended by

#define P_a 101.325 // kPa
#define ns_marine 0.331
#define ns_gravel 0.273
#define rho_const 19.0 // t/m3
#define As_gravel 0.312 // m/s
#define As_silt 0.192 // m/s

#define k_pliocene_top 0.669
#define k_pliocene_bot 0.559
#define k_miocene_top 0.624
#define k_miocene_bot 0.485
#define k_paleogene_top 0.447
#define k_paleogene_bot 0.404

#define age_1_pliocene 1.8
#define age_2_pliocene 5.3

#define age_1_miocene 5.3
#define age_2_miocene 24

#define age_1_paleogene 24
#define age_2_paleogene 65






#endif

#if !defined(M_PI)
    #define M_PI 3.14159

#endif

//#if defined(__BIG_ENDIAN__)
//# define __BIG_ENDIAN__ 0
//#endif

//#ifdef __APPLE__
//#include <machine/endian.h>
//#include <libkern/OSByteOrder.h>

//#define htobe16(x) OSSwapHostToBigInt16(x)
//#define htole16(x) OSSwapHostToLittleInt16(x)
//#define be16toh(x) OSSwapBigToHostInt16(x)
//#define le16toh(x) OSSwapLittleToHostInt16(x)
//
//#define htobe32(x) OSSwapHostToBigInt32(x)
//#define htole32(x) OSSwapHostToLittleInt32(x)
//#define be32toh(x) OSSwapBigToHostInt32(x)
//#define le32toh(x) OSSwapLittleToHostInt32(x)
//
//#define htobe64(x) OSSwapHostToBigInt64(x)
//#define htole64(x) OSSwapHostToLittleInt64(x)
//#define be64toh(x) OSSwapBigToHostInt64(x)
//#define le64toh(x) OSSwapLittleToHostInt64(x)
//
//#define __BIG_ENDIAN    BIG_ENDIAN
//#define __LITTLE_ENDIAN LITTLE_ENDIAN
//#define __BYTE_ORDER    BYTE_ORDER
//#else
//#include <endian.h>
//#endif
