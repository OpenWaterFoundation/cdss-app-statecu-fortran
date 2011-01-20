C***************************************************************************
C
C   Include File    : gcommon.inc
C   Author          : HB Manguerra
C   Date            : December 1994
C   Purpose         : This includes declarations of all global variables
C                     used by crdss-cu.
C   Calling program : All subroutines except acount.f, budget.f,
C                     dayhrs.f, evap.f, growth.f, interkc.f, irgate.f,
C   Assumptions     : 
C   Limitations     :
C emw 2006/12     Version 12.1  - 
C emw 2006/07     Version 12.0  - 
c emw 2004/12     Version 11.3  - allows missing data to be prorated
c rrb 2003/05/12; Version 10.4 2003/05/12
c rrb 2003/06/20; Allow multiple subirrigation crops (grass)
c rrb 2003/06/20; Add multiple subirrigation crops parameter (dim_sub)
c rrb 2003/06/20; Add multiple subirrigation crop ID (subname)
c rrb 2004/01/26; Version 10.5 2004/01/26
c                 Revise Wsupsum to correct output problems related to 
c		  fraction of Gw and sprinkler land and Soil moisture
c		  treatment when acres change
c ew 03/12/2004; Version 10.7 03/12/2005
c ew 03/12/2004; Consider drain/tailwater supply
c ew 08/04     ; Version 10.8
c ew 12/04     ; Version 11.0 - new format for input files, ASCE modified method,
c                altitude adjustment capabilites
c jb 07/07     ; Version 12.14
c jb 08/07     ; Version 12.15
c                more /0 tests for acreage inputs
c                include recharge (areach(), gmode 3) in unused water category output
c                change WARNING message criteria for acreage discrepancy (CDS vs IPY)
c                  to 2%.
c                change DIM_NY (year dimension) to 130 from 100
c jb 08/07     ; Version 12.16
c                change WARNING message criteria for acreage discrepancy (CDS vs IPY)
c                  to 2% AND > 1.0 .
c                change WARNING message criteria for crop acreage % discrepancy
c                  to 2%
c                change WARNING message format for acreage discrepancy - single record vs. wrapped @ 80 characters
C jb 09/07     ; Version 12.17
c                enhance DWB, SWB report column headers when ISUPLY=4 and IFLOOD>0
c                note: handles up to IFLOOD=4 - four subirrigated crops, crops > 4 will not have column headers
C jb 11/07     ; Version 12.18
c                expanded allowable IDRAIN parameter values - added new value of 2 which allows negative drain/tailwater values to be used
c                if IDRAIN=1, drain file is read and negative values are changed to 0
c                if IDRAIN=2, drain file is read and negative values are left alone
c                  the latter is used in SPDSS for carrier ditches to cancel river diversions after losses - part of the gw model interaction requirements
C jb 11/07     ; Version 12.19
c                modified cds reading code - read and use acreage values (not % values) when possible
c                negative deliveries can not cause negative CU
c                systemwide table at bottom of WSL and IWR reports now allows areas > 999,999 acres
c                converts max pumping rate in ipy (AF/month) from being based on 30.4 days/month to actual days/month
C jb 02/08     ; Version 12.20
c                modified/updated winter precip soil moisture algorithm and all related routines
c                add deficit irrigation (gw pumping only) capability - new record in CCU file
C jb 02/08     ; Version 12.21
c                added subbasin totals to the binary output file (bd1)
C jb 09/08     ; Version 12.30
c                adjusted subbasin totals for binary output file (bd1) - new sub basin (district) ID format "DIST01", etc.
c                adjusted negative drain file value logic
c                added fill on fly options
c                adjusted ... fractions vs percentages
c jhb 09/08      version 13.0! major new release
c                major redesign of log file
c                added several new data filling options (diversion and climate data) using new imiss values
c                adjusted shortage based filling logic to be more robust
c                tweaked typout values to designate whether to put district and basin totals in BD1 file
c jhb 12/08      increased error log array (DIM_NLWS) size from 500 to 800
c                
c                
C***************************************************************************
C   Notes           : Current Dimensions of the program.
C                     number of subareas (or structures)    = 800 
C                     number of parcels per structure       = 50
C                     number of years                       = 130 
C                     number of crop types                  = 70
C                     number of weather stations            = 300
C                     number of county hucs                 = 200
C                     number of sub basins (districts)      = 100
C***************************************************************************
C                     number of log file warnings               = 100
C                     max number of log file warning structures = 800
C                        (per warning)
C***************************************************************************

