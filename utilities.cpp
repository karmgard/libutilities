#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

// PRECISION should be <= 5 for floating point numbers, 
// otherwise you'll get a core dump
#define PRECISION 2

const double ONE_OVER_RAND_MAX = 1.0f/(RAND_MAX + 1.0f);

double randd( void ) {
  return ONE_OVER_RAND_MAX*random();
}
float randf( void ) {
  return (float)ONE_OVER_RAND_MAX*random();
}
gaussf randgf(float mu, float sigma) {
  //const double epsilon = std::numeric_limits<double>::min();
  //const double two_pi = 2.0*3.14159265358979323846;

  if ( sigma == 0 )
    sigma = 2.0f * mu;

  /*static double z0, z1;
  static bool generate;
  generate = !generate;

  if (!generate)
  return z1 * sigma + mu;*/

  double u1, u2, z0, z1;
  do {
    u1 = randf();
    u2 = randf();
  }
  while ( u1 <= epsilon );

  z0 = sqrt(-2.0 * log(u1)) * cos(TWO_PI * u2);
  z1 = sqrt(-2.0 * log(u1)) * sin(TWO_PI * u2);

  float x = (float)(z0 * sigma + mu);
  float y = (float)(z1 * sigma + mu);
  gaussf g{x,y};

  if ( sign(mu+3*sigma) == sign(mu-3*sigma) ) {
    g.x *= (randf() < 0.5) ? -1 : 1;
    g.y *= (randf() < 0.5) ? -1 : 1;
  }
  return g;
}

gaussd randgd( void ) {
  double x1, x2, w=1.0, y1, y2;
 
  while (w >= 1.0) {
    x1 = 2.0 * randd() - 1.0;
    x2 = 2.0 * randd() - 1.0;
    w = x1 * x1 + x2 * x2;
  }

  w = sqrt( (-2.0 * log( w ) ) / w );
  y1 = x1 * w;
  y2 = x2 * w;

  return gaussd{y1,y2};
}

char state[256];
void randomize( unsigned long int seed ) {
  int filedes = 0;

  if ( !seed ) {
    errno = 0;
    if ( (filedes=open("/dev/urandom", O_RDONLY)) > 0 ) {
      ssize_t res = read(filedes, (void *)&seed, sizeof(unsigned long int));
      if ( !res ) {
	perror("Unable to seed random number generator!\n");
	exit(errno);
      }
      close(filedes);
    } else {
      perror("Unable to seed random number generator!\n");
      exit(errno);
    }
    // Dump out the seed in case we'd like to do this exact run again
    fprintf(stderr, "Starting run with random seed: %lu\n", seed);
  }

  initstate( seed, (char *)state, 256);
  srandom(seed);

  return;
}

/*-- Simple, locally available, functions to round a floating point numbers --*/
double dround( double number ) {
  double rounded_number =  0.0f;

  char   rounded_number_string[PRECISION];
  char   format[8];
  
  sprintf( format, "%%.%if", PRECISION );
  sprintf( rounded_number_string, format, number );

  rounded_number = strtod(rounded_number_string, NULL);

  return rounded_number;
}

float fround( float number ) {

  float rounded_number =  0.0f;

  char   rounded_number_string[PRECISION];
  char   format[8];
  
  sprintf( format, "%%.%if", PRECISION );
  sprintf( rounded_number_string, format, number );

  rounded_number = strtof(rounded_number_string, NULL);

  return rounded_number;
}

/*-- Simple local function to return |z| rapidly --*/
float Cabs(complex z) { 
  float x, y, ans, temp; 

  x = fabs(z.r); 
  y = fabs(z.i); 

  if ( x == 0.0 ) 
    ans = y; 
  else if ( y == 0.0 ) 
    ans = x; 
  else if ( x > y ) { 
    temp = y/x; 
    ans = x*sqrt( 1.0 + temp*temp ); 
  } else { 
    temp = x/y; 
    ans = y*sqrt( 1.0 + temp*temp ); 
  }

  return ans; 
} 

