/* gps.cpp: Function implementations for GPS coordinates.
 * You MAY NOT CHANGE this file.
 */

#include "gps.h"
#include <cmath>
using namespace std;

// mean radius of the earth in miles
#define EARTH_RADIUS (3958.7613)


// helper functions to convert between degrees and radians
double d2r(double deg) {
  return deg * M_PI / 180.0;
}

// creates a gpsco from latitude and longitude in degrees
gpsco make_gps(double lat, double lon) 
{
  gpsco pt = {lat, lon};
  return pt;
}

// the haversine of theta = (sin(theta/2))^2
double hav(double rads) {
  double s = sin(rads/2);
  return s*s;
}

// returns the distance in miles between two GPS points
double gps_dist(gpsco p1, gpsco p2) {
  // haversine formula: https://en.wikipedia.org/wiki/Haversine_formula
  double h = hav(d2r(p2.lat) - d2r(p1.lat))
             + cos( d2r(p1.lat) ) * cos( d2r(p2.lat) )
               * hav( d2r(p2.lon) - d2r(p1.lon) );
  return EARTH_RADIUS * 2 * atan2(sqrt(h), sqrt(1.0-h));
}
