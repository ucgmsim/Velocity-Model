To generate multiple profiles text file containing parameters is necessary.

This text file must can be of any name, but must be set within the VM input textfile as the variable "COORDINATES_TEXTFILE=myParametersFile.txt"

It is possible to generate one or more profiles using the following syntax. 

1) The first line of the text file is an integer representing the number of profiles to extract.
2) Each subsequent line represents the lat-lon points of the profile and the Zmax (the maximum depth of the profile +ve downwards in km), Zmin (the top of the profile in km i.e. -1 is 1km ABOVE Mean Sea level) and zSpacing (the spacing between gridpoints to extract at in km) 
i.e. [Profile_Lat] [Profile_Lon] [Zmax] [Zmin] [zSpacing]

refer to ‘MultipleProfileParameters.txt’ for an example


