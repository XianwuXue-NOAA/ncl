.\"
.\"	$Id: gupd_ws.m,v 1.4 1993-05-03 17:31:48 haley Exp $
.\"
.TH GUPD_WS 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
gupd_ws (Update workstation) - Insures that the workstation is current and
reflects all requests that have been made.
.SH SYNOPSIS
#include <ncarg/gks.h>
.sp
void gupd_ws(Gint ws_id, Gupd_regen_flag upd_regen_flag);
.SH DESCRIPTION
.IP ws_id 12
(Input) A number identifying the workstation to be updated.
ws_id must be the same as that used in some previous gopen_ws call.
.IP upd_regen_flag 12
(Input) A flag to specify if the current picture should be regenerated.  The 
possible values include:
.RS
.IP GUPD_NOT_PEND 12
Postpone
.IP GUPD_PEND
Perform
.RE
.sp
For the workstation types supported in NCAR GKS, this flag should always be 
set to GUPD_PEND.
.SH USAGE
A call to gupd_ws flushes all buffers to bring the workstation up to date.
.SH ACCESS
To use the GKS C-binding routines, load the ncarg_gksC, ncarg_gks, ncarg_c,
and ncarg_loc libraries.
.SH SEE ALSO
Online: 
.BR sflush(3NCARG),
.BR plotif(3NCARG), 
.BR gks(3NCARG),
.BR ncarg_gks_cbind(3NCARG)
.sp
Hardcopy: 
"User's Guide for NCAR GKS-0A Graphics"
.SH COPYRIGHT
Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
