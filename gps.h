/* gps.h: header file for GPS coordinates functions
 * You MAY NOT CHANGE this file.
 */

#ifndef GPS_H
#define GPS_H

// stores a GPS coordinate as latitude and longitude in degrees
struct gpsco {
  double lat;
  double lon;
};

// creates a gpsco from latitude and longitude in degrees
gpsco make_gps(double lat, double lon);

// returns the distance in miles between two GPS points
double gps_dist(gpsco p1, gpsco p2);

#endif // GPS_H
