cmake_minimum_required(VERSION 3.25)
project(Velocity_Model C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(NZVM
        src/assignValues.c
        src/basinSubModelList.c
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
        src/ftoa.c
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
        src/perturbationSubMod.c
        src/pointInPoly.c
        src/readWriteInputFiles.c
        src/sliceFunctions.c
        src/structs.h
        src/v1DsubMod.c
        src/writeCVMData.c
        src/writeGridPoints.c
        src/writeLatLonData.c
        src/writeLogFiles.c)

find_package(OpenMP REQUIRED) 
if(OpenMP_C_FOUND) 
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}") 
endif()
target_compile_options(NZVM PRIVATE -Wextra -Wpedantic)
target_link_libraries(NZVM OpenMP::OpenMP_C m) 