C-----Dimension (Corresponding dimension parameters in other include files
C-----should be changed)
      INTEGER  DIM_NA, DIM_NP, DIM_NY, DIM_NC, DIM_NW, DIM_CH
      INTEGER  DIM_SUB, DIM_NSB, DIM_NLW, DIM_NLWS
      CHARACTER*16 RDATE
      REAL VERS
c grb new version number 
      PARAMETER (VERS = 13.00)
      PARAMETER (RDATE = '10/01/08 11:11AM')
c dim_na used to be 1999
      PARAMETER (DIM_NA = 800)
      PARAMETER (DIM_NP = 50)
      PARAMETER (DIM_NY = 130)
      PARAMETER (DIM_NC = 70)
      PARAMETER (DIM_NW = 300)
      PARAMETER (DIM_CH = 200)
c _____________________________________________________
c rrb 2003/06/20; Allow multiple subirrigation crops
      parameter (dim_sub = 5)
c _____________________________________________________
c rrb 2003/06/20; Allow multiple subirrigation crops
      integer subirr
c _____________________________________________________
c rrb 2003/06/20; Allow multiple subirrigation crops
      character*5  subname(dim_sub)
c _____________________________________________________
c grb 05-20-00 add variable to determine years of missing diversion data or aggregates
      INTEGER missflag(dim_na,dim_ny),missflg1(dim_na)
c _____________________________________________________
c jhb 03-00-08 add variables to determine years of missing iwr and diversion data (if not filled)
      INTEGER missiwr(dim_na,dim_ny),missdiv(dim_na,dim_ny)
c _____________________________________________________
c grb 05-20-00 added switch in crop characteristics file for modified vs. original blaney (kt=1)	   
      integer ktsw(dim_nc)
c _____________________________________________________
C jhb 02/20/08 added sub totals - sub basins (sb)
C jhb          for now make it the first two chars of structure ID (district),
C jhb          but it could be any subgrouping
      PARAMETER (DIM_NSB = 100)
c _____________________________________________________
      PARAMETER (DIM_NLW = 100)
      PARAMETER (DIM_NLWS = 800)
c _____________________________________________________

C-----Global Variable Declaration
      LOGICAL DAYONE
      LOGICAL WARNINGS
      
      CHARACTER*1 QUOTE
      CHARACTER*3 AMN(12)
Cjhb=&==================================================================
Cjhb  replaced with named constants to use in select case construct
Cjhb=&==================================================================
C     CHARACTER*9 FILEID(33)
Cjhb=&==================================================================
      CHARACTER*120 TITLE(6)
      CHARACTER (LEN = 8) CURDATE
      CHARACTER (LEN = 10) CURTIME
      CHARACTER*80 STNAME(DIM_NW),PPNAME(DIM_NW)
      CHARACTER*87 DLINE, SLINE
      CHARACTER*130 DLLINE,SLLINE
      CHARACTER*40 BAS_ID(DIM_NA)
      CHARACTER*30 CPNAME(DIM_NC)
      CHARACTER*10 OCOUNT(DIM_CH),TTCOUNT(DIM_CH),NU_NME(DIM_NC+1)
      CHARACTER*10 NU_SUM(DIM_NY,DIM_NC+1),SUM_Y(DIM_NY)
      CHARACTER*15 SOIL(DIM_NC), SNAME(14)
      CHARACTER*24 NAMEASP(DIM_NA)
      CHARACTER*200 dfile
