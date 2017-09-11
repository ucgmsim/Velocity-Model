To generate multiple profiles with variable gridspacing a vector of depths is able to be input. For use with the GENERATE_MULTIPLE_PROFILES call type.

This text file must can be of any name, but must be set within the VM input textfile as the variable "PROFILE_DEPTHS_TEXTFILE=myParametersFile.txt" in conjunction with "SPACING_TYPE=VARIABLE"

Within the text file, the format must be:

1) The first line of the text file is an integer representing the number of depth points to generate velocities at.
2) Each subsequent line represents depth of the gridpoint
i.e. [Depth] 
Depths are +ve downwards in km (ie -0.1 is 100m above mean sea level)

refer to ‘ProfileDepthPoints.txt’ for an example


