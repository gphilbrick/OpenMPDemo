# OpenMP Demo 

![beforeAfter.png](beforeAfter.png)

by Greg Philbrick (Copyright 2024)
													
## Description	

This is a toy demonstration of OpenMP, applied to the problem of convolving an image. When
the OpenMPDemo executable runs, it exports before and after PGM images representing the application 
of a simple box-blur kernel to an image of a hard-edged circle (the result of this convolution
is a blurred circle).

To see where OpenMP is invoked, look inside convolve() in convolve.h.

## License
					
This code is subject to the Boost Software License v1.0. See LICENSE.txt.