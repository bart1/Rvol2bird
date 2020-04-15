// #include <libvol2bird.h>


#include <Rcpp.h>
// using namespace Rcpp;

// [[Rcpp::depends(RcppGSL)]]
extern "C" {
  namespace vol2bird
  {
#include "libvol2bird/libvol2bird.h"
#include "librave/polarvolume.h"
#include "rave_io.h"
  
  }
}
//' convert pvol file
//'
//' @param inn the input polar volume file as a string
//' @param the output h5 file for the vp
//' 
//' @return A character with the vol to bird version
//' 
//' @export
//' 
//' @examples
//' runVol2bird(system.file("extdata", "volume.h5", package = "bioRad"), t<-tempfile() )
//' vp<-bioRad::read_vpfiles(t)
//' plot(vp)
//' 
// [[Rcpp::export]]
Rcpp::CharacterVector runVol2bird(const Rcpp::CharacterVector& inn, const Rcpp::CharacterVector& out) {
  
  //int main(int argc, char** argv) {
    //    cfg_t* cfg;
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
        std::cout <<"aaaasdfasdfas\n" ;
        
        Rcpp::stop("Error: input file  does not exist.");
        //return -1;
        
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
    (&alldata)->options.azimMax = 360;//cfg_getfloat(*cfg, "AZIMMAX");
    (&alldata)->options.azimMin = 0;//cfg_getfloat(*cfg, "AZIMMIN");
    
    // alldata->options.azimMax = cfg_getfloat(*cfg, "AZIMMAX");
    // alldata->options.azimMin = cfg_getfloat(*cfg, "AZIMMIN");
    // alldata->options.layerThickness = cfg_getfloat(*cfg, "HLAYER");
    // alldata->options.nLayers = cfg_getint(*cfg, "NLAYER");
    // alldata->options.rangeMax = cfg_getfloat(*cfg, "RANGEMAX");
    // alldata->options.rangeMin = cfg_getfloat(*cfg, "RANGEMIN");
    // alldata->options.elevMax = cfg_getfloat(*cfg, "ELEVMAX");
    // alldata->options.elevMin = cfg_getfloat(*cfg, "ELEVMIN");
    // alldata->options.radarWavelength = cfg_getfloat(*cfg, "RADAR_WAVELENGTH_CM");
    // alldata->options.useClutterMap = cfg_getbool(*cfg,"USE_CLUTTERMAP");
    // alldata->options.clutterValueMin = cfg_getfloat(*cfg, "CLUTTERVALUEMIN");
    // strcpy(alldata->options.clutterMap,cfg_getstr(*cfg,"CLUTTERMAP"));
    // alldata->options.printDbz = cfg_getbool(*cfg,"PRINT_DBZ");
    // alldata->options.printDealias = cfg_getbool(*cfg,"PRINT_DEALIAS");
    // alldata->options.printVrad = cfg_getbool(*cfg,"PRINT_VRAD");
    // alldata->options.printRhohv = cfg_getbool(*cfg,"PRINT_RHOHV");
    // alldata->options.printTex = cfg_getbool(*cfg,"PRINT_TEXTURE");
    // alldata->options.printCell = cfg_getbool(*cfg,"PRINT_CELL");
    // alldata->options.printCellProp = cfg_getbool(*cfg,"PRINT_CELL_PROP");
    // alldata->options.printClut = cfg_getbool(*cfg,"PRINT_CLUT");
    // alldata->options.printOptions = cfg_getbool(*cfg,"PRINT_OPTIONS");
    // alldata->options.printProfileVar = cfg_getbool(*cfg,"PRINT_PROFILE");
    // alldata->options.printPointsArray = cfg_getbool(*cfg,"PRINT_POINTS_ARRAY");
    // alldata->options.fitVrad = cfg_getbool(*cfg,"FIT_VRAD");
    // alldata->options.exportBirdProfileAsJSONVar = cfg_getbool(*cfg,"EXPORT_BIRD_PROFILE_AS_JSON"); 
    // alldata->options.minNyquist = cfg_getfloat(*cfg,"MIN_NYQUIST_VELOCITY");
    // alldata->options.maxNyquistDealias = cfg_getfloat(*cfg,"MAX_NYQUIST_DEALIAS");
    // alldata->options.birdRadarCrossSection = cfg_getfloat(*cfg,"SIGMA_BIRD");
    // alldata->options.cellStdDevMax = cfg_getfloat(*cfg,"STDEV_CELL");
    // alldata->options.stdDevMinBird = cfg_getfloat(*cfg,"STDEV_BIRD");
    // alldata->options.etaMax = cfg_getfloat(*cfg,"ETAMAX");
    // alldata->options.cellEtaMin = cfg_getfloat(*cfg,"ETACELL");
    // strcpy(alldata->options.dbzType,cfg_getstr(*cfg,"DBZTYPE"));
    // alldata->options.requireVrad = cfg_getbool(*cfg,"REQUIRE_VRAD");
    // alldata->options.dealiasVrad = cfg_getbool(*cfg,"DEALIAS_VRAD");
    // alldata->options.dealiasRecycle = cfg_getbool(*cfg,"DEALIAS_RECYCLE");
    // alldata->options.dualPol = cfg_getbool(*cfg,"DUALPOL");
    // alldata->options.singlePol = cfg_getbool(*cfg,"SINGLEPOL");
    // alldata->options.dbzThresMin = cfg_getfloat(*cfg,"DBZMIN");
    // alldata->options.rhohvThresMin = cfg_getfloat(*cfg,"RHOHVMIN");
    // alldata->options.resample = cfg_getbool(*cfg,"RESAMPLE");
    // alldata->options.resampleRscale = cfg_getfloat(*cfg,"RESAMPLE_RSCALE");
    // alldata->options.resampleNbins = cfg_getint(*cfg,"RESAMPLE_NBINS");
    // alldata->options.resampleNrays = cfg_getint(*cfg,"RESAMPLE_NRAYS");
    // alldata->options.mistNetNElevs = cfg_size(*cfg, "MISTNET_ELEVS");
    // for(int i=0; i<alldata->options.mistNetNElevs; i++){
    //   alldata->options.mistNetElevs[i] = cfg_getnfloat(*cfg, "MISTNET_ELEVS",i);
    // }
    // alldata->options.mistNetElevsOnly = cfg_getbool(*cfg, "MISTNET_ELEVS_ONLY");
    // alldata->options.useMistNet = cfg_getbool(*cfg, "USE_MISTNET");
    // strcpy(alldata->options.mistNetPath,cfg_getstr(*cfg,"MISTNET_PATH"));
    // 
    // 
    // // ------------------------------------------------------------- //
    // //              vol2bird options from constants.h                //
    // // ------------------------------------------------------------- //
    // 
    // alldata->constants.areaCellMin = AREACELL;
    // alldata->constants.cellClutterFractionMax = CLUTPERCCELL;
    // alldata->constants.chisqMin = CHISQMIN;
    // alldata->constants.fringeDist = FRINGEDIST;
    // alldata->constants.nBinsGap = NBINSGAP;
    // alldata->constants.nPointsIncludedMin = NDBZMIN;
    // alldata->constants.nNeighborsMin = NEIGHBORS;
    // alldata->constants.nObsGapMin = NOBSGAPMIN;
    // alldata->constants.nAzimNeighborhood = NTEXBINAZIM;
    // alldata->constants.nRangNeighborhood = NTEXBINRANG;
    // alldata->constants.nCountMin = NTEXMIN; 
    // alldata->constants.refracIndex = REFRACTIVE_INDEX_OF_WATER;
    // alldata->constants.absVDifMax = VDIFMAX;
    // alldata->constants.vradMin = VRADMIN;
    // 
    // // ------------------------------------------------------------- //
    // //       some other variables, derived from user options         //
    // // ------------------------------------------------------------- //
    // 
    // alldata->misc.rCellMax = alldata->options.rangeMax + RCELLMAX_OFFSET;
    // alldata->misc.nDims = 2;
    // alldata->misc.nParsFitted = 3;
    // 
    // // the following settings depend on wavelength, will be set in Vol2birdSetup
    // alldata->misc.dbzFactor = NAN;
    // alldata->misc.dbzMax = NAN;
    // alldata->misc.cellDbzMin = NAN;
    // 
    // alldata->misc.loadConfigSuccessful = TRUE;    
    
    
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






