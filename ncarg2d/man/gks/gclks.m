.\"
.\"	$Id: gclks.m,v 1.2 1993-04-02 16:44:01 haley Exp $
.\"
.TH GCLKS 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
GCLKS (Close GSK) - closes the GKS package.
.SH SYNOPSIS
CALL GCLKS
.SH C-BINDING SYNOPSIS
#include <ncarg/gks.h>
.sp
void gclose_gks( void );
.SH USAGE
A call to GCLKS insures that the graphics package is properly
terminated.  No graphics calls depending on GKS should be made
during the time that GKS is closed.  If output to a CGM has been
the exclusive requirement of the graphics job, and the SPPS entry OPNGKS
has been used to open GKS and to open and activate a CGM workstation,
then CLSGKS can be used to deactivate and close the CGM workstation,
and to close GKS, making a direct call to GCLKS unnecessary in that
case.
.SH ACCESS
To use GKS routines, load the NCAR GKS-0A library 
ncarg_gks.
.SH SEE ALSO
Online: 
gopks, gopwk, gacwk, gdawk, gclwk, gupwk, opngks, clsgks, gclose_gks
.sp
Hardcopy: 
"User's Guide for NCAR GKS-0A Graphics"
.SH COPYRIGHT
(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
