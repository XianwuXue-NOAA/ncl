.TH FL2INT 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
FL2INT -
converts X and Y coordinates from the User Coordinate System to the
Metacode Coordinate System.
.SH STATUS
Metacode units are no longer used in NCAR Graphics;
thus, FL2INT is considered an obsolete routine.
.sp
The current recognized coordinate systems are GKS world coordinates,
GKS normalized device coordinates, NCAR Graphics fractional
coordinates, and NCAR Graphics user coordinates.  See the NCAR Graphics
document "NCAR Graphics Fundamentals, UNIX Version" for a description
of these coordinate systems.
.sp
FL2INT continues to be provided for compatibility of early NCAR Graphics
codes.  If you are writing new code, we suggest that you use user coordinates
and fractional coordinates.  The routines CUFX and CUFY will convert from
user coordinates to fractional coordinates.  CFUX and CFUY will convert
fractional coordinates to user coordinates.
.sp
The following definition of the Metacode Coordinate System is included
for the purpose of interpreting and converting codes:
.sp
The metacode coordinates of a point are integers IMX and IMY between
0 and 32767 inclusive.  The area addressed is a square in a "metacode space"
that is usually mapped into a square subset of the addressable area of
the plotting device.  Metacode coordinates were used in calls to the
routine PLOTIT and are returned in calls to FL2INT.
.SH SYNOPSIS
CALL FL2INT (PX,PY,IX,IY)
.SH DESCRIPTION 
.IP PX 12
(an input expression of type REAL) is the X coordinate of a point in
user coordinates.
.IP PY 12
(an input expression of type REAL) is the Y coordinate of a point in
user coordinates.
.IP IX 12
(an output variable of type INTEGER) is the X coordinate of the point in
metacode coordinates.
.IP IY 12
(an output variable of type INTEGER) is the Y coordinate of the point in
metacode coordinates.
.SH ACCESS
To use FL2INT, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order.
.SH SEE ALSO
Online:
spps, plotit, cmfx, cmfy, cmux, cmuy,
kfmx, kfmy, kmpx, kmpy, kpmx, kpmy, kumx, kumy
.sp
Hardcopy:  
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
