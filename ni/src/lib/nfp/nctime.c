/*********************************************************************
 *   Copyright 2008, University Corporation for Atmospheric Research
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *   $Id: nctime.c,v 1.8 2010/03/18 17:32:07 russ Exp $
 *********************************************************************/

/* 
 * The "nctime.c" file was extracted from the NetCDF-4.1.1 source code
 * and permission was granted for inclusion in NCL.
 *
 * I changed CU_VERBOSE and CU_FATAL to 0 so error messages would get printed, and 
 * then the program will exit.
 *
 * This code was originally extracted with permission from the CDMS
 * time conversion and arithmetic routines developed by Bob Drach,
 * Program for Climate Model Diagnosis and Intercomparison (PCMDI) at
 * Lawrence Livermore National Laboratory as part of the cdtime
 * library.  Russ Rew of the UCAR Unidata Program made changes and
 * additions to support the "-t" option of the netCDF ncdump utility,
 * including a 366-day climate calendar.
 *
 * For the complete time conversion and climate calendar facilities of
 * the CDMS library, get the original sources from LLNL.
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>

#include <netcdf.h>

#include "nctime.h"

int cuErrOpts = 1;			     /* Error options */
int cuErrorOccurred;		     /* True iff cdError was called */

#define CU_FATAL 1			     /* Exit immediately on fatal error */
#define CU_VERBOSE 1			     /* Report errors */

#define CD_DEFAULT_BASEYEAR "1979"	     /* Default base year for relative time (no 'since' clause) */
#define VALCMP(a,b) ((a)<(b)?-1:(b)<(a)?1:0)

/* 
 * "calendar_type" was extracted from the "ncdump.c" code in 
 * NetCDF-4.1.1 and modified for use in NCL.
 *
 * It takes a calendar string and returns the cdtime calendar type.
 */
cdCalenType
calendar_type(const char *calstr) {
    int ctype;
    static struct {
	char* attname;
	int type;
    } calmap[] = {
	{"gregorian", cdMixed},
	{"standard", cdMixed}, /* synonym */
	{"proleptic_gregorian", cdStandard},
	{"noleap", cdNoLeap},
	{"no_leap", cdNoLeap},
	{"365_day", cdNoLeap},	/* synonym */
	{"365", cdNoLeap},	/* synonym */
	{"allleap", cd366},
	{"all_leap", cd366},	/* synonym */
	{"366_day", cd366},	/* synonym */
	{"366", cd366},         /* synonym */
	{"360_day", cd360},
	{"360", cd360},         /* synonym */
	{"julian", cdJulian},
	{"none", cdClim}	/* TODO: test this */
    };
    int ncals = (sizeof calmap)/(sizeof calmap[0]);
    int itype;
    ctype = cdMixed;  /* default mixed Gregorian/Julian ala udunits */
    for(itype = 0; itype < ncals; itype++) {
      if(strcmp(calstr, calmap[itype].attname) == 0) {
	ctype = calmap[itype].type;
	break;
      }
    }
    return ctype;
}

/* Trim trailing whitespace, up to n characters. */
/* If no whitespace up to the last character, set */
/* the last character to null, else set the first */
/* whitespace character to null. */
void
cdTrim(char* s, int n)
{
	char* c;

	if(s==NULL)
		return;
	for(c=s; *c && c<s+n-1 && !isspace(*c); c++);
	*c='\0';
	return;
}

int cuErrorOccurred = 0;
void cdError(char *fmt, ...){
	va_list args;
	
	cuErrorOccurred = 1;
	if(cuErrOpts & CU_VERBOSE){
		va_start(args,fmt);
		fprintf(stderr, "CDMS error: ");
		vfprintf(stderr, fmt, args);
		fprintf(stderr, "\n");
		va_end(args);
	}
	/*
	if(cuErrOpts & CU_FATAL)
		exit(1);
	*/
	return;
}

