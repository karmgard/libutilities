#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <limits>
const float inf = std::numeric_limits<float>::infinity();
const double epsilon = std::numeric_limits<double>::min();

#define MAX_UL_INT 0xffffffff
#define MAX_INT    0x7fffffff

// Conversion between degrees and radians
#define DEG_2_RAD 0.0174533
#define RAD_2_DEG 57.2958

// Various pieces of pie
#define PI        3.14159265359
#define PI_2      1.57079632679
#define TWO_PI    2.0 * PI

// Quick & dirty template for determining the sign of a number
template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

/*-- Define a couple of new (personal) variable types --*/
typedef struct { float r, i; } complex;
typedef struct { float x, y; } gaussf;
typedef struct { double x, y; } gaussd;

double dround ( double number );
float  fround ( float number );
float  Cabs   ( complex z );

void   randomize ( unsigned long int = 0 );

double randd ( void );
float  randf ( void );
gaussf randgf( float=0.0f, float=1.0f );
gaussd randgd( void );

#endif
