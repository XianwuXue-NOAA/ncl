.TH DASHDC 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
DASHDC -
Defines a dash pattern with labels.  If DASHDC is called when
the "quick" version of Dashline is used, an error exit results.
.SH SYNOPSIS
CALL DASHDC (IPAT,JCRT,JSIZE)
.SH C-BINDING SYNOPSIS
#include <ncarg/ncargC.h>
.sp
void c_dashdc (char *ipat, int jcrt, int jsize)
.SH DESCRIPTION 
.IP IPAT 12
(an input constant or variable of type CHARACTER) specifies
the dash pattern to be used.  Although IPAT is of arbitrary
length, 60 characters seems to be a practical limit.
This pattern is repeated for successive
line segments until the full line is drawn.
A dollar sign in IPAT indicates
solid; an apostrophe indicates a gap; blanks are
ignored. Any character in IPAT which is not a dollar
sign, apostrophe, or blank is considered to be part
of a line label. Each line label can be at most 15
characters in length. Sufficient white space is
reserved in the dashed line for writing line labels.
.IP JCRT 12
(an input expression of type INTEGER) specifies that the length to be
assigned to each increment of the line pattern is (JCRT/1023.) NDCs
(Normalized Device Coordinates).  Each increment is either a gap
(represented by a dollar sign in IPAT) or a line segment (represented
by an apostrophe in IPAT).
JCRT must be greater than or equal to 1.
.IP JSIZE 12
(an input expression of type INTEGER) specifies the width of
the plotted characters, as follows:
.RS
.IP " 0" 4
\&.0078 NDCs
.IP " 1" 4
\&.0117 NDCs
.IP " 2" 4
\&.0156 NDCs
.IP " 3" 4
\&.0234 NDCs
.IP ">3" 4
JSIZE/1023. NDCs
.RE
.SH C-BINDING DESCRIPTION
The C-binding argument descriptions are the same as the FORTRAN 
argument descriptions.
.SH EXAMPLES
Use the ncargex command to see the following relevant examples: 
tdashc, tdashp, tdashs, 
fcoord1,
fcoord2,
fdldashc, fdldashd. 
.SH USAGE
DASHDC may be called to define a dash pattern for any of the four
versions of Dashline except the "quick" version; if you call it
when the "quick" version is in use, an error exit will result.
.sp
A dash pattern defined by a call to DASHDC will supersede one defined
by an earlier call to DASHDB or DASHDC.
.SH ACCESS
To use DASHDC or c_dashdc, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order.  
.SH SEE ALSO
Online:
dashline, dashline_params,
curved, dashdb, frstd, lastd, lined, reset, vectd, ncarg_cbind
.sp
Hardcopy:  
NCAR Graphics Contouring and Mapping Tutorial;
NCAR Graphics Fundamentals, UNIX Version;
User's Guide for NCAR GKS-0A Graphics
.SH COPYRIGHT
Copyright (C) 1987-2000
.br
University Corporation for Atmospheric Research
.br

This documentation is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the
License, or (at your option) any later version.

This software is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this software; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA.