#define ISLEAP(year,timeType)	((timeType & Cd366) || (((timeType) & CdHasLeap) && (!((year) % 4) && (((timeType) & CdJulianType) || (((year) % 100) || !((year) % 400))))))
static int mon_day_cnt[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
static int days_sum[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

/* Compute month and day from year and day-of-year.
 *
 *	Input:
 *		doy	     (int)  (day-of-year)
 *		date->year   (long)  (year since 0 BC)
 *              date->timeType (CdTimetype) (time type)
 *              date->baseYear   base year for relative times
 *	Output: 
 *		date->month  (short)  (month in year) 
 *		date->day    (short)  (day in month)
 *
 * 
 * Derived from NRL NEONS V3.6.
 */

void
CdMonthDay(int *doy, CdTime *date)
{
	int i;				/* month counter */
	int idoy;			/* day of year counter */
	long year;

	if ((idoy = *doy) < 1) {
		date->month = 0;
		date->day   = 0;
		return;
	}

	if(!(date->timeType & CdChronCal))   /* Ignore year for Clim calendar */
		year = 0;
	else if(!(date->timeType & CdBase1970))	/* year is offset from base for relative time */
		year = date->baseYear + date->year;
	else
		year = date->year;

	if (ISLEAP(year,date->timeType)) {
		mon_day_cnt[1] = 29;
	} else {
		mon_day_cnt[1] = 28;
	}
	date->month	= 0;
	for (i = 0; i < 12; i++) {
		(date->month)++;
		date->day	= idoy;
		if ((idoy -= ((date->timeType & Cd365) ? (mon_day_cnt[date->month-1]) : 30)) <= 0) {
			return;
		}
	}
	return;
}

/* Compute day-of-year from year, month and day
 * 
 *	Input:
 *		date->year  (long)  (year since 0 BC)
 *		date->month (short)  (month in year)
 *		date->day   (short)  (day in month)
 *              date->baseYear   base year for relative times
 *	Output: doy         (int)  (day-of-year)
 * 
 * Derived from NRL NEONS V3.6
 */

void
CdDayOfYear(CdTime *date, int *doy)
{
	int leap_add = 0;		/* add 1 day if leap year */
	int month;			/* month */
	long year;

   	month	= date->month;
	if (month < 1 || month > 12) {
		cdError( "Day-of-year error; month: %d\n", month);
		month = 1;	
	}

	if(!(date->timeType & CdChronCal))   /* Ignore year for Clim calendar */
		year = 0;
	else if(!(date->timeType & CdBase1970))	/* year is offset from base for relative time */
		year = date->baseYear + date->year;
	else
		year = date->year;

	if (ISLEAP(year,date->timeType) && month > 2) leap_add = 1;
	if( ((date->timeType) & Cd365) || ((date->timeType) & Cd366) ) {
	    *doy = days_sum[month-1] + date->day + leap_add ;
	} else {		/* date->timeType & Cd360 */
	    *doy = 30*(month-1) + date->day + leap_add ;
	}
	return;
}

#if 0
/* Convert epochal time (hours since 00 jan 1, 1970)
 *   to human time (structured)
 * 
 * Input: 
 *   etime = epochal time representation
 *   timeType = time type (e.g., CdChron, CdClim, etc.) as defined in cdms.h
 *   baseYear = base real, used for relative time types only
 * 
 * Output: htime = human (structured) time representation
 * 
 * Derived from NRL Neons V3.6
 */
void
Cde2h(double etime, CdTimeType timeType, long baseYear, CdTime *htime)
{
	long 	ytemp;			/* temporary year holder */
	int 	yr_day_cnt;		/* count of days in year */
	int 	doy;			/* day of year */
	int     daysInLeapYear;		     /* number of days in a leap year */
	int     daysInYear;		     /* days in non-leap year */
	extern void CdMonthDay(int *doy, CdTime *date);

	doy	= (long) floor(etime / 24.) + 1;
	htime->hour	= etime - (double) (doy - 1) * 24.;

					     /* Correct for goofy floor func on J90 */
	if(htime->hour >= 24.){
		doy += 1;
		htime->hour -= 24.;
	}

	htime->baseYear = (timeType & CdBase1970) ? 1970 : baseYear;
	if(!(timeType & CdChronCal)) htime->baseYear = 0; /* Set base year to 0 for Clim */
	if(timeType & Cd366) {
	    daysInLeapYear = 366;
	    daysInYear = 366;
	} else {
	    daysInLeapYear = (timeType & Cd365) ? 366 : 360;
	    daysInYear = (timeType & Cd365) ? 365 : 360;
	}

	if (doy > 0) {
		for (ytemp = htime->baseYear; ; ytemp++) {
			yr_day_cnt = ISLEAP(ytemp,timeType) ? daysInLeapYear : daysInYear;
			if (doy <= yr_day_cnt) break;
			doy -= yr_day_cnt;
		}
	} else {
		for (ytemp = htime->baseYear-1; ; ytemp--) {
			yr_day_cnt = ISLEAP(ytemp,timeType) ? daysInLeapYear : daysInYear;
			doy += yr_day_cnt;
			if (doy > 0) break;
		}
	}
        htime->year = (timeType & CdBase1970) ? ytemp : (ytemp - htime->baseYear);
	if(!(timeType & CdChronCal)) htime->year = 0; /* Set year to 0 for Clim */
	htime->timeType = timeType;
	CdMonthDay(&doy,htime);

        return;
}
#endif

void Cde2h(double etime, CdTimeType timeType, long baseYear, CdTime *htime);
void Cdh2e(CdTime *htime, double *etime);

/* Add 'nDel' times 'delTime' to epochal time 'begEtm',
 * return the result in epochal time 'endEtm'.
 */
void
CdAddDelTime(double begEtm, long nDel, CdDeltaTime delTime, CdTimeType timeType,
	     long baseYear, double *endEtm)
{
	double delHours;
	long delMonths, delYears;
	CdTime bhtime, ehtime;

	switch(delTime.units){
	  case CdYear:
		delMonths = 12;
		break;
	  case CdSeason:
		delMonths = 3;
		break;
	  case CdMonth:
		delMonths = 1;
		break;
	  case CdWeek:
		delHours = 168.0;
		break;
	  case CdDay:
		delHours = 24.0;
		break;
	  case CdHour:
		delHours = 1.0;
		break;
	  case CdMinute:
		delHours = 1./60.;
		break;
	  case CdSecond:
		delHours = 1./3600.;
		break;
	  default:
		cdError("Invalid delta time units: %d\n",delTime.units);
		return;
	}

	switch(delTime.units){
	  case CdYear: case CdSeason: case CdMonth:
		Cde2h(begEtm,timeType,baseYear,&bhtime);
		delMonths = delMonths * nDel * delTime.count + bhtime.month - 1;
		delYears = (delMonths >= 0 ? (delMonths/12) : (delMonths+1)/12 - 1);
		ehtime.year = bhtime.year + delYears;
		ehtime.month = delMonths - (12 * delYears) + 1;
		ehtime.day = 1;
		ehtime.hour = 0.0;
		ehtime.timeType = timeType;
		ehtime.baseYear = !(timeType & CdChronCal) ? 0 :
			(timeType & CdBase1970) ? 1970 : baseYear; /* base year is 0 for Clim, */
								   /* 1970 for Chron, */
								   /* or input base year for Rel */
		Cdh2e(&ehtime,endEtm);
		break;
	  case CdWeek: case CdDay: case CdHour: case CdMinute: case CdSecond:
		delHours *= (nDel * delTime.count);
		*endEtm = begEtm + delHours;
		break;
	  default:
		cdError("Invalid delta time units: %d\n",delTime.units);
	}
	return;
}

#if 0
/* Parse relative units, returning the unit and base component time. */
/* Function returns 1 if error, 0 on success */
int
cdParseRelunits(cdCalenType timetype, char* relunits, cdUnitTime* unit, cdCompTime* base_comptime)
{
	char charunits[CD_MAX_RELUNITS];
	char relword[CD_MAX_CHARTIME];
	char basetime_1[CD_MAX_CHARTIME];
	char basetime_2[CD_MAX_CHARTIME];
	char basetime[CD_MAX_CHARTIME];
	int nconv, nconv1, nconv2;
	/* Allow ISO-8601 "T" date-time separator as well as blank separator */
	nconv1 = sscanf(relunits,"%s %s %[^T]T%s",charunits,relword,basetime_1,basetime_2);
	if(nconv1==EOF || nconv1==0){
		cdError("Error on relative units conversion, string = %s\n",relunits);
		return 1;
	}
	nconv2 = sscanf(relunits,"%s %s %s %s",charunits,relword,basetime_1,basetime_2);
	if(nconv2==EOF || nconv2==0){
		cdError("Error on relative units conversion, string = %s\n",relunits);
		return 1;
	}
	if(nconv1 < nconv2) {
	    nconv = nconv2;
	} else {
	  nconv = sscanf(relunits,"%s %s %[^T]T%s",charunits,relword,basetime_1,basetime_2);
	}
/*
 * Mods for NCL: allow for upper or lower case (since/SINCE)
 * and for multiple "relative" words (since,after,ref,from)
 */
	if(nconv==EOF || nconv==0){
		cdError("Error on relative units conversion, string = %s\n",relunits);
		return 1;
	}

	if(nconv >=3 && strcasecmp(relword,"since") && 
	                strcasecmp(relword,"after") &&  
	                strcasecmp(relword,"ref")  && 
                        strcasecmp(relword,"from")) {
	  cdError("Error on relative units conversion, string = %s\n",relunits);
	  return 1;
	}
					     /* Get the units */
	cdTrim(charunits,CD_MAX_RELUNITS);
/*
 * Mods for NCL: allow for upper or lower case units (hours/HOURS).
 * Simply changed "strncmp" and "strcmp" to "strncasecmp" and "strcasecmp".
 */
	if(!strncasecmp(charunits,"sec",3) || !strcasecmp(charunits,"s")){
		*unit = cdSecond;
	}
	else if(!strncasecmp(charunits,"min",3) || !strcasecmp(charunits,"mn")){
		*unit = cdMinute;
	}
	else if(!strncasecmp(charunits,"hour",4) || !strcasecmp(charunits,"hr")){
		*unit = cdHour;
	}
	else if(!strncasecmp(charunits,"day",3) || !strcasecmp(charunits,"dy")){
		*unit = cdDay;
	}
	else if(!strncasecmp(charunits,"week",4) || !strcasecmp(charunits,"wk")){
		*unit = cdWeek;
	}
	else if(!strncasecmp(charunits,"month",5) || !strcasecmp(charunits,"mo")){
		*unit = cdMonth;
	}
	else if(!strncasecmp(charunits,"season",6)){
		*unit = cdSeason;
	}
	else if(!strncasecmp(charunits,"year",4) || !strcasecmp(charunits,"yr")){
		if(!(timetype & cdStandardCal)){
			cdError("Error on relative units conversion: climatological units cannot be 'years'.\n");
			return 1;
		}
		*unit = cdYear;
	}
	else {
		cdError("Error on relative units conversion: invalid units = %s\n",charunits);
		return 1;
	}

					     /* Build the basetime, if any (default is 1979), */
					     /* or month 1 for climatological time. */
	if(nconv == 1){
		if(timetype & cdStandardCal)
			strcpy(basetime,CD_DEFAULT_BASEYEAR);
		else
			strcpy(basetime,"1");
	}
					     /* Convert the basetime to component, then epochal (hours since 1970) */
	else{
		if(nconv == 2){
			cdTrim(basetime_1,CD_MAX_CHARTIME);
			strcpy(basetime,basetime_1);
		}
		else{
			cdTrim(basetime_1,CD_MAX_CHARTIME);
			cdTrim(basetime_2,CD_MAX_CHARTIME);
			sprintf(basetime,"%s %s",basetime_1,basetime_2);
		}
	}

	cdChar2Comp(timetype, basetime, base_comptime);

	return 0;
}
#endif

/* ca - cb in Gregorian calendar */
/* Result is in hours. */
double
cdDiffGregorian(cdCompTime ca, cdCompTime cb){

	double rela, relb;

	cdComp2Rel(cdStandard, ca, "hours", &rela);
	cdComp2Rel(cdStandard, cb, "hours", &relb);
	return (rela - relb);
}

/* Return -1, 0, 1 as ca is less than, equal to, */
/* or greater than cb, respectively. */
int cdCompCompare(cdCompTime ca, cdCompTime cb){

	int test;

	if ((test = VALCMP(ca.year, cb.year)))
		return test;
	else if ((test = VALCMP(ca.month, cb.month)))
		return test;
	else if ((test = VALCMP(ca.day, cb.day)))
		return test;
	else
		return (VALCMP(ca.hour, cb.hour));
}

/* ca - cb in Julian calendar.  Result is in hours. */
static double
cdDiffJulian(cdCompTime ca, cdCompTime cb){

	double rela, relb;

	cdComp2Rel(cdJulian, ca, "hours", &rela);
	cdComp2Rel(cdJulian, cb, "hours", &relb);
	return (rela - relb);
}

/* ca - cb in mixed Julian/Gregorian calendar. */
/* Result is in hours. */
double
cdDiffMixed(cdCompTime ca, cdCompTime cb){

	static cdCompTime ZA = {1582, 10, 5, 0.0};
	static cdCompTime ZB = {1582, 10, 15, 0.0};
	double result;

	if (cdCompCompare(cb, ZB) == -1){
		if (cdCompCompare(ca, ZB) == -1) {
			result = cdDiffJulian(ca, cb);
		}
		else {
			result = cdDiffGregorian(ca, ZB) + cdDiffJulian(ZA, cb);
		}
	}
	else {
		if (cdCompCompare(ca, ZB) == -1){
			result = cdDiffJulian(ca, ZA) + cdDiffGregorian(ZB, cb);
		}
		else {
			result = cdDiffGregorian(ca, cb);
		}
	}
	return result;
}

/* Divide ('endEtm' - 'begEtm') by 'delTime',
 * return the integer portion of the result in 'nDel'.
 */
void
CdDivDelTime(double begEtm, double endEtm, CdDeltaTime delTime, CdTimeType timeType,
	     long baseYear, long *nDel)
{
	double delHours, frange;
	long delMonths, range;
	CdTime bhtime, ehtime;
	int hoursInYear;

	switch(delTime.units){
	  case CdYear:
		delMonths = 12;
		break;
	  case CdSeason:
		delMonths = 3;
		break;
	  case CdMonth:
		delMonths = 1;
		break;
	  case CdWeek:
		delHours = 168.0;
		break;
	  case CdDay:
		delHours = 24.0;
		break;
	  case CdHour:
		delHours = 1.0;
		break;
	  case CdMinute:
		delHours = 1./60.;
		break;
	  case CdSecond:
		delHours = 1./3600.;
		break;
	  default:
		cdError("Invalid delta time units: %d\n",delTime.units);
		return;
	}

	switch(delTime.units){
	  case CdYear: case CdSeason: case CdMonth:
		delMonths *= delTime.count;
		Cde2h(begEtm,timeType,baseYear,&bhtime);
		Cde2h(endEtm,timeType,baseYear,&ehtime);
		if(timeType & CdChronCal){   /* Chron and Rel time */
			range = 12*(ehtime.year - bhtime.year)
				+ (ehtime.month - bhtime.month);
		}
		else{			     /* Clim time, ignore year */
			range = (ehtime.month - bhtime.month);
			if(range < 0) range += 12;
		}
		*nDel = abs(range)/delMonths;
		break;
	  case CdWeek: case CdDay: case CdHour: case CdMinute: case CdSecond:
		delHours *= (double)delTime.count;
		if(timeType & CdChronCal){   /* Chron and Rel time */
			frange = fabs(endEtm - begEtm);
		}
		else{			     /* Clim time, ignore year, but */
					     /* wraparound relative to hours-in-year*/
			frange = endEtm - begEtm;
			if(timeType & Cd366) {
			    hoursInYear = 8784;
			} else {
			    hoursInYear = (timeType & Cd365) ? 8760. : 8640.;
			}
					     /* Normalize frange to interval [0,hoursInYear) */
			if(frange < 0.0 || frange >= hoursInYear)
				frange -= hoursInYear * floor(frange/hoursInYear);
		}
		*nDel = (frange + 1.e-10*delHours)/delHours;
		break;
	  default:
		cdError("Invalid delta time units: %d\n",delTime.units);
	}
	return;
}

/* Value is in hours. Translate to units. */
double
cdFromHours(double value, cdUnitTime unit){
	double result;

	switch(unit){
	case cdSecond:
		result = value * 3600.0;
		break;
	case cdMinute:
		result = value * 60.0;
		break;
	case cdHour:
		result = value;
		break;
	case cdDay:
		result = value/24.0;
		break;
	case cdWeek:
		result = value/168.0;
		break;
	case cdMonth:
	case cdSeason:
	case cdYear:
	case cdFraction:
	default:
    		cdError("Error on conversion from hours to vague unit");
		result = 0;
		break;
	}
	return result;
}
					     /* Map to old timetypes */
int
cdToOldTimetype(cdCalenType newtype, CdTimeType* oldtype)
{
	switch(newtype){
	  case cdStandard:
		*oldtype = CdChron;
		break;
	  case cdJulian:
		*oldtype = CdJulianCal;
		break;
	  case cdNoLeap:
		*oldtype = CdChronNoLeap;
		break;
	  case cd360:
		*oldtype = CdChron360;
		break;
	  case cd366:
		*oldtype = CdChron366;
		break;
	  case cdClim:
		*oldtype = CdClim;
		break;
	  case cdClimLeap:
		*oldtype = CdClimLeap;
		break;
	  case cdClim360:
		*oldtype = CdClim360;
		break;
	  default:
		cdError("Error on relative units conversion, invalid timetype = %d",newtype);
		return 1;
	}
	return 0;
}

#if 0
/* Convert human time to epochal time (hours since 00 jan 1, 1970)
 * 
 * Input: htime = human time representation
 * 
 * Output: etime = epochal time representation
 * 
 * Derived from NRL Neons V3.6
 */
void
Cdh2e(CdTime *htime, double *etime)
{
	long 	ytemp, year;			/* temporary year holder */
	int	day_cnt;		/* count of days */
	int 	doy;			/* day of year */
	long    baseYear;		     /* base year for epochal time */
	int     daysInLeapYear;		     /* number of days in a leap year */
	int     daysInYear;		     /* days in non-leap year */
	extern void CdDayOfYear(CdTime *date, int *doy);

	CdDayOfYear(htime,&doy);
	
	day_cnt	= 0;

	baseYear = ((htime->timeType) & CdBase1970) ? 1970 : htime->baseYear;
	year = ((htime->timeType) & CdBase1970) ? htime->year : (htime->year + htime->baseYear);
	if(!((htime->timeType) & CdChronCal)) baseYear = year = 0;	/* set year and baseYear to 0 for Clim */
	if((htime->timeType) & Cd366) {
	    daysInLeapYear = 366;
	    daysInYear = 366;
	} else {
	    daysInLeapYear = ((htime->timeType) & Cd365) ? 366 : 360;
	    daysInYear = ((htime->timeType) & Cd365) ? 365 : 360;
	}
	
	if (year > baseYear) {
		for (ytemp = year - 1; ytemp >= baseYear; ytemp--) {
			day_cnt += ISLEAP(ytemp,htime->timeType) ? daysInLeapYear : daysInYear;
		}
	} else if (year < baseYear) {
		for (ytemp = year; ytemp < baseYear; ytemp++) {
			day_cnt -= ISLEAP(ytemp,htime->timeType) ? daysInLeapYear : daysInYear;
		}
	}	
	*etime	= (double) (day_cnt + doy - 1) * 24. + htime->hour;
        return;
}
#endif

/* Validate the component time, return 0 if valid, 1 if not */
int
cdValidateTime(cdCalenType timetype, cdCompTime comptime)
{
	if(comptime.month<1 || comptime.month>12){
		cdError("Error on time conversion: invalid month = %hd\n",comptime.month);
		return 1;
	}
	if(comptime.day<1 || comptime.day>31){
		cdError("Error on time conversion: invalid day = %hd\n",comptime.day);
		return 1;
	}
	if(comptime.hour<0.0 || comptime.hour>24.0){
		cdError("Error on time conversion: invalid hour = %lf\n",comptime.hour);
		return 1;
	}
	return 0;
}

#if 0
void
cdChar2Comp(cdCalenType timetype, char* chartime, cdCompTime* comptime)
{
	double sec;
	int ihr, imin, nconv;
	long year;
	short day;
	short month;

	comptime->year = CD_NULL_YEAR;
	comptime->month = CD_NULL_MONTH;
	comptime->day = CD_NULL_DAY;
	comptime->hour = CD_NULL_HOUR;
	
	if(timetype & cdStandardCal){
		nconv = sscanf(chartime,"%ld-%hd-%hd %d:%d:%lf",&year,&month,&day,&ihr,&imin,&sec);
		if(nconv==EOF || nconv==0){
			cdError("Error on character time conversion, string = %s\n",chartime);
			return;
		}
		if(nconv >= 1){
			comptime->year = year;
		}
		if(nconv >= 2){
			comptime->month = month;
		}
		if(nconv >= 3){
			comptime->day = day;
		}
		if(nconv >= 4){
			if(ihr<0 || ihr>23){
				cdError("Error on character time conversion: invalid hour = %d\n",ihr);
				return;
			}
			comptime->hour = (double)ihr;
		}
		if(nconv >= 5){
			if(imin<0 || imin>59){
				cdError("Error on character time conversion: invalid minute = %d\n",imin);
				return;
			}
			comptime->hour += (double)imin/60.;
		}
		if(nconv >= 6){
			if(sec<0.0 || sec>60.0){
				cdError("Error on character time conversion: invalid second = %lf\n",sec);
				return;
			}
			comptime->hour += sec/3600.;
		}
	}
	else{				     /* Climatological */
		nconv = sscanf(chartime,"%hd-%hd %d:%d:%lf",&month,&day,&ihr,&imin,&sec);
		if(nconv==EOF || nconv==0){
			cdError("Error on character time conversion, string = %s",chartime);
			return;
		}
		if(nconv >= 1){
			comptime->month = month;
		}
		if(nconv >= 2){
			comptime->day = day;
		}
		if(nconv >= 3){
			if(ihr<0 || ihr>23){
				cdError("Error on character time conversion: invalid hour = %d\n",ihr);
				return;
			}
			comptime->hour = (double)ihr;
		}
		if(nconv >= 4){
			if(imin<0 || imin>59){
				cdError("Error on character time conversion: invalid minute = %d\n",imin);
				return;
			}
			comptime->hour += (double)imin/60.;
		}
		if(nconv >= 5){
			if(sec<0.0 || sec>60.0){
				cdError("Error on character time conversion: invalid second = %lf\n",sec);
				return;
			}
			comptime->hour += sec/3600.;
		}
	}
	(void)cdValidateTime(timetype,*comptime);
	return;
}
#endif

/* Convert ct to relunits (unit, basetime) */
/* in the mixed Julian/Gregorian calendar. */
/* unit is anything but year, season, month. unit and basetime are */
/* from the parsed relunits. Return result in reltime. */
void
cdComp2RelMixed(cdCompTime ct, cdUnitTime unit, cdCompTime basetime, double *reltime){

	double hourdiff;

	hourdiff = cdDiffMixed(ct, basetime);
	*reltime = cdFromHours(hourdiff, unit);
	return;
}

void
cdComp2Rel(cdCalenType timetype, cdCompTime comptime, char* relunits, double* reltime)
{
	cdCompTime base_comptime;
	CdDeltaTime deltime;
	CdTime humantime;
	CdTimeType old_timetype;
	cdUnitTime unit;
	double base_etm, etm, delta=0.;
	long ndel, hoursInYear;
	
					     /* Parse the relunits */
	if(cdParseRelunits(timetype, relunits, &unit, &base_comptime))
		return;

					     /* Handle mixed Julian/Gregorian calendar */
	if (timetype == cdMixed){
		switch(unit){
		case cdWeek: case cdDay: case cdHour: case cdMinute: case cdSecond:
			cdComp2RelMixed(comptime, unit, base_comptime, reltime);
			return;
		case cdYear: case cdSeason: case cdMonth:
			timetype = cdStandard;
			break;
		case cdFraction:
		case cdBadUnit:
		        cdError("invalid unit in conversion");
		        break;
		}
	}
	
					     /* Convert basetime to epochal */
	humantime.year = base_comptime.year;
	humantime.month = base_comptime.month;
	humantime.day = base_comptime.day;
	humantime.hour = base_comptime.hour;
	humantime.baseYear = 1970;
					     /* Map to old-style timetype */
	if(cdToOldTimetype(timetype,&old_timetype))
		return;
	humantime.timeType = old_timetype;
	Cdh2e(&humantime,&base_etm);

					     /* Map end time to epochal */
	humantime.year = comptime.year;
	humantime.month = comptime.month;
	humantime.day = comptime.day;
	humantime.hour = comptime.hour;
	Cdh2e(&humantime,&etm);
					     /* Calculate relative time value for months or hours */
	deltime.count = 1;
	deltime.units = (CdTimeUnit)unit;
	switch(unit){
	  case cdWeek: case cdDay: case cdHour: case cdMinute: case cdSecond:
		delta = etm - base_etm;
		if(!(timetype & cdStandardCal)){	/* Climatological time */
			hoursInYear = (timetype & cd365Days) ? 8760. : (timetype & cdHasLeap) ? 8784. : 8640.;
					     /* Normalize delta to interval [0,hoursInYear) */
			if(delta < 0.0 || delta >= hoursInYear)
				delta -= hoursInYear * floor(delta/hoursInYear);
		}
		break;
	  case cdYear: case cdSeason: case cdMonth:
		CdDivDelTime(base_etm, etm, deltime, old_timetype, 1970, &ndel);
		break;
	  case cdFraction:
	  case cdBadUnit:
	        cdError("invalid unit in conversion");
		break;
	}

					     /* Convert to output units */
	switch(unit){
	  case cdSecond:
		*reltime = 3600.0 * delta;
		break;
	  case cdMinute:
		*reltime = 60.0 * delta;
		break;
	  case cdHour:
		*reltime = delta;
		break;
	  case cdDay:
		*reltime = delta/24.0;
		break;
	  case cdWeek:
		*reltime = delta/168.0;
		break;
	  case cdMonth: case cdSeason: case cdYear: /* Already in correct units */
		if(timetype & cdStandardCal)
			*reltime = (base_etm <= etm) ? (double)ndel : (double)(-ndel);
		else			     /* Climatological time is already normalized*/
			*reltime = (double)ndel;
		break;
	  default:
		cdError("invalid unit in conversion");
		break;
	}

	return;
}

/* Add (value,unit) to comptime. */
/* value is in hours. */
/* calendar is anything but cdMixed. */
void
cdCompAdd(cdCompTime comptime, double value, cdCalenType calendar, cdCompTime *result){

	double reltime;

	cdComp2Rel(calendar, comptime, "hours", &reltime);
	reltime += value;
	cdRel2Comp(calendar, "hours", reltime, result);
	return;
}

/* Add value in hours to ct, in the mixed Julian/Gregorian
 * calendar. */
static void
cdCompAddMixed(cdCompTime ct, double value, cdCompTime *result){

	static cdCompTime ZA = {1582, 10, 5, 0.0};
	static cdCompTime ZB = {1582, 10, 15, 0.0};
	double xj, xg;

	if (cdCompCompare(ct, ZB) == -1){
		xj = cdDiffJulian(ZA, ct);
		if (value <= xj){
			cdCompAdd(ct, value, cdJulian, result);
		}
		else {
			cdCompAdd(ZB, value-xj, cdStandard, result);
		}
	}
	else {
		xg = cdDiffGregorian(ZB, ct);
		if (value > xg){
			cdCompAdd(ct, value, cdStandard, result);
		}
		else {
			cdCompAdd(ZA, value-xg, cdJulian, result);
		}
	}
	return;
}

/* Return value expressed in hours. */
static double
cdToHours(double value, cdUnitTime unit){

	double result = 0;

	switch(unit){
	case cdSecond:
		result = value/3600.0;
		break;
	case cdMinute:
		result = value/60.0;
		break;
	case cdHour:
		result = value;
		break;
	case cdDay:
		result = 24.0 * value;
		break;
	case cdWeek:
		result = 168.0 * value;
		break;
	default:
	        cdError("invalid unit in conversion");
		break;

	}
	return result;
}

/* Convert relative time (reltime, unit, basetime) to comptime in the
 * mixed Julian/Gregorian calendar. unit is anything but year, season,
 * month. unit and basetime are from the parsed relunits. Return
 * result in comptime. */
void
cdRel2CompMixed(double reltime, cdUnitTime unit, cdCompTime basetime, cdCompTime *comptime){

	reltime = cdToHours(reltime, unit);
	cdCompAddMixed(basetime, reltime, comptime);
	return;
}


void
cdRel2Comp(cdCalenType timetype, char* relunits, double reltime, cdCompTime* comptime)
{
	CdDeltaTime deltime;
	CdTime humantime;
	CdTimeType old_timetype;
	cdCompTime base_comptime;
	cdUnitTime unit, baseunits;
	double base_etm, result_etm;
	double delta=0.;
	long idelta=0;

					     /* Parse the relunits */
	if(cdParseRelunits(timetype, relunits, &unit, &base_comptime))
		return;

	if (timetype == cdMixed){
		switch(unit){
		case cdWeek: case cdDay: case cdHour: case cdMinute: case cdSecond:
			cdRel2CompMixed(reltime, unit, base_comptime, comptime);
			return;
		case cdYear: case cdSeason: case cdMonth:
			timetype = cdStandard;
			break;
		case cdFraction:
		case cdBadUnit:
		        cdError("invalid unit in conversion");
		        break;
		}
	}

	baseunits =cdBadUnit;
	switch(unit){
	  case cdSecond:
		delta = reltime/3600.0;
		baseunits = cdHour;
		break;
	  case cdMinute:
		delta = reltime/60.0;
		baseunits = cdHour;
		break;
	  case cdHour:
		delta = reltime;
		baseunits = cdHour;
		break;
	  case cdDay:
		delta = 24.0 * reltime;
		baseunits = cdHour;
		break;
	  case cdWeek:
		delta = 168.0 * reltime;
		baseunits = cdHour;
		break;
	  case cdMonth:
		idelta = (long)(reltime + (reltime<0 ? -1.e-10 : 1.e-10));
		baseunits = cdMonth;
		break;
	  case cdSeason:
		idelta = (long)(3.0 * reltime + (reltime<0 ? -1.e-10 : 1.e-10));
		baseunits = cdMonth;
		break;
	  case cdYear:
		idelta = (long)(12 * reltime + (reltime<0 ? -1.e-10 : 1.e-10));
		baseunits = cdMonth;
		break;
	  default:
	        cdError("invalid unit in conversion");
		break;
	}

	deltime.count = 1;
	deltime.units = (CdTimeUnit)baseunits;

	humantime.year = base_comptime.year;
	humantime.month = base_comptime.month;
	humantime.day = base_comptime.day;
	humantime.hour = base_comptime.hour;
	humantime.baseYear = 1970;
					     /* Map to old-style timetype */
	if(cdToOldTimetype(timetype,&old_timetype))
		return;
	humantime.timeType = old_timetype;

	Cdh2e(&humantime,&base_etm);
					     /* If months, seasons, or years, */
	if(baseunits == cdMonth){

					     /* Calculate new epochal time from integer months. */
					     /* Convert back to human, then comptime. */
					     /* For zero reltime, just return the basetime*/
		if(reltime != 0.0){
			CdAddDelTime(base_etm,idelta,deltime,old_timetype,1970,&result_etm);
			Cde2h(result_etm, old_timetype, 1970, &humantime);
		}
	}
					     /* Calculate new epochal time. */
					     /* Convert back to human, then comptime. */
	else{
		Cde2h(base_etm+delta, old_timetype, 1970, &humantime);
		
	}
	comptime->year = humantime.year;
	comptime->month = humantime.month;
	comptime->day = humantime.day;
	comptime->hour = humantime.hour;

	return;
}

/* rkr: output as ISO 8601 strings */
/*
 * This routine was modified from cdComp2Iso (for NCL usage)
 * to return minutes and seconds (comptime already contains
 * year, month, day, hour), rather than formatted strings.
 */
void
cdComp2Iso_minsec(cdCalenType timetype, cdCompTime *comptime, int *imin, 
		  double *sec)
{
	double dtmp;
	int ihr;

	if(cdValidateTime(timetype,*comptime))
		return;
	
	ihr = (int)comptime->hour;
	dtmp = 60.0 * (comptime->hour - (double)ihr);
	*imin = (int)dtmp;
	*sec = 60.0 * (dtmp - (double)(*imin));
}

/* rkr: added for output closer to ISO 8601 */
/*
 * This routine was modified from cdRel2Iso (for NCL usage)
 * to return "comptime" in addition to minutes and seconds
 * (comptime already contains year, month, day, hour),
 * rather than formatted strings.
 */
void
cdRel2Iso_minsec(cdCalenType timetype, char* relunits, double reltime,
		 cdCompTime *comptime, int *min, double *sec)
{
	cdRel2Comp(timetype, relunits, reltime, comptime);
	cdComp2Iso_minsec(timetype, comptime, min, sec);

	return;
}