c rb- add input filenames
      character*200 rcufile,ccufile,kbcfile,tmpfile,pptfile
      character*200 ddhfile,othfile,logfile,cdsfile,fdfile,cchfile
      character*200 kpmfile,strfile,ddsfile,kmhfile
      character*200 ddrfile,penfile,tmxfile,tmnfile
      character*200 pdyfile,solfile,vapfile,wndfile,dlyfile
      character*200 ipyfile,pvhfile,dlafile,clifile
c grb 06-29-00 add admfile and dddfile
c jhb 07-12-00 add gisfile (known, but unused in statecu)
      character*200 admfile,dddfile,addfile,dcufile,gisfile
      character*12 wsid(DIM_NW),ttid(DIM_NW),S4CATID
      integer dbyear,deyear,ddcsw,iwheat
      integer ifir,ddstrctt,ddindex,iddrec
      character*12 ddstruct(DIM_NA)
      integer imissflg
      REAL TRIGHTS(DIM_NA)
      REAL def_irr, def_irr_frac(DIM_NA,DIM_NY)
c grb 06-29-00 add variables for admin calculation
      real persen(dim_na,dim_ny,12),peroth(dim_na,dim_ny,12)
      INTEGER ipresim,iprtysm,iagg,imiss2
      LOGICAL :: FILLONFLY
      INTEGER :: divonfly, climonfly
      real endsoils(dim_na),endsoilj(dim_na),endsoilo(dim_na)
c jhb log file variables
      integer :: scu_debug
      character*150 :: lgwrmsg1(0:dim_nlw)
      character*150 :: lgwrmsg2(0:dim_nlw)
      character*150 :: lgwrmsg3(0:dim_nlw)
      integer ::  lgwrcnt(0:dim_nlw),lgwrscnt(0:dim_nlw)
      character*50 ::  lgwrsid(0:dim_nlw,0:dim_nlws)
      integer ::  lgwrswcnt(0:dim_nlw,0:dim_nlws)
c rb- add fn_len so that it can be used in all subroutines
      integer fn_len,fmode
c rb- added added year dimension to nparce
      INTEGER NBASIN,CKEY(DIM_NC),BKEY(DIM_NA,DIM_NP,DIM_NY)
      INTEGER NPARCE(DIM_NA,dim_ny),NHUC
      INTEGER MIDDLE(12), IQUOTE, OHUC(DIM_CH),TTHUC(DIM_CH)
      Integer idaily, iclim
      INTEGER FLAG1, FLAG2, FLAG3, ISUPLY, NU_DIM,GWFLAG
      INTEGER NYRS, NYR1, NYR2, RN_XCO, RN_REF
      INTEGER GNYRS, GNYR1, GNYR2
      INTEGER ONYRS, ONYR1, ONYR2
      INTEGER N_STA, N_CRPS, N_CRP(DIM_NC)
      INTEGER N_SOIL(DIM_NC),CRPTYP(DIM_NC)
      INTEGER JBEG(DIM_NP,DIM_NY), JEND(DIM_NP,DIM_NY)
      INTEGER TFLG1(DIM_NC),TFLG2(DIM_NC)
      INTEGER GDATE1(DIM_NC),GDATE2(DIM_NC),GDATE3(DIM_NC)
      INTEGER GDATE4(DIM_NC),GDATES(DIM_NC)
      INTEGER MONTH(12), GDATE5(DIM_NC),CUT2(DIM_NC),CUT3(DIM_NC)
      INTEGER TYPOUT(DIM_NA)
      INTEGER INCH, S_OUT, JEXIST(DIM_NY)

      REAL AWCR(DIM_NA),DIVSUP(DIM_NA,DIM_NY,14),SENASP(DIM_NA)
      REAL*8 ADMINENT
      REAL JUNASP(DIM_NA)
      REAL WLAT(DIM_NW),WELEV(DIM_NW)
      REAL TRAIN(DIM_NY,366)
      REAL wws(DIM_NA,DIM_NW), WRS(DIM_NA,DIM_NW)
