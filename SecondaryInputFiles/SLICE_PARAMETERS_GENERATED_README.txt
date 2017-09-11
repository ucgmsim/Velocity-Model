To generate slices from a scratch, a text file containing parameters is necessary.

This text file must can be of any name, but must be set within the VM input textfile as the variable "GENERATED_SLICE_PARAMETERS_TEXTFILE=myParametersFile.txt"

It is possible to generate one or more slices using the following syntax. 

1) The first line of the text file is an integer representing the number of slices to generate.
2) Each subsequent line represents the lat-lon points of the slice and the number of points in the XY and Z directions to sample at. 
i.e. [Lon of point1] [Lon of point2] [Lat of point1] [Lat of point2] [depth of top of slice (in km +ve downwards)] [depth of bottom of slice (in km +ve downwards)] [number of sample points XY] [number of sample points Z]


refer to ‘SliceParametersGenerated.txt’ for an example
