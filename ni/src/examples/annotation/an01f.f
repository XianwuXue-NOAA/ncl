CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                      C
C                            Copyright (C)  1995                       C
C                 University Corporation for Atmospheric Research      C
C                            All Rights Reserved                       C
C                                                                      C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C      File:            an01f.f
C
C      Author:          David Brown (converted to Fortran by Mary Haley)
C                       National Center for Atmospheric Research
C                       PO 3000, Boulder, Colorado
C
C      Date:            Tue Jan 24 10:49:54 MST 1995
C
C     Description:      Illustrates use of Annotation objects.    
C
      external NhlFAppLayerClass
      external NhlFNcgmWorkstationLayerClass
      external NhlFMapPlotLayerClass
      external NhlFtextitemLayerClass
      external nhlfannotationlayerclass
C
C Define enough frames for a fairly smooth animation.
C
      parameter(FRAME_COUNT=36,NDIM=25)

      character*50 name(NDIM)
      real lat(NDIM),lon(NDIM)
      integer num_anno_ids
      data num_anno_ids/NDIM/
      integer anno_ids(NDIM), text_ids(NDIM)
      data anno_ids/25*-1/

      integer ret
      data ret/-1/
      integer appid,wid,mapid,rlist,grlist
      integer i
      integer NCGM
      data name/'Los Angeles','Seattle','Toronto','New York','Miami',
     1 'Mexico City','London','Jakarta','Moscow','New Delhi',
     1 'Rio de Janeiro','Cairo','Buenos Aires','Beijing','Tokyo',
     1 'Lagos','Nairobi','Sydney','Bogota','Lima','Cape Town',
     1 'Calcutta','Shanghai','Bombay','Denver'/

      data lat/34.0,47.6,43.7,40.67,25.75,19.417,51.32,-6.13,55.75,
     1 28.37,-22.883,30.05, -34.67,39.917,35.67,6.45,-1.283,-33.9167,
     1 4.633,-12.1,-33.933,22.583,31.217,18.93,39.716/

      data lon/-118.28,-122.33,-79.4167,-73.83,-80.25,-99.167,-0.1,
     1 106.75,37.7,77.217,-43.283,31.25,-58.4167,116.4167,139.67,3.28,
     1 36.833,151.167,-74.083,-77.05,18.4667,88.35,121.4167,72.85,
     1 -105.017/
C
C Default is to create a metafile.
C
      NCGM=1
C
C Initialize the high level utility library
C
      call NhlFInitialize
C
C Create an application context. Set the app dir to the current
C directory so the application looks for a resource file in the
C working directory. The resource file sets most of the Contour
C resources that remain fixed throughout the life of the Contour
C object.
C
      call NhlFRLCreate(rlist,'SETRL')
      call NhlFRLClear(rlist)
      call NhlFRLSetstring(rlist,'appUsrDir','./',ierr)
      call NhlFRLSetstring(rlist,'appDefaultParent','True',ierr)
      call NhlFCreate(appid,'an01',NhlFAppLayerClass,0,rlist,ierr)
      if( NCGM.eq.1 ) then
C
C Create a meta file workstation
C
         call NhlFRLClear(rlist)
         call NhlFRLSetstring(rlist,'wkMetaName','./an01f.ncgm',ierr)
         call NhlFCreate(wid,'an01Work',NhlFNcgmWorkstationLayerClass,
     1        0,rlist,ierr)
      else
C
C Create an X workstation
C
         call NhlFRLClear(rlist)
         call NhlFRLSetstring(rlist,'wkPause','True',ierr)
         call NhlFCreate(wid,'an01Work',NhlFXWorkstationLayerClass,0,
     1        rlist,ierr)
      endif
C
C Annotation objects are generic object containers that the Overlay
C object knows how to manipulate in a uniform fashion. They may be 
C manipulated in NDC space like the Title or LabelBar objects, or, as
C in this example, aligned with with the plot object's data space.
C
C Create a TextItem for each place name to be included on the map.
C Then create an Annotation object for each TextItem. Register each
C Annotation with the MapPlot object, the creator of the Overlay.
C
      do 10 i = 1,NDIM
         call NhlFRLClear(rlist)
         call NhlFRLSetstring(rlist,'txString',name(i),ierr)
         call NhlFCreate(text_ids(i),name(i),NhlFtextitemLayerClass,wid,
     1    rlist,ierr)
 10   continue
C
C Since the MapPlot object is by default an Overlay plot, you can
C make each TextItem View object into an Annotation simply by setting
C the  ovAnnoViews resource with the array of TextItem ids. 
C
      call NhlFRLClear(rlist)
      call NhlFRLSetintegerarray(rlist,'ovAnnoViews',text_ids,NDIM,ierr)
      call NhlFCreate(mapid,'Map0',NhlFMapPlotLayerClass,wid,rlist,ierr)
C
C Retrieve the ids of the Annotation objects created by the Overlay and
C then set their location in data coordinate space. The Annotation
C objects are arranged in the same order as the TextItems in the
C ovAnnoViews resource.
C
      call NhlFRLCreate(grlist,'GETRL')
      call NhlFRLClear(grlist)
      call NhlFRLGetintegerarray(grlist,'ovAnnotations',anno_ids,
     +                           num_anno_ids,ierr)
      call NhlFGetValues(mapid,grlist,ierr)

      do 20 i=1,num_anno_ids
         call NhlFRLClear(rlist)
         call NhlFRLSetfloat(rlist,'anDataXF',lon(i),ierr)
         call NhlFRLSetfloat(rlist,'anDataYF',lat(i),ierr)
         call NhlFSetValues(anno_ids(i),rlist,ierr)
 20   continue
C
C Create FRAME_COUNT plots, varying the center longitude by an equal
C amount each time.
C
      do 30 i = FRAME_COUNT,1,-1
         call NhlFRLClear(rlist)
         call NhlFRLSetfloat(rlist,'mpCenterLonF',i*360.0/FRAME_COUNT,
     1    ierr)
         call NhlFSetValues(mapid,rlist,ierr)
         call NhlFDraw(mapid,ierr)
         call NhlFFrame(wid,ierr)
 30   continue
C
C Destroy the objects created, close the HLU library and exit.
C
      do 40 i=1,NDIM
         call NhlFDestroy(text_ids(i),ierr)
 40   continue
      call NhlFDestroy(mapid,ierr)
      call NhlFDestroy(wid,ierr)
      call NhlFDestroy(appid,ierr)
      call NhlFClose
      stop
      end