Cjhb=&==================================================================
      REAL ota(DIM_NA,DIM_NW), opa(DIM_NA,DIM_NW)
Cjhb=&==================================================================
      REAL owws(DIM_NA,DIM_NW), OWRS(DIM_NA,DIM_NW)
      REAL ttwws(DIM_NW), TTWRS(DIM_NW),psenmo,pjunmo,pothmo
      REAL T_AREA(DIM_NA,DIM_NY),M_AREA(DIM_NA,DIM_NY,12)
      REAL MAD(DIM_NC), C_AREA(DIM_NY,DIM_NC) 
      REAL tmpr(DIM_NW,DIM_NY,13),WBU(DIM_NA,DIM_NY,14)
      REAL WINTPREC(DIM_NA,DIM_NY,14),WBUSED(DIM_NA,DIM_NY,14)
      REAL RPARA(3),RETN(DIM_NA,60),tmpt(DIM_NW,DIM_NY,13)
      REAL TMOIS1(DIM_NC),TMOIS2(DIM_NC),AWC(DIM_NC), RZ(DIM_NC)
      REAL IRZ(DIM_NC),FRZ(DIM_NC),APD(DIM_NC),SCAPATOT(DIM_NA,dim_ny)
      REAL EFF(DIM_NA),APER(DIM_NA),DPER(DIM_NA),GWP(DIM_NA,DIM_NY,12)
      REAL BLAT(DIM_NA),BELEV(DIM_NA),ETTOT(DIM_NA,DIM_NY,14)
      REAL TMEAN(DIM_NY,13),RNTOT(DIM_NY,13),EFFPPT(DIM_NA,DIM_NY,14)
      REAL T28(DIM_NA,DIM_NY,2),T32(DIM_NA,DIM_NY,2),PERDP(DIM_NA)
      REAL AREA(DIM_NA,DIM_NP,DIM_NY), PJAREA(DIM_NY) 

c ew dimensioned arrays to go with new common block G12 to carry variables
c    that used to be written to tmp files
      REAL REQT(DIM_NA,DIM_NY,14),DEPLET(DIM_NA,DIM_NY,12)
      REAL ESTCRPT(DIM_NA,DIM_NY,14)
      REAL SMEF
      
c ew 03/12/2004; Consider drain/tailwater supply
      INTEGER IDRAIN
      CHARACTER*200 DRAFILE
