#include "Body.h"

#define XRANGE (290e9) // width of the solar system in km
#define YRANGE (290e9)
#define ZRANGE (290e9)

#define XACC (100)
#define YACC (100)
#define ZACC (100)

#define XVEL (100e3) // plants velocity are in about ~1000s km/ hour
#define YVEL (100e3)
#define ZVEL (100e3)

#define MASS_MIN (1e10)
#define MASS_MAX (1.9e27) // mass of jupiter ( make it the max )

#define MAXR (1e6)

#define NUM_BODIES (500)


inline double randDoubleRange(double a, double b)
{
  return ((b - a) * ((double)rand() / RAND_MAX)) + a;
}


Body bodies[NUM_BODIES];

bool bodiesInRange(int index, Body newBody){

  for (int i = 0; i < index, i++){

    if (bodies[i].dist(newBody) < MAXR){
      return true;
    }
    return false;

  }

}

int main(){

  vector_3d randPos, randVel, randAcc;
  doube randMass;

  int i = 0;
  while( i < NUM_BODIES){

    randPos[0] = randDoubleRange(0, XRANGE);
    randPos[1] = randDoubleRange(0, YRANGE);
    randPos[2] = randDoubleRange(0, ZRANGE);

    randAcc[0] = randDoubleRange(0, XACC);
    randAcc[1] = randDoubleRange(0, YACC);
    randAcc[2] = randDoubleRange(0, ZACC);

    randVel[0] = randDoubleRange(0, XVEL);
    randVel[1] = randDoubleRange(0, YVEL);
    randVel[2] = randDoubleRange(0, ZVEL);
    
    randMass = randDoubleRange(MASS_MIN, MASS_MAX);

    bodies[i] = Body(i, randMass, randPos, randAcc, randVel);

    if (bodiesInRange(i, bodies[i])){
      continue;
    } else {
      i++;
    }
  }  

}
