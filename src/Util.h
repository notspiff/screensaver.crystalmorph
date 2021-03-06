#pragma once

#include "types.h"

struct CUSTOMVERTEX
{
  float x, y, z; // The untransformed position for the vertex.
  float nx, ny, nz; // Normal vector for lighting calculations	
  CRGBA color; // The vertex colour.
};

CRGBA HSVtoRGB( float h, float s, float v );
inline float frand(){return ((float) rand() / (float) RAND_MAX);};

#define iMin(a,b) ((a)<(b)?(a):(b))
#define iMax(a,b) ((a)>(b)?(a):(b))

void incrementColor();
CRGBA randColor();



class MorphColor
{
  public:
    MorphColor(int speed = 50);
    CRGBA getColor(){return currentColor;};
    void incrementColor();
  
  private:
    void reset(const CRGBA& color);
    CRGBA currentColor;
    CRGBA startColor;
    CRGBA endColor;
    float ratio;
    int steps;
    int colorSpeed;
};
