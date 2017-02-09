cmake_minimum_required(VERSION 3.6)
project(Velocity_Model)

set(CMAKE_C_STANDARD 99)


set(SOURCE_FILES
        src/assignValues.c
        src/basinSubModels.c
        src/BrocherCorrelations.c
        src/callRoutines.c
        src/constants.h
        src/createOutputDirectories.c
        src/determineSurfaceDepths.c
        src/EPtomo2010subMod.c
        src/extractPartialMesh.c
        src/findAdjacentPoints.c
        src/findSubVeloModelInd.c
        src/functions.h
        src/generateModelGrid.c
        src/generateProfile.c
        src/getSurfSubModNames.c
        src/linearInterpolation.c
        src/loadAllData.c
        src/loadBasins.c
        src/loadSurface.c
        src/main.c
        src/mathFunctions.c
        src/NaNsubMod.c
        src/pointInPoly.c
        src/sliceFunctions.c
        src/structs.h
        src/v1DsubMod.c
        src/writeCVMData.c
        src/writeGridPoints.c
        src/writeLatLonData.c
        src/writeLogFiles.c
        src/readWriteInputFiles.c)

add_executable(Velocity_Model ${SOURCE_FILES})

target_link_libraries(Velocity_Model m)