To prescribe velocities to gridpoints a text file containing gridpoints is necessary.

This text file must can be of any name, but must be set within the VM input textfile as the variable "COORDINATES_TEXT_FILE=myGridpointsFile.txt"

It is possible to generate one or more profiles using the following syntax. 

1) The first line of the text file is an integer representing the number of gridpoints.
2) Each subsequent line represents the lat-lon-depth points of the profile 
i.e. [Gridpoint_Lat] [Gridpoint_Lon] [Gridpoint_Depth]

Depths are +ve downwards in km (ie -0.1 is 100m above mean sea level)

refer to ‘GridpointCoords.txt’ for an example


