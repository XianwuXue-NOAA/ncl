.TH MAPGTI 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
MAPGTI - 
Gets the current value of a specified Ezmap 
parameter of type INTEGER. 
.sp
MPGETI is an alternate name for the routine MAPGTI.     
.SH SYNOPSIS
CALL MAPGTI (PNAM,IVAL)
.SH C-BINDING SYNOPSIS
#include <ncarg/ncargC.h>
.sp
void c_mapgti (char *pnam, int *ival)
.SH DESCRIPTION 
.IP PNAM 12
(an input expression of type CHARACTER) specifies the name of the
parameter to get. Only the first two characters of the string are
examined. 
.IP IVAL 12
(an output variable of type INTEGER)
receives the value of the parameter
specified by PNAM.
.SH C-BINDING DESCRIPTION
The C-binding argument descriptions are the same as the FORTRAN 
argument descriptions.
.SH USAGE
This routine allows you to retrieve the current value of
Ezmap parameters.  For a complete list of parameters available
in this utility, see the ezmap_params man page.
.SH EXAMPLES
Use the ncargex command to see the following relevant
examples: mpex07, mpex08.
.SH ACCESS
To use MAPGTI or MPGETI, load the NCAR Graphics libraries ncarg, 
ncarg_gks, ncarg_c, and ncarg_loc, preferably in that order.  To use c_mapgti
or c_mpgeti, load the NCAR Graphics libraries ncargC, ncarg_gksC, 
ncarg, ncarg_gks, ncarg_c, and ncarg_loc, preferably in that order.
.SH MESSAGES
See the ezmap man page for a description of all Ezmap error
messages and/or informational messages.
.SH SEE ALSO
Online:
ezmap, 
ezmap_params, 
mapaci,
mapbla,
mapdrw,
mapeod, 
mapfst,
mapgci,
mapgrd,
mapgrm,
mapgtc,  
mapgtl,  
mapgtr,  
mapint,
mapiq,
mapiqa,
mapiqm,
mapit,
mapita,
mapitm,
maplbl,
maplmb,
maplot,
mappos,   
maproj,  
maprs,
maprst,  
mapsav,  
mapset,  
mapstc,  
mapsti,  
mapstl,  
mapstr,  
maptra,
maptri,
maptrn,
mapusr, 
mapvec,
mpgetc,
mpgeti,  
mpgetl,
mpgetr,  
mpsetc,  
mpseti,  
mpsetl,  
mpsetr,  
supmap,
supcon,
ncarg_cbind
.sp
Hardcopy: 
Tutorial: A Step-by-Step Guide to Contouring and Mapping
.SH COPYRIGHT
Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
