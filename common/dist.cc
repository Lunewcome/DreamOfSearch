#include <cmath>
#include <iostream>
using namespace std;

#define zz(s) \
  ((s) * 3.1415927 / 180.0)
#define sqr(s) \
  ((s) * (s))

double Distance(double alongitude,
                double alatitude,
                double blongitude,
                double blatitude) {
  static const double earth_radius = 6378137.0;
  double a = zz(alatitude) - zz(blatitude);
  double b = zz(alongitude) - zz(blongitude);
  double s = 2 * asin(sqrt(sqr(sin(a / 2)) +
                           cos(zz(alatitude)) *
                           cos(zz(blatitude)) *
                           sqr(sin(b / 2))));
  s *= earth_radius;
  return s;
}

// An anti-function of Distance with
// alatitude = blongitude = blatitude = 0.
double FromDistToLongtitude(double dist) {
  static const double common =
      180 / (6378137.0 * 3.1415927);
  return dist * common;
}