c jhb declare/dimension sub-basin (sb) common block variables
      integer :: sbcount, sbsb(dim_na)
      logical :: sboutput, sb_flag, sb0_flag, b_flag
      character*6 :: sbid(0:dim_nsb)
      character*24 :: sbname(0:dim_nsb)
      integer :: sbstrcnt(0:dim_nsb)
      real :: sbtarea(0:dim_nsb,dim_ny,12), sbmarea(0:dim_nsb,dim_ny,12)
      real :: sbettot(0:dim_nsb,dim_ny,12),sbeffppt(0:dim_nsb,dim_ny,12)
      real :: sbreqt(0:dim_nsb,dim_ny,12), sbwbu(0:dim_nsb,dim_ny,12)
      real ::sbwbused(0:dim_nsb,dim_ny,12),sbreqreq(0:dim_nsb,dim_ny,12)
      real :: sbdivsup(0:dim_nsb,dim_ny,12),sbcloss(0:dim_nsb,dim_ny,12)
      real :: sbceff(0:dim_nsb,dim_ny,12),sbfdiv(0:dim_nsb,dim_ny,12)
      real :: sbsfeff(0:dim_nsb,dim_ny,12)
      real :: sbeffcu(0:dim_nsb,dim_ny,12)
      real :: sbseffcu(0:dim_nsb,dim_ny,12)
      real :: sbarech(0:dim_nsb,dim_ny,12)
      real :: sbtail(0:dim_nsb,dim_ny,12)
      real :: sbseniorf(0:dim_nsb,dim_ny,12)
      real :: sbjuniorf(0:dim_nsb,dim_ny,12)
      real :: sbotherf(0:dim_nsb,dim_ny,12)
      real :: sbcrop_cus(0:dim_nsb,dim_ny,12)
      real :: sbcrop_cuj(0:dim_nsb,dim_ny,12)
      real :: sbcrop_cuo(0:dim_nsb,dim_ny,12)
      real :: sbcrop_cut(0:dim_nsb,dim_ny,12)
      real :: sbsoil_cus(0:dim_nsb,dim_ny,12)
      real :: sbsoil_cuj(0:dim_nsb,dim_ny,12)
      real :: sbsoil_cuo(0:dim_nsb,dim_ny,12)
      real :: sbsoil_cu(0:dim_nsb,dim_ny,12)
      real :: sbsoiltotts(0:dim_nsb,dim_ny,12)
      real :: sbsoiltottj(0:dim_nsb,dim_ny,12)
      real :: sbsoiltotto(0:dim_nsb,dim_ny,12)
      real :: sbsoiltott(0:dim_nsb,dim_ny,12)
      real :: sbcropcusoil(0:dim_nsb,dim_ny,12)
      real :: sbestcrps(0:dim_nsb,dim_ny,12)
      real :: sbestcrpj(0:dim_nsb,dim_ny,12)
      real :: sbestcrpo(0:dim_nsb,dim_ny,12)
      real :: sbestcrpt(0:dim_nsb,dim_ny,12)
      real::sbdivcus(0:dim_nsb,dim_ny,12),sbdivcuj(0:dim_nsb,dim_ny,12)
      real::sbdivcuo(0:dim_nsb,dim_ny,12),sbdivcu(0:dim_nsb,dim_ny,12)
      real :: sbgdiv(0:dim_nsb,dim_ny,12)
      real :: sbgwcu(0:dim_nsb,dim_ny,12)
      real :: sbeffgw(0:dim_nsb,dim_ny,12)
      real :: sbgwro(0:dim_nsb,dim_ny,12)
      real :: sbcutot(0:dim_nsb,dim_ny,12)
      real :: sbcustot(0:dim_nsb,dim_ny,12)
      real :: sbtdp(0:dim_nsb,dim_ny,12)
      real :: sbgsdiv(0:dim_nsb,dim_ny,12)
      real :: sbgfdiv(0:dim_nsb,dim_ny,12)
      real :: sbulags(0:dim_nsb,dim_ny,12),sbulagj(0:dim_nsb,dim_ny,12)
      real :: sbulago(0:dim_nsb,dim_ny,12),sbulagt(0:dim_nsb,dim_ny,12)
      real :: sblagrets(0:dim_nsb,dim_ny,12)
      real :: sblagretj(0:dim_nsb,dim_ny,12)
      real :: sblagreto(0:dim_nsb,dim_ny,12)
      real :: sblagrett(0:dim_nsb,dim_ny,12)
      real :: sbtotret(0:dim_nsb,dim_ny,12)
      real :: sblaglates(0:dim_nsb,dim_ny,12)
      real :: sblaglatej(0:dim_nsb,dim_ny,12)
      real :: sblaglateo(0:dim_nsb,dim_ny,12)
      real :: sblaglatet(0:dim_nsb,dim_ny,12)
      real :: sbdeps(0:dim_nsb,dim_ny,12)
      real :: sbdepj(0:dim_nsb,dim_ny,12)
      real :: sbdepo(0:dim_nsb,dim_ny,12)
      real :: sbdept(0:dim_nsb,dim_ny,12)
      real :: sbshortage(0:dim_nsb,dim_ny,12)
      real :: sbgrass(0:dim_nsb,dim_ny,12,10)
