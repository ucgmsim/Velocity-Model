TARGET = NZVM
LIBS = -lm
CC = xlc_r
CFLAGS = -O3 -qlanglvl=stdc99 -g -q64  -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -qcpluscmt -D_AIX

#.PHONY: default all clean

HEADERS = $(wildcard *.h)
#SRCS=$(wildcard *.c)
#OBJECTS=$(SRCS:.c=.o)
OBJECTS=BrocherCorrelations.o \
EPtomo2010subMod.o \
NaNsubMod.o \
assignValues.o \
basinSubModels.o \
callRoutines.o \
createOutputDirectories.o \
determineSurfaceDepths.o \
extractPartialMesh.o \
findAdjacentPoints.o \
findSubVeloModelInd.o \
generateModelGrid.o \
generateProfile.o \
getSurfSubModNames.o \
linearInterpolation.o \
loadAllData.o \
loadBasins.o \
loadSurface.o \
main.o \
mathFunctions.o \
pointInPoly.o \
sliceFunctions.o \
v1DsubMod.o \
writeCVMData.o \
writeGridPoints.o \
writeLatLonData.o \
writeLogFiles.o 


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(TARGET): $(OBJECTS)
	$(CC)  $(OBJECTS) $(LIBS) -o $@ $^
	mv NZVM ..

clean:
	-rm -f *.o
	-rm -f $(TARGET)
