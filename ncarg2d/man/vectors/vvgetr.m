.TH VVGETR 3NCARG "April 1993" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
VVGETR - 
Gets the current value of a
Vectors parameter of type REAL.
.SH SYNOPSIS
CALL VVGETR (CNM,RVL) 
.SH C-BINDING SYNOPSIS
#include <ncarg/ncargC.h>
.sp
void c_vvgetr(char *cnm, float *rvl)
.SH DESCRIPTION 
.IP CNM 12
(CHARACTER, input) is the name of a parameter whose real value is to
be retrieved. Only the first three characters of CNM are examined. The
three characters may either be entirely upper or entirely lower case;
mixed case is not recognized. It is recommended that the rest of the
character string be used to improve the readability of the code. For
example, instead of just \'DMX\', use \'DMX - Device Maximum Vector
Size\'.
.IP RVL 12
(REAL, output) is a variable in which the value of the
parameter specified by CNM is to be returned.
.SH C-BINDING DESCRIPTION
The C-binding argument descriptions are the same as the FORTRAN
argument descriptions.
.SH USAGE
This routine allows you to retrieve the current value of Vectors
parameters of type REAL.  For a complete list of parameters available
in this utility, see the vectors_params man page.
.SH EXAMPLES
Use the ncargex command to see the following relevant examples: 
ffex02,
ffex05,
vvex01,
vvex02.
.SH ACCESS
To use VVGETR, load the NCAR Graphics libraries ncarg, ncarg_gks,
ncarg_c, and ncarg_loc, preferably in that order.  To use c_vvgetr,
load the NCAR Graphics libraries ncargC, ncarg_gksC, ncarg, ncarg_gks,
ncarg_c, and ncarg_loc, preferably in that order.
.SH MESSAGES
See the vectors man page for a description of all Vectors error
messages and/or informational messages.
.SH SEE ALSO
Online:
vectors,
vectors_params,
vvectr,
vvgetc,
vvgeti,
vvinit,
vvrset,
vvsetc,
vvseti,
vvsetr,
vvudmv,
vvumxy,
ncarg_cbind.
.SH COPYRIGHT
Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
