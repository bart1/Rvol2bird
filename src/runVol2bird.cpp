// #include <libvol2bird.h>


#include <Rcpp.h>
// using namespace Rcpp;

// [[Rcpp::depends(RcppGSL)]]
extern "C" {
  namespace vol2bird
  {
#include "libvol2bird/constants.h"

#include "libvol2bird/libvol2bird.h"
#include "librave/polarvolume.h"
#include "rave_io.h"
  
  }
}
//' convert pvol file
//'
//' @param inn the input polar volume file as a string
//' @param the output h5 file for the vp
//' @param vol2BirdConfig a list with config options
//' 
//' @return A character with the vol to bird version
//' 
//' @export
//' 
//' @examples
//' runVol2bird(system.file("extdata", "volume.h5", package = "bioRad"), t<-tempfile(), vol2BirdConfig )
//' vp<-bioRad::read_vpfiles(t)
//' plot(vp)
//' 
// [[Rcpp::export]]
Rcpp::NumericVector runVol2bird(const Rcpp::CharacterVector& inn, const Rcpp::CharacterVector& out,const Rcpp::List& options) {
  
    vol2bird::vol2bird_t alldata;
    
    // number of input files specified on command line
    int nInputFiles = 1;
    // the polar volume file that the user provided as input
    char* fileIn[1];
    // the (optional) vertical profile file that the user specified as output
    const char* fileVpOut = NULL;
    // the (optional) vertical profile file that the user specified as output
    const char* fileVolOut = NULL;
    
    // the polar volume file that the user provided as input
    nInputFiles = 1;
    std::string tmp =Rcpp::as<std::string>(inn);
    std::string tmpout =Rcpp::as<std::string>(out);
    tmp.append("\0");
    tmpout.append("\0");
    
    
    // char bb = Rcpp::as<char>(inn);
    fileIn[0] =&tmp.at(0);    
    fileVpOut =&tmpout.at(0);
    
    // check that input files exist
    for (int i=0; i<nInputFiles; i++){
      
      
      if(!vol2bird::isRegularFile(fileIn[i])){
        Rcpp::stop("Error: input file  does not exist.");
      }
    }
    
    // Initialize hlhdf library
    // NEEDED ? HL_init();
    
    // Make rave and hlhdf library print debugging error messages
    //HL_setDebugLevel(HLHDF_SPEWDEBUG);
    //Rave_initializeDebugger();
    //Rave_setDebugLevel(RAVE_WARNING);
    //Rave_setDebugLevel(RAVE_INFO);
    
    // store the input filename TODO: add other input files
    strcpy(alldata.misc.filename_pvol, fileIn[0]);
    if (fileVpOut != NULL){
      strcpy(alldata.misc.filename_vp,fileVpOut);
    }
    
    // read configuration options
    // int configSuccessful = vol2birdLoadConfig(&alldata) == 0;
    // 
    // if (configSuccessful == FALSE) {
    //   Rcpp::stop("Error: failed to load configuration\n");
    //   // return -1;
    // }

    
    ///////////////////////
    //
    // CONFIG
    //
    //////////////

    (&alldata)->options.azimMin = Rcpp::as<float>(options["AZIMMIN"]);
    (&alldata)->options.azimMax = Rcpp::as<float>(options["AZIMMAX"]);
    (&alldata)->options.layerThickness = Rcpp::as<float>(options["HLAYER"]);
    (&alldata)->options.nLayers = Rcpp::as<int>(options["NLAYER"]);
    (&alldata)->options.rangeMin = Rcpp::as<float>(options["RANGEMIN"]);
    (&alldata)->options.rangeMax = Rcpp::as<float>(options["RANGEMAX"]);
    (&alldata)->options.elevMin = Rcpp::as<float>(options["ELEVMIN"]);
    (&alldata)->options.elevMax = Rcpp::as<float>(options["ELEVMAX"]);
    (&alldata)->options.radarWavelength = Rcpp::as<float>(options["RADAR_WAVELENGTH_CM"]);
    (&alldata)->options.useClutterMap = Rcpp::as<bool>(options["USE_CLUTTERMAP"]);
    (&alldata)->options.clutterValueMin = Rcpp::as<float>(options["CLUTTERVALUEMIN"]);

    char abbbn[1000] ;
    strcpy(abbbn ,Rcpp::as<std::string>(options["CLUTTERMAP"]).append("\0").c_str());
    strcpy((&alldata)->options.clutterMap ,abbbn);
      //  strcpy((&alldata)->options.clutterMap ,Rcpp::as<std::string>(options["CLUTTERMAP"]).c_str());//= Rcpp::as<char[1000]>(options["CLUTTERMAP"]);
    (&alldata)->options.printDbz = Rcpp::as<bool>(options["PRINT_DBZ"]);
    (&alldata)->options.printDealias = Rcpp::as<bool>(options["PRINT_DEALIAS"]);
    (&alldata)->options.printVrad = Rcpp::as<bool>(options["PRINT_VRAD"]);
    (&alldata)->options.printRhohv = Rcpp::as<bool>(options["PRINT_RHOHV"]);
    (&alldata)->options.printTex = Rcpp::as<bool>(options["PRINT_TEXTURE"]);
    (&alldata)->options.printCell = Rcpp::as<bool>(options["PRINT_CELL"]);
    (&alldata)->options.printCellProp = Rcpp::as<bool>(options["PRINT_CELL_PROP"]);
    (&alldata)->options.printClut = Rcpp::as<bool>(options["PRINT_CLUT"]);
    (&alldata)->options.printOptions = Rcpp::as<bool>(options["PRINT_OPTIONS"]);
    (&alldata)->options.printProfileVar = Rcpp::as<bool>(options["PRINT_PROFILE"]);
    (&alldata)->options.printPointsArray = Rcpp::as<bool>(options["PRINT_POINTS_ARRAY"]);
    (&alldata)->options.fitVrad = Rcpp::as<bool>(options["FIT_VRAD"]);
    (&alldata)->options.exportBirdProfileAsJSONVar = Rcpp::as<bool>(options["EXPORT_BIRD_PROFILE_AS_JSON"]);

    (&alldata)->options.minNyquist = Rcpp::as<float>(options["MIN_NYQUIST_VELOCITY"]);
    
    
    
    (&alldata)->options.minNyquist = Rcpp::as<float>(options["MIN_NYQUIST_VELOCITY"]);
    (&alldata)->options.maxNyquistDealias = Rcpp::as<float>(options["MAX_NYQUIST_DEALIAS"]);
    (&alldata)->options.birdRadarCrossSection = Rcpp::as<float>(options["SIGMA_BIRD"]);
    (&alldata)->options.cellStdDevMax = Rcpp::as<float>(options["STDEV_CELL"]);

    (&alldata)->options.stdDevMinBird = Rcpp::as<float>(options["STDEV_BIRD"]);
    (&alldata)->options.etaMax = Rcpp::as<float>(options["ETAMAX"]);
    (&alldata)->options.cellEtaMin = Rcpp::as<float>(options["ETACELL"]);
  //  strcpy((&alldata)->options.dbzType ,Rcpp::as<std::string>(options["DBZTYPE"]).c_str());
    char abbb[10] ;
    strcpy(abbb ,Rcpp::as<std::string>(options["DBZTYPE"]).append("\0").c_str());
    strcpy((&alldata)->options.dbzType ,abbb);
    

    (&alldata)->options.requireVrad = Rcpp::as<bool>(options["REQUIRE_VRAD"]);
    (&alldata)->options.dealiasVrad = Rcpp::as<bool>(options["DEALIAS_VRAD"]);
    (&alldata)->options.dealiasRecycle = Rcpp::as<bool>(options["DEALIAS_RECYCLE"]);
    (&alldata)->options.dualPol = Rcpp::as<bool>(options["DUALPOL"]);
    (&alldata)->options.singlePol = Rcpp::as<bool>(options["SINGLEPOL"]);
    (&alldata)->options.resample = Rcpp::as<bool>(options["RESAMPLE"]);
    
    (&alldata)->options.dbzThresMin = Rcpp::as<float>(options["DBZMIN"]);
    (&alldata)->options.rhohvThresMin = Rcpp::as<float>(options["RHOHVMIN"]);
    (&alldata)->options.resampleRscale = Rcpp::as<float>(options["RESAMPLE_RSCALE"]);
    (&alldata)->options.resampleNbins = Rcpp::as<int>(options["RESAMPLE_NBINS"]);
    (&alldata)->options.resampleNrays = Rcpp::as<int>(options["RESAMPLE_NRAYS"]);
    
    (&alldata)->options.mistNetElevsOnly = Rcpp::as<bool>(options["MISTNET_ELEVS_ONLY"]);
    (&alldata)->options.useMistNet = Rcpp::as<bool>(options["USE_MISTNET"]);
   char abb[1000] ;
   strcpy(abb ,Rcpp::as<std::string>(options["MISTNET_PATH"]).append("\0").c_str());
   strcpy((&alldata)->options.mistNetPath ,abb);
    
Rcpp::NumericVector aa= Rcpp::as<Rcpp::NumericVector>(options["MISTNET_ELEVS"]);
(&alldata)->options.mistNetNElevs = aa.size();

    // alldata->options.mistNetNElevs = cfg_size(*cfg, "MISTNET_ELEVS");
     for(int i=0; i<(&alldata)->options.mistNetNElevs; i++){
       (&alldata)->options.mistNetElevs[i] = aa[i];
     }
    
    // 
    // 
    // // ------------------------------------------------------------- //
    // //              vol2bird options from constants.h                //
    // // ------------------------------------------------------------- //
    // 
    (&alldata)->constants.areaCellMin = AREACELL;
    (&alldata)->constants.cellClutterFractionMax = CLUTPERCCELL;
    (&alldata)->constants.chisqMin = CHISQMIN;
    (&alldata)->constants.fringeDist = FRINGEDIST;
    (&alldata)->constants.nBinsGap = NBINSGAP;
    (&alldata)->constants.nPointsIncludedMin = NDBZMIN;
    (&alldata)->constants.nNeighborsMin = NEIGHBORS;
    (&alldata)->constants.nObsGapMin = NOBSGAPMIN;
    (&alldata)->constants.nAzimNeighborhood = NTEXBINAZIM;
    (&alldata)->constants.nRangNeighborhood = NTEXBINRANG;
    (&alldata)->constants.nCountMin = NTEXMIN; 
    (&alldata)->constants.refracIndex = REFRACTIVE_INDEX_OF_WATER;
    (&alldata)->constants.absVDifMax = VDIFMAX;
    (&alldata)->constants.vradMin = VRADMIN;
    // 
    // // ------------------------------------------------------------- //
    // //       some other variables, derived from user options         //
    // // ------------------------------------------------------------- //
    // 
    (&alldata)->misc.rCellMax = (&alldata)->options.rangeMax + RCELLMAX_OFFSET;
    (&alldata)->misc.nDims = 2;
    (&alldata)->misc.nParsFitted = 3;
    // 
    // // the following settings depend on wavelength, will be set in Vol2birdSetup
    (&alldata)->misc.dbzFactor = NAN;
    (&alldata)->misc.dbzMax = NAN;
    (&alldata)->misc.cellDbzMin = NAN;
    // 
    (&alldata)->misc.loadConfigSuccessful = TRUE;    
    
    
    ///////////////////////
    //
    // END CONFIG
    //
    //////////////
    
    
    
    

    // read in data up to a distance of alldata.misc.rCellMax
    // we do not read in the full volume for speed/memory
    vol2bird::PolarVolume_t* volume = NULL;
    
    volume = vol2bird::vol2birdGetVolume(fileIn, nInputFiles, 1000000,1);
    //volume = vol2birdGetVolume(fileIn, nInputFiles, alldata.misc.rCellMax,1);
    if (volume == NULL) {
      Rcpp::stop("Error: failed to read radar volume");
    }
    
    // loading static clutter map upon request
    if (alldata.options.useClutterMap){
      int clutterSuccessful = vol2bird::vol2birdLoadClutterMap(volume, alldata.options.clutterMap,alldata.misc.rCellMax) == 0;
      
      if (clutterSuccessful == FALSE) {
        Rcpp::stop("Error: failed to load static clutter map");
      }
      
    }

    // resample the volume upon request
    if (alldata.options.resample) {
      vol2bird::PolarVolume_t* volume_orig = volume;
      volume = PolarVolume_resample(volume,alldata.options.resampleRscale,
                                    alldata.options.resampleNbins,alldata.options.resampleNrays);
      if (volume == NULL) {
        Rcpp::stop("Error: volume resampling failed");
      }
      // vol2bird::RAVE_OBJECT_RELEASE(volume_orig);
    }
    
    // initialize volbird library

    int initSuccessful = vol2bird::vol2birdSetUp(volume, &alldata) == 0;
    
    if (initSuccessful == FALSE) {
      Rcpp::stop("Error: failed to initialize vol2bird");
    }

    // output (optionally de-aliased) volume
    if (fileVolOut != NULL){
      vol2bird::saveToODIM((vol2bird::RaveCoreObject*) volume, fileVolOut);
    }

    // call vol2bird's main routine
  vol2bird::vol2birdCalcProfiles(&alldata);

  
  // ------------------------------------------------------------------- //
  //  using getter functions to access at the profile data               //
  // ------------------------------------------------------------------- //
  // const char* date;
  // const char* time;
  // const char* source;
  // 
  // date = vol2bird::PolarVolume_getDate(volume);
  // time = vol2bird::PolarVolume_getTime(volume);
  // source = vol2bird::PolarVolume_getSource(volume);
  
  // {  // getter example scope begin
  //   
  //   int nRowsProfile = vol2birdGetNRowsProfile(&alldata);
  //   int nColsProfile = vol2birdGetNColsProfile(&alldata);
  //   
  //   fprintf(stdout, "# vol2bird Vertical Profile of Birds (VPB)\n");
  //   fprintf(stdout, "# source: %s\n",source);
  //   fprintf(stdout, "# polar volume input: %s\n",fileIn[0]);
  //   if (alldata.misc.vcp > 0) fprintf(stdout, "# volume coverage pattern (VCP): %i\n", alldata.misc.vcp);
  //   printf("# date   time HGHT    u      v       w     ff    dd  sd_vvp gap dbz     eta   dens   DBZH   n   n_dbz n_all n_dbz_all\n");
  //   
  //   float *profileBio;
  //   float *profileAll;
  //   
  //   profileBio = vol2birdGetProfile(1, &alldata);
  //   profileAll = vol2birdGetProfile(3, &alldata);
  //   
  //   int iRowProfile;
  //   int iCopied = 0;
  //   
  //   for (iRowProfile = 0; iRowProfile < nRowsProfile; iRowProfile++) {
  //     iCopied=iRowProfile*nColsProfile;
  //     printf("%8s %.4s ",date,time);
  //     printf("%4.f %6.2f %6.2f %7.2f %5.2f %5.1f %6.2f %1c %6.2f %6.1f %6.2f %6.2f %5.f %5.f %5.f %5.f\n",
  //            profileBio[0+iCopied],
  //                      nanify(profileBio[2+iCopied]),nanify(profileBio[3+iCopied]),
  //                      nanify(profileBio[4+iCopied]),nanify(profileBio[5+iCopied]),
  //                      nanify(profileBio[6+iCopied]),nanify(profileAll[7+iCopied]),
  //                      profileBio[8+iCopied] == TRUE ? 'T' : 'F',
  //                      nanify(profileBio[9+iCopied]),nanify(profileBio[11+iCopied]),
  //                      nanify(profileBio[12+iCopied]),nanify(profileAll[9+iCopied]),
  //                      nanify(profileBio[10+iCopied]),nanify(profileBio[13+iCopied]),
  //                      nanify(profileAll[10+iCopied]),nanify(profileAll[13+iCopied]));
  //   }
  //   
  //   profileAll = NULL;
  //   profileBio = NULL;
  //   free((void*) profileAll);
  //   free((void*) profileBio);
  //   
  //   //}
  // } // getter scope end
  // 
  // 
  
  // ------------------------------------------------------------------- //
  //                 end of the getter example section                   //
  // ------------------------------------------------------------------- //            
  
  //map vol2bird profile data to Rave profile object
  mapDataToRave(volume, &alldata);

  //save rave profile to ODIM hdf5 file
  if (fileVpOut != NULL){
    int result;
    result = vol2bird::saveToODIM((vol2bird::RaveCoreObject*) alldata.vp, fileVpOut); 
    if (result == FALSE){
      Rcpp::stop( "critical error, cannot write file");
//      return -1;
    }
  }

  // tear down vol2bird, give memory back
  vol2bird::vol2birdTearDown(&alldata);
  // vol2bird::RAVE_OBJECT_RELEASE(volume);

  
  // output some performance data
  //clock_gettime(CLOCK_REALTIME, &ts);
  //double nSeconds = ((double) ts.tv_nsec)/1e9;
  //fprintf(stderr, "Processing done in %.2f seconds\n",nSeconds);
  
  
  
  return 0;
  
}






