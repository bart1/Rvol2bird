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
//' Returns vol2bird version
//'
//' @return A character with the vol to bird version
//' 
//' 
// [[Rcpp::export]]
Rcpp::CharacterVector vol2birdVersion() {
  // const char* a= vol2bird::libvol2bird_version() ;

  return Rcpp::wrap(vol2bird::libvol2bird_version()) ;
}