c jhb declare/dimension basin (b) common block variables
      character*2 :: bid
      character*24 :: bname
      real :: btarea(dim_ny,12), bmarea(dim_ny,12)
      real :: bettot(dim_ny,12),beffppt(dim_ny,12)
      real :: breqt(dim_ny,12), bwbu(dim_ny,12)
      real :: bwbused(dim_ny,12),breqreq(dim_ny,12)
      real :: bdivsup(dim_ny,12),bcloss(dim_ny,12)
      real :: bceff(dim_ny,12),bfdiv(dim_ny,12)
      real :: bsfeff(dim_ny,12)
      real :: beffcu(dim_ny,12)
      real :: bseffcu(dim_ny,12)
      real :: barech(dim_ny,12)
      real :: btail(dim_ny,12)
      real :: bseniorf(dim_ny,12), bjuniorf(dim_ny,12)
      real :: botherf(dim_ny,12)
      real :: bcrop_cus(dim_ny,12),bcrop_cuj(dim_ny,12)
      real :: bcrop_cuo(dim_ny,12),bcrop_cut(dim_ny,12)
      real :: bsoil_cus(dim_ny,12),bsoil_cuj(dim_ny,12)
      real :: bsoil_cuo(dim_ny,12),bsoil_cu(dim_ny,12)
      real :: bsoiltotts(dim_ny,12),bsoiltottj(dim_ny,12)
      real :: bsoiltotto(dim_ny,12),bsoiltott(dim_ny,12)
      real :: bcropcusoil(dim_ny,12)
      real :: bestcrps(dim_ny,12),bestcrpj(dim_ny,12)
      real :: bestcrpo(dim_ny,12),bestcrpt(dim_ny,12)
      real :: bdivcus(dim_ny,12),bdivcuj(dim_ny,12)
      real :: bdivcuo(dim_ny,12),bdivcu(dim_ny,12)
      real :: bgdiv(dim_ny,12)
      real :: bgwcu(dim_ny,12)
      real :: beffgw(dim_ny,12)
      real :: bgwro(dim_ny,12)
      real :: bcutot(dim_ny,12)
      real :: bcustot(dim_ny,12)
      real :: btdp(dim_ny,12)
      real :: bgsdiv(dim_ny,12)
      real :: bgfdiv(dim_ny,12)
      real :: bulags(dim_ny,12),bulagj(dim_ny,12)
      real :: bulago(dim_ny,12),bulagt(dim_ny,12)
      real :: blagrets(dim_ny,12)
      real :: blagretj(dim_ny,12)
      real :: blagreto(dim_ny,12)
      real :: blagrett(dim_ny,12)
      real :: btotret(dim_ny,12)
      real :: blaglates(dim_ny,12)
      real :: blaglatej(dim_ny,12)
      real :: blaglateo(dim_ny,12)
      real :: blaglatet(dim_ny,12)
      real :: bdeps(dim_ny,12)
      real :: bdepj(dim_ny,12)
      real :: bdepo(dim_ny,12)
      real :: bdept(dim_ny,12)
      real :: bshortage(dim_ny,12)
      real :: bgrass(dim_ny,12,10)

c rb- added fn_len to common
      COMMON /G0/ DAYONE,fn_len,rspck,fmode
