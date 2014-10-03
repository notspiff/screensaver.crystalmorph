#include "Util.h"

CRGBA HSVtoRGB( float h, float s, float v )
{
  int i;
  float f;
  int r, g, b, p, q, t, m;

  if( s == 0 ) { // achromatic (grey)
    r = g = b = (int)(255*v);
    return CRGBA(r,g,b,255);
  }

  h /= 60;      // sector 0 to 5
  i = (int)( h );
  f = h - i;      // frational part of h
  m = (int)(255*v);
  p = (int)(m * ( 1 - s ));
  q = (int)(m * ( 1 - s * f ));
  t = (int)(m * ( 1 - s * ( 1 - f ) ));

  switch( i ) {
    case 0: return CRGBA(m,t,p,255);
    case 1: return CRGBA(q,m,p,255);
    case 2: return CRGBA(p,m,t,255);
    case 3: return CRGBA(p,q,m,255);
    case 4: return CRGBA(t,p,m,255);
    default: break;    // case 5:
  }
  return CRGBA(m,p,q,255);
}

int g_colorType = 1;

void incrementColor()
{
  float val = frand();
  if (val < 0.65)
    g_colorType = 0;
  else if (val < 0.9)
    g_colorType = 1;
  else
    g_colorType = 2;
}

CRGBA randColor()
{
  float h = (float)(rand()%360),s,v;
  switch(g_colorType)
  {
    case 0: 
      h = (float)(rand()%360);
      s = 0.3f + 0.7f*frand();
      v = 0.67f + 0.25f*frand();;
      break;
    case 1: 
      s = 0.9f + 0.1f*frand(); 
      v = 0.67f + 0.3f*frand();
      break;
    default:
      s = 1.0f*frand(); 
      v = 0.3f+0.7f*frand();
  }
  return HSVtoRGB(h,s,v);
}

MorphColor::MorphColor(int speed)
{
  colorSpeed = speed;
  reset(randColor());
}

void MorphColor::reset(const CRGBA& color)
{
  currentColor = startColor = color;
  endColor = randColor();
  ratio = 0.0f;
  steps = colorSpeed + rand() % colorSpeed;
}

void MorphColor::incrementColor()
{
  if (ratio >= 1.0f)
    reset(endColor);
  else
  {
    ratio += 1.0f/(float)steps;
    if (ratio > 1.0f) 
      ratio = 1.0f;
    currentColor = CRGBA::LerpColor(startColor, endColor, ratio);
  }
}
