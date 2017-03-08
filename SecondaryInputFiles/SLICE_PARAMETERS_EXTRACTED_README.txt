To extract slices from a saved model a text file containing parameters is necessary.
The model must have been generated previously using the GENERATE_VELOCITY_MOD call.

This text file must can be of any name, but must be set within the VM input textfile as the variable "EXTRACTED_SLICE_PARAMETERS_TEXTFILE=myParametersFile.txt"

It is possible to extract one or more slices using the following syntax. 

1) The first line of the text file is an integer representing the number of slices to extract.
2) Each subsequent line represents the lat-lon points of the slice and the number of points along the transect to sample at. 
i.e. [Lon of point1] [Lon of point2] [Lat of point1] [Lat of point2] [number of sample points]

refer to ‘SliceParametersExtracted.txt’ for an example