c grb add dddopt and admopt variables  06-29-00
      common /g1/ persen,peroth
      COMMON /G1b/ admopt,dddopt,idaily,iclim,iwheat
      COMMON /G1a/ FLAG1, FLAG2, FLAG3, ISUPLY, GWFLAG,ddcsw, iflood
      COMMON /G2/ WLAT,WELEV
      COMMON /G3/ N_STA, N_CRPS, N_CRP
      COMMON /G4/ WWS, WRS, OTA, OPA, TRAIN
      COMMON /G5/ NYRS, NYR1, NYR2, RN_XCO, RN_REF
      COMMON /G5a/ GNYRS, GNYR1, GNYR2
      COMMON /G6/ T_AREA, M_AREA, MAD, NU_NME, NU_SUM, SUM_Y
      COMMON /G6a/C_AREA, TFLG1, TFLG2, NU_DIM
      COMMON /G7/ ISM,SMEF,WBU,WINTPREC,WBUSED
      COMMON /G8/ RPARA
      COMMON /G9/ N_SOIL,CRPTYP
      COMMON /G10/ TMOIS1, TMOIS2, AWC
      COMMON /G10a/RZ, IRZ, APD
      COMMON /G11/ GDATE1, GDATE2, GDATE3
      COMMON /G11a/GDATE4, GDATE5, FRZ, GDATES
      COMMON /MISC/ WARNINGS
      COMMON /FILL_LOG/ FILLONFLY
      COMMON /FILL_INT/ CLIMONFLY,DIVONFLY
      COMMON /GG1a/ MONTH
      COMMON /GG1b/ MIDDLE
      COMMON /GG1c/ IQUOTE
c grb 06-29-00 add admfile and dddfile
      COMMON /GG2a/ DBYEAR,DEYEAR,IFIR
      COMMON /GG2b/ ddstrctt,ddindex,iddrec
      COMMON /GG2c/ ddstruct,imissflg,TRIGHTS
      common /gg2d/ admfile,dddfile,addfile,dcufile
      common /gg2e/ rcufile,ccufile,kbcfile,tmpfile,pptfile
      common /gg2f/ ddhfile,othfile,logfile,cdsfile,fdfile,cchfile
      common /gg2g/ kpmfile,strfile,ddsfile,kmhfile
      common /gg2h/ wsid,divsup,ddrfile,penfile,vapfile
      common /gg2i/ tmxfile,tmnfile,pdyfile,solfile,wndfile
      common /gg2j/ dlyfile,ipyfile,pvhfile,clifile,dlafile
c  grb add soil mositure priority and presimulation variables as well as ending soil moisture values
      common /gg2k/ ipresim,iprtysm,iagg,ipdy,imiss2
      common /gg2l/ endsoils,endsoilj,endsoilo
c grb 05-20-00 add variable missflag and ktsw to common
      common /gg2m/ missflag,missflg1,ktsw,missiwr,missdiv
c ew 03/12/2004 add variables to Consider drain/tailwater supply
      common /gg2n/ idrain,drafile
c
c ew added common block g12 for variables that used to be written to tmp files
c     variables are the same for Blanney Criddle and Penman-Montieth since methods
c     are not used simultaneously
c
      COMMON /G12/REQT,DEPLET,ESTCRPT,ETTOT,EFFPPT,GWP
c
c ew added common block for supply limit variables, diversions and water rights
c
      COMMON /G14/AWCR,ADMINENT,APER,DPER,RETN,SCAPATOT,EFF,
     !      NAMEASP,PSENMO,PJUNMO,POTHMO,SENASP,JUNASP
C
C ew added common block to separate "other" use variables from variables used
c    in blaney criddle estimated
c    
      COMMON /G15/ NHUC,OCOUNT,OHUC,TTHUC,TTCOUNT,ONYR1,ONYR2,
     !             ONYRS,N_WTS,TTID,TTWWS,TTWRS,OWWS,OWRS,TMPT,TMPR
cjhb
cjhb  added common block for deficit irrigation parameters
      COMMON /G16/ def_irr, def_irr_frac
