c
c  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c  .                                                             .
c  .                  copyright (c) 1998 by UCAR                 .
c  .                                                             .
c  .       University Corporation for Atmospheric Research       .
c  .                                                             .
c  .                      all rights reserved                    .
c  .                                                             .
c  .                                                             .
c  .                         SPHEREPACK3.0                       .
c  .                                                             .
c  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
c
c
c ... file shses.f
c
c     this file contains code and documentation for subroutines
c     shses and shsesi
c
c ... files which must be loaded with shses.f
c
c     sphcom.f, hrfft.f
c
c     subroutine shses(nlat,nlon,isym,nt,g,idg,jdg,a,b,mdab,ndab,
c    +                 wshses,lshses,work,lwork,ierror)
c
c     subroutine shses performs the spherical harmonic synthesis
c     on the arrays a and b and stores the result in the array g.
c     the synthesis is performed on an equally spaced grid.  the
c     associated legendre functions are stored rather than recomputed
c     as they are in subroutine shsec.  the synthesis is described
c     below at output parameter g.
c
c *** required files from spherepack2
c
c     sphcom.f, hrfft.f
c
c
c     input parameters
c
c     nlat   the number of colatitudes on the full sphere including the
c            poles. for example, nlat = 37 for a five degree grid.
c            nlat determines the grid increment in colatitude as
c            pi/(nlat-1).  if nlat is odd the equator is located at
c            grid point i=(nlat+1)/2. if nlat is even the equator is
c            located half way between points i=nlat/2 and i=nlat/2+1.
c            nlat must be at least 3. note: on the half sphere, the
c            number of grid points in the colatitudinal direction is
c            nlat/2 if nlat is even or (nlat+1)/2 if nlat is odd.
c
c     nlon   the number of distinct londitude points.  nlon determines
c            the grid increment in longitude as 2*pi/nlon. for example
c            nlon = 72 for a five degree grid. nlon must be greater
c            than or equal to 4. the efficiency of the computation is
c            improved when nlon is a product of small prime numbers.
c
c     isym   = 0  no symmetries exist about the equator. the synthesis
c                 is performed on the entire sphere.  i.e. on the
c                 array g(i,j) for i=1,...,nlat and j=1,...,nlon.
c                 (see description of g below)
c
c            = 1  g is antisymmetric about the equator. the synthesis
c                 is performed on the northern hemisphere only.  i.e.
c                 if nlat is odd the synthesis is performed on the
c                 array g(i,j) for i=1,...,(nlat+1)/2 and j=1,...,nlon.
c                 if nlat is even the synthesis is performed on the
c                 array g(i,j) for i=1,...,nlat/2 and j=1,...,nlon.
c
c
c            = 2  g is symmetric about the equator. the synthesis is
c                 performed on the northern hemisphere only.  i.e.
c                 if nlat is odd the synthesis is performed on the
c                 array g(i,j) for i=1,...,(nlat+1)/2 and j=1,...,nlon.
c                 if nlat is even the synthesis is performed on the
c                 array g(i,j) for i=1,...,nlat/2 and j=1,...,nlon.
c
c     nt     the number of syntheses.  in the program that calls shses,
c            the arrays g,a and b can be three dimensional in which
c            case multiple syntheses will be performed.  the third
c            index is the synthesis index which assumes the values
c            k=1,...,nt.  for a single synthesis set nt=1. the
c            discription of the remaining parameters is simplified
c            by assuming that nt=1 or that the arrays g,a and b
c            have only two dimensions.
c
c     idg    the first dimension of the array g as it appears in the
c            program that calls shses.  if isym equals zero then idg
c            must be at least nlat.  if isym is nonzero then idg
c            must be at least nlat/2 if nlat is even or at least
c            (nlat+1)/2 if nlat is odd.
c
c     jdg    the second dimension of the array g as it appears in the
c            program that calls shses.  jdg must be at least nlon.
c
c     a,b    two or three dimensional arrays (see the input parameter
c            nt) that contain the coefficients in the spherical harmonic
c            expansion of g(i,j) given below at the definition of the
c            output parameter g.  a(m,n) and b(m,n) are defined for
c            indices m=1,...,mmax and n=m,...,nlat where mmax is the
c            maximum (plus one) longitudinal wave number given by
c            mmax = min0(nlat,(nlon+2)/2) if nlon is even or
c            mmax = min0(nlat,(nlon+1)/2) if nlon is odd.
c
c     mdab   the first dimension of the arrays a and b as it appears
c            in the program that calls shses. mdab must be at least
c            min0(nlat,(nlon+2)/2) if nlon is even or at least
c            min0(nlat,(nlon+1)/2) if nlon is odd.
c
c     ndab   the second dimension of the arrays a and b as it appears
c            in the program that calls shses. ndab must be at least nlat
c
c     wshses an array which must be initialized by subroutine shsesi.
c            once initialized, wshses can be used repeatedly by shses
c            as long as nlon and nlat remain unchanged.  wshses must
c            not be altered between calls of shses.
c
c     lshses the dimension of the array wshses as it appears in the
c            program that calls shses. define
c
c               l1 = min0(nlat,(nlon+2)/2) if nlon is even or
c               l1 = min0(nlat,(nlon+1)/2) if nlon is odd
c
c            and
c
c               l2 = nlat/2        if nlat is even or
c               l2 = (nlat+1)/2    if nlat is odd
c
c            then lshses must be at least
c
c               (l1*l2*(nlat+nlat-l1+1))/2+nlon+15
c
c     work   a work array that does not have to be saved.
c
c     lwork  the dimension of the array work as it appears in the
c            program that calls shses.  define
c
c               l2 = nlat/2        if nlat is even or
c               l2 = (nlat+1)/2    if nlat is odd
c
c            if isym is zero then lwork must be at least
c
c               (nt+1)*nlat*nlon
c
c            if isym is nonzero lwork must be at least
c
c               (nt+1)*l2*nlon.
c
c     **************************************************************
c
c     output parameters
c
c     g      a two or three dimensional array (see input parameter
c            nt) that contains the spherical harmonic synthesis of
c            the arrays a and b at the colatitude point theta(i) =
c            (i-1)*pi/(nlat-1) and longitude point phi(j) =
c            (j-1)*2*pi/nlon. the index ranges are defined above at
c            at the input parameter isym.  for isym=0, g(i,j) is
c            given by the the equations listed below.  symmetric
c            versions are used when isym is greater than zero.
c
c     the normalized associated legendre functions are given by
c
c     pbar(m,n,theta) = sqrt((2*n+1)*factorial(n-m)/(2*factorial(n+m)))
c                       *sin(theta)**m/(2**n*factorial(n)) times the
c                       (n+m)th derivative of (x**2-1)**n with respect
c                       to x=cos(theta)
c
c     define the maximum (plus one) longitudinal wave number
c     as   mmax = min0(nlat,(nlon+2)/2) if nlon is even or
c          mmax = min0(nlat,(nlon+1)/2) if nlon is odd.
c
c     then g(i,j) = the sum from n=0 to n=nlat-1 of
c
c                   .5*pbar(0,n,theta(i))*a(1,n+1)
c
c              plus the sum from m=1 to m=mmax-1 of
c
c                   the sum from n=m to n=nlat-1 of
c
c              pbar(m,n,theta(i))*(a(m+1,n+1)*cos(m*phi(j))
c                                    -b(m+1,n+1)*sin(m*phi(j)))
c
c
c     ierror = 0  no errors
c            = 1  error in the specification of nlat
c            = 2  error in the specification of nlon
c            = 3  error in the specification of isym
c            = 4  error in the specification of nt
c            = 5  error in the specification of idg
c            = 6  error in the specification of jdg
c            = 7  error in the specification of mdab
c            = 8  error in the specification of ndab
c            = 9  error in the specification of lshses
c            = 10 error in the specification of lwork
c
c
c ****************************************************************
c     subroutine shsesi(nlat,nlon,wshses,lshses,work,lwork,dwork,
c    +                  ldwork,ierror)
c
c     subroutine shsesi initializes the array wshses which can then
c     be used repeatedly by subroutine shses.
c
c     input parameters
c
c     nlat   the number of colatitudes on the full sphere including the
c            poles. for example, nlat = 37 for a five degree grid.
c            nlat determines the grid increment in colatitude as
c            pi/(nlat-1).  if nlat is odd the equator is located at
c            grid point i=(nlat+1)/2. if nlat is even the equator is
c            located half way between points i=nlat/2 and i=nlat/2+1.
c            nlat must be at least 3. note: on the half sphere, the
c            number of grid points in the colatitudinal direction is
c            nlat/2 if nlat is even or (nlat+1)/2 if nlat is odd.
c
c     nlon   the number of distinct londitude points.  nlon determines
c            the grid increment in longitude as 2*pi/nlon. for example
c            nlon = 72 for a five degree grid. nlon must be greater
c            than or equal to 4. the efficiency of the computation is
c            improved when nlon is a product of small prime numbers.
c
c     lshses the dimension of the array wshses as it appears in the
c            program that calls shsesi. define
c
c               l1 = min0(nlat,(nlon+2)/2) if nlon is even or
c               l1 = min0(nlat,(nlon+1)/2) if nlon is odd
c
c            and
c
c               l2 = nlat/2        if nlat is even or
c               l2 = (nlat+1)/2    if nlat is odd
c
c            then lshses must be at least
c
c               (l1*l2*(nlat+nlat-l1+1))/2+nlon+15
c
c     work   a real   work array that does not have to be saved.
c
c     lwork  the dimension of the array work as it appears in
c            the program that calls shsesi.  define
c
c               l1 = min0(nlat,(nlon+2)/2) if nlon is even or
c               l1 = min0(nlat,(nlon+1)/2) if nlon is odd
c
c            and
c
c               l2 = nlat/2        if nlat is even or
c               l2 = (nlat+1)/2    if nlat is odd
c
c            then lwork must be at least
c
c               5*nlat*l2+3*((l1-2)*(nlat+nlat-l1-1))/2
c
c
C*PL*ERROR* Comment line too long
c     dwork  a double precision work array that does not have to be saved.
c
c     ldwork the dimension of the array dwork as it appears in the
c            program that calls shsesi.  ldwork must be at least nlat+1
c
c
c     output parameters
c
c     wshses an array which is initialized for use by subroutine shses.
c            once initialized, wshses can be used repeatedly by shses
c            as long as nlon and nlat remain unchanged.  wshses must
c            not be altered between calls of shses.
c
c     ierror = 0  no errors
c            = 1  error in the specification of nlat
c            = 2  error in the specification of nlon
c            = 3  error in the specification of lshses
c            = 4  error in the specification of lwork
c            = 5  error in the specification of ldwork
c
c ****************************************************************
      SUBROUTINE DSHSES(NLAT,NLON,ISYM,NT,G,IDG,JDG,A,B,MDAB,NDAB,
     +                 WSHSES,LSHSES,WORK,LWORK,IERROR)
      DOUBLE PRECISION G
      DOUBLE PRECISION A
      DOUBLE PRECISION B
      DOUBLE PRECISION WSHSES
      DOUBLE PRECISION WORK
      DIMENSION G(IDG,JDG,1),A(MDAB,NDAB,1),B(MDAB,NDAB,1),WSHSES(1),
     +          WORK(1)

      IERROR = 1
      IF (NLAT.LT.3) RETURN
      IERROR = 2
      IF (NLON.LT.4) RETURN
      IERROR = 3
      IF (ISYM.LT.0 .OR. ISYM.GT.2) RETURN
      IERROR = 4
      IF (NT.LT.0) RETURN
      IERROR = 5
      IF ((ISYM.EQ.0.AND.IDG.LT.NLAT) .OR.
     +    (ISYM.NE.0.AND.IDG.LT. (NLAT+1)/2)) RETURN
      IERROR = 6
      IF (JDG.LT.NLON) RETURN
      IERROR = 7
      MMAX = MIN0(NLAT,NLON/2+1)
      IF (MDAB.LT.MMAX) RETURN
      IERROR = 8
      IF (NDAB.LT.NLAT) RETURN
      IERROR = 9
      IMID = (NLAT+1)/2
      LPIMN = (IMID*MMAX* (NLAT+NLAT-MMAX+1))/2
      IF (LSHSES.LT.LPIMN+NLON+15) RETURN
      IERROR = 10
      LS = NLAT
      IF (ISYM.GT.0) LS = IMID
      NLN = NT*LS*NLON
      IF (LWORK.LT.NLN+LS*NLON) RETURN
      IERROR = 0
      IST = 0
      IF (ISYM.EQ.0) IST = IMID
      CALL DSHSES1(NLAT,ISYM,NT,G,IDG,JDG,A,B,MDAB,NDAB,WSHSES,IMID,LS,
     +            NLON,WORK,WORK(IST+1),WORK(NLN+1),WSHSES(LPIMN+1))
      RETURN
      END
      SUBROUTINE DSHSES1(NLAT,ISYM,NT,G,IDGS,JDGS,A,B,MDAB,NDAB,P,IMID,
     +                  IDG,JDG,GE,GO,WORK,WHRFFT)
      DOUBLE PRECISION G
      DOUBLE PRECISION A
      DOUBLE PRECISION B
      DOUBLE PRECISION P
      DOUBLE PRECISION GE
      DOUBLE PRECISION GO
      DOUBLE PRECISION WORK
      DOUBLE PRECISION WHRFFT
      DIMENSION G(IDGS,JDGS,1),A(MDAB,NDAB,1),B(MDAB,NDAB,1),P(IMID,1),
     +          GE(IDG,JDG,1),GO(IDG,JDG,1),WORK(1),WHRFFT(1)

      LS = IDG
      NLON = JDG
      MMAX = MIN0(NLAT,NLON/2+1)
      MDO = MMAX
      IF (MDO+MDO-1.GT.NLON) MDO = MMAX - 1
      NLP1 = NLAT + 1
      MODL = MOD(NLAT,2)
      IMM1 = IMID
      IF (MODL.NE.0) IMM1 = IMID - 1
      DO 80 K = 1,NT
          DO 80 J = 1,NLON
              DO 80 I = 1,LS
                  GE(I,J,K) = 0.D0
 8000             CONTINUE
  800             CONTINUE
   80 CONTINUE
      IF (ISYM.EQ.1) GO TO 125
      DO 100 K = 1,NT
          DO 100 NP1 = 1,NLAT,2
              DO 100 I = 1,IMID
                  GE(I,1,K) = GE(I,1,K) + A(1,NP1,K)*P(I,NP1)
  100 CONTINUE
      NDO = NLAT
      IF (MOD(NLAT,2).EQ.0) NDO = NLAT - 1
      DO 110 MP1 = 2,MDO
          M = MP1 - 1
          MB = M* (NLAT-1) - (M* (M-1))/2
          DO 110 NP1 = MP1,NDO,2
              MN = MB + NP1
              DO 110 K = 1,NT
                  DO 110 I = 1,IMID
                      GE(I,2*MP1-2,K) = GE(I,2*MP1-2,K) +
     +                                  A(MP1,NP1,K)*P(I,MN)
                      GE(I,2*MP1-1,K) = GE(I,2*MP1-1,K) +
     +                                  B(MP1,NP1,K)*P(I,MN)
  110 CONTINUE
      IF (MDO.EQ.MMAX .OR. MMAX.GT.NDO) GO TO 122
      MB = MDO* (NLAT-1) - (MDO* (MDO-1))/2
      DO 120 NP1 = MMAX,NDO,2
          MN = MB + NP1
          DO 120 K = 1,NT
              DO 120 I = 1,IMID
                  GE(I,2*MMAX-2,K) = GE(I,2*MMAX-2,K) +
     +                               A(MMAX,NP1,K)*P(I,MN)
  120 CONTINUE
  122 IF (ISYM.EQ.2) GO TO 155
  125 DO 140 K = 1,NT
          DO 140 NP1 = 2,NLAT,2
              DO 140 I = 1,IMM1
                  GO(I,1,K) = GO(I,1,K) + A(1,NP1,K)*P(I,NP1)
  140 CONTINUE
      NDO = NLAT
      IF (MOD(NLAT,2).NE.0) NDO = NLAT - 1
      DO 150 MP1 = 2,MDO
          MP2 = MP1 + 1
          M = MP1 - 1
          MB = M* (NLAT-1) - (M* (M-1))/2
          DO 150 NP1 = MP2,NDO,2
              MN = MB + NP1
              DO 150 K = 1,NT
                  DO 150 I = 1,IMM1
                      GO(I,2*MP1-2,K) = GO(I,2*MP1-2,K) +
     +                                  A(MP1,NP1,K)*P(I,MN)
                      GO(I,2*MP1-1,K) = GO(I,2*MP1-1,K) +
     +                                  B(MP1,NP1,K)*P(I,MN)
  150 CONTINUE
      MP2 = MMAX + 1
      IF (MDO.EQ.MMAX .OR. MP2.GT.NDO) GO TO 155
      MB = MDO* (NLAT-1) - (MDO* (MDO-1))/2
      DO 152 NP1 = MP2,NDO,2
          MN = MB + NP1
          DO 152 K = 1,NT
              DO 152 I = 1,IMM1
                  GO(I,2*MMAX-2,K) = GO(I,2*MMAX-2,K) +
     +                               A(MMAX,NP1,K)*P(I,MN)
  152 CONTINUE
  155 DO 160 K = 1,NT
          IF (MOD(NLON,2).NE.0) GO TO 157
          DO 156 I = 1,LS
              GE(I,NLON,K) = 2.D0*GE(I,NLON,K)
  156     CONTINUE
  157     CALL DHRFFTB(LS,NLON,GE(1,1,K),LS,WHRFFT,WORK)
  160 CONTINUE
      IF (ISYM.NE.0) GO TO 180
      DO 170 K = 1,NT
          DO 170 J = 1,NLON
              DO 175 I = 1,IMM1
                  G(I,J,K) = .5D0* (GE(I,J,K)+GO(I,J,K))
                  G(NLP1-I,J,K) = .5D0* (GE(I,J,K)-GO(I,J,K))
  175         CONTINUE
              IF (MODL.EQ.0) GO TO 170
              G(IMID,J,K) = .5D0*GE(IMID,J,K)
  170 CONTINUE
      RETURN
  180 DO 185 K = 1,NT
          DO 185 I = 1,IMID
              DO 185 J = 1,NLON
                  G(I,J,K) = .5D0*GE(I,J,K)
  185 CONTINUE
      RETURN
      END

      SUBROUTINE DSHSESI(NLAT,NLON,WSHSES,LSHSES,WORK,LWORK,DWORK,
     +                  LDWORK,IERROR)
      DOUBLE PRECISION WSHSES
      DOUBLE PRECISION WORK
      DIMENSION WSHSES(*),WORK(*)
C*PT*WARNING* Already double-precision
      DOUBLE PRECISION DWORK(*)

      IERROR = 1
      IF (NLAT.LT.3) RETURN
      IERROR = 2
      IF (NLON.LT.4) RETURN
      IERROR = 3
      MMAX = MIN0(NLAT,NLON/2+1)
      IMID = (NLAT+1)/2
      LPIMN = (IMID*MMAX* (NLAT+NLAT-MMAX+1))/2
      IF (LSHSES.LT.LPIMN+NLON+15) RETURN
      IERROR = 4
      LABC = 3* ((MMAX-2)* (NLAT+NLAT-MMAX-1))/2
      IF (LWORK.LT.5*NLAT*IMID+LABC) RETURN
      IERROR = 5
      IF (LDWORK.LT.NLAT+1) RETURN
      IERROR = 0
      IW1 = 3*NLAT*IMID + 1
      CALL DSES1(NLAT,NLON,IMID,WSHSES,WORK,WORK(IW1),DWORK)
      CALL DHRFFTI(NLON,WSHSES(LPIMN+1))
      RETURN
      END
