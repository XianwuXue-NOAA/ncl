.\"
.\"	$Id: guwk.m,v 1.2 1993-04-02 16:51:27 haley Exp $
.\"
.TH GUWK 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
GUWK (Update workstation) - Insures that the workstation is current and
reflects all requests that have been made.
.SH SYNOPSIS
CALL GUWK(WKID,REGFL)
.SH C-BINDING SYNOPSIS
#include <ncarg/gks.h>
.sp
void gupd_ws(Gint ws_id, Gupd_regen_flag upd_regen_flag);
.SH DESCRIPTION
.IP WKID 12
(Integer, Input) - A number identifying the workstation to be updated.
WKID must be the same as that used in some previous GOPWK call.
.IP REGFL 12
(Real, Input) - A flag to specify if the current picture should be
regenerated.  The possible values for this are "0" for postpone, and
"1" for perform.  For the workstation types supported in NCAR GKS,
this flag should always be set to "0".
.SH USAGE
A call to GUWK flushes all GKS buffers to bring the workstation up to date.
GUWK does not flush any NCAR Graphics buffers that sit above GKS such
as the pen-move buffer in SPPS (see the man page for plotif).
.SH ACCESS
To use GKS routines, load the NCAR GKS-0A library 
ncarg_gks.
.SH SEE ALSO
Online: 
sflush, plotif, gupd_ws
.sp
Hardcopy: 
"User's Guide for NCAR GKS-0A Graphics"
.SH COPYRIGHT
Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