cjhb  added common block for all the sub-basin or district totals
      COMMON /SUBBASIN/ sbcount, sbstrcnt, sbsb, sbtarea, sbmarea,
     &            sbettot, sbeffppt, sbreqt,
     &            sbwbu, sbwbused, sbreqreq,
     &            sbceff, sbcloss, sbfdiv, sbtail,
     &            sbseniorf, sbjuniorf, sbotherf, sbdivsup,
     &            sbarech,
     &            sbcrop_cus,sbcrop_cuj,sbcrop_cuo,sbcrop_cut,
     &            sbsoil_cus,sbsoil_cuj,sbsoil_cuo,sbsoil_cu,
     &            sbsoiltotts,sbsoiltottj,sbsoiltotto,sbsoiltott,
     &            sbcropcusoil,
     &            sbestcrps,sbestcrpj,sbestcrpo,sbestcrpt,
     &            sbdivcus,sbdivcuj,sbdivcuo,sbdivcu,
     &            sbgdiv,sbgwcu,sbeffgw,sbgwro,
     &            sbgsdiv, sbgfdiv,
     &            sbcutot, sbcustot, sbtdp,
     &            sbulags,sbulagj,sbulago,sbulagt,
     &            sblagrets,sblagretj,sblagreto,sblagrett,sbtotret,
     &            sblaglates,sblaglatej,sblaglateo,sblaglatet,
     &            sbdeps,sbdepj,sbdepo,sbdept,
     &            sbsfeff, sbeffcu, sbseffcu, sbshortage,
     &            sbgrass
      COMMON /BASIN/ btarea, bmarea,
     &            bettot, beffppt, breqt,
     &            bwbu, bwbused, breqreq,
     &            bceff, bcloss, bfdiv, btail,
     &            bseniorf, bjuniorf, botherf, bdivsup, 
     &            barech,
     &            bcrop_cus,bcrop_cuj,bcrop_cuo,bcrop_cut,
     &            bsoil_cus,bsoil_cuj,bsoil_cuo,bsoil_cu,
     &            bsoiltotts,bsoiltottj,bsoiltotto,bsoiltott,
     &            bcropcusoil,
     &            bestcrps,bestcrpj,bestcrpo,bestcrpt,
     &            bdivcus,bdivcuj,bdivcuo,bdivcu,
     &            bgdiv,bgwcu,beffgw,bgwro,
     &            bgsdiv, bgfdiv,
     &            bcutot, bcustot, btdp,
     &            bulags,bulagj,bulago,bulagt,
     &            blagrets,blagretj,blagreto,blagrett,btotret,
     &            blaglates,blaglatej,blaglateo,blaglatet,
     &            bdeps,bdepj,bdepo,bdept,
     &            bsfeff, beffcu, bseffcu, bshortage,
     &            bgrass
      COMMON /SUBBASIN1/ sbid, sbname
      COMMON /BASIN1/ bid, bname
      COMMON /SUBBASIN2/ sboutput, sb_flag, sb0_flag, b_flag
      COMMON /LOGWARN/ scu_debug,lgwrcnt,lgwrscnt,lgwrswcnt,
     &                  lgwrmsg1,lgwrmsg2,lgwrmsg3,lgwrsid
      COMMON /TAB3/ CPNAME, SOIL, QUOTE
      COMMON /TAB3a/ BAS_ID
      COMMON /TAB3b/ SLINE
      COMMON /TAB3c/ DLINE
      COMMON /TAB3d/ AMN
      COMMON /TAB4/ TITLE, STNAME, PPNAME, CURDATE, CURTIME
      COMMON /TAB4a/ dfile
      COMMON /TAB4b/ SNAME
      COMMON /TAB9aa/ SLLINE
      COMMON /TAB9bb/ DLLINE
      COMMON /TAB5a/ TYPOUT, RNTOT, S4CATID
      COMMON /TAB6/  BLAT,BELEV
      COMMON /TAB6a/ JBEG, JEND, JEXIST
      COMMON /TAB7a/ TMEAN
      COMMON /TAB7b/ T28, T32
      COMMON /TAB9a/ CUT2, CUT3
      COMMON /TAB10/ NBASIN, CKEY, BKEY
      COMMON /TAB10a/NPARCE
      COMMON /TAB11/ AREA
      COMMON /TAB11a/ PJAREA
      COMMON /TAB15/ INCH, SOUT
c
c rrb 2003/06/20; Allow multiple subirrigation crops
      common /subirr/ subname
      REAL FT_M
      PARAMETER (FT_M = 0.304878)
