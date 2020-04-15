// #include <libvol2bird.h>


#include <Rcpp.h>
// using namespace Rcpp;

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
    int configSuccessful = vol2birdLoadConfig(&alldata) == 0;
    
    if (configSuccessful == FALSE) {
      Rcpp::stop("Error: failed to load configuration\n");
      // return -1;
    }
    
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






