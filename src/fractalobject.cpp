//////////////////////////////////////////////////////////////////
// FRACTAL.CPP
//
//////////////////////////////////////////////////////////////////
#include "fractalobject.h"       // also includes glu and gl correctly
#include "types.h"

#include <GL/gl.h>
//#include "materials.h"



// stuff for the environment cube
struct FracVertex
{
	CVector position;
	CVector normal;
};

//#define FVF_FRACVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE )
#define FVF_FRACVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL )
// man, how many times have you typed (or pasted) this data for a cube's
// vertices and normals, eh?
FracVertex g_cubeVertices[] =
{
  {CVector(-0.5f, 0.5f,-0.5f), CVector(0.0f, 0.0f,-1.0f)},// D3DCOLOR_RGBA(0,255,0,255)},
  {CVector( 0.5f, 0.5f,-0.5f), CVector(0.0f, 0.0f,-1.0f)},// D3DCOLOR_RGBA(255,255,0,255)},
  {CVector(-0.5f,-0.5f,-0.5f), CVector(0.0f, 0.0f,-1.0f)},// D3DCOLOR_RGBA(0,0,0,255)},
  {CVector( 0.5f,-0.5f,-0.5f), CVector(0.0f, 0.0f,-1.0f)},// D3DCOLOR_RGBA(255,0,0,255)},

  {CVector(-0.5f, 0.5f, 0.5f), CVector(0.0f, 0.0f, 1.0f)},// D3DCOLOR_RGBA(0,255,255,255)},
  {CVector(-0.5f,-0.5f, 0.5f), CVector(0.0f, 0.0f, 1.0f)},// D3DCOLOR_RGBA(0,255,255,255)},
  {CVector( 0.5f, 0.5f, 0.5f), CVector(0.0f, 0.0f, 1.0f)},// D3DCOLOR_RGBA(255,255,255,255)},
  {CVector( 0.5f,-0.5f, 0.5f), CVector(0.0f, 0.0f, 1.0f)},// D3DCOLOR_RGBA(255,0,255,255)},

  {CVector(-0.5f, 0.5f, 0.5f), CVector(0.0f, 1.0f, 0.0f)},// D3DCOLOR_RGBA(0,255,255,255)},
  {CVector( 0.5f, 0.5f, 0.5f), CVector(0.0f, 1.0f, 0.0f)},// D3DCOLOR_RGBA(255,255,255,255)},
  {CVector(-0.5f, 0.5f,-0.5f), CVector(0.0f, 1.0f, 0.0f)},// D3DCOLOR_RGBA(0,255,0,255)},
  {CVector( 0.5f, 0.5f,-0.5f), CVector(0.0f, 1.0f, 0.0f)},// D3DCOLOR_RGBA(255,255,0,255)},

  {CVector(-0.5f,-0.5f, 0.5f), CVector(0.0f,-1.0f, 0.0f)},// D3DCOLOR_RGBA(0,0,255,255)},
  {CVector(-0.5f,-0.5f,-0.5f), CVector(0.0f,-1.0f, 0.0f)},// D3DCOLOR_RGBA(0,0,0,255)},
  {CVector( 0.5f,-0.5f, 0.5f), CVector(0.0f,-1.0f, 0.0f)},// D3DCOLOR_RGBA(255,0,255,255)},
  {CVector( 0.5f,-0.5f,-0.5f), CVector(0.0f,-1.0f, 0.0f)},// D3DCOLOR_RGBA(255,0,0,255)},

  {CVector( 0.5f, 0.5f,-0.5f), CVector(1.0f, 0.0f, 0.0f)},// D3DCOLOR_RGBA(255,255,0,255)},
  {CVector( 0.5f, 0.5f, 0.5f), CVector(1.0f, 0.0f, 0.0f)},// D3DCOLOR_RGBA(255,255,255,255)},
  {CVector( 0.5f,-0.5f,-0.5f), CVector(1.0f, 0.0f, 0.0f)},// D3DCOLOR_RGBA(255,0,0,255)},
  {CVector( 0.5f,-0.5f, 0.5f), CVector(1.0f, 0.0f, 0.0f)},// D3DCOLOR_RGBA(255,0,255,255)},

  {CVector(-0.5f, 0.5f,-0.5f), CVector(-1.0f, 0.0f, 0.0f)},// D3DCOLOR_RGBA(0,255,0,255)},
  {CVector(-0.5f,-0.5f,-0.5f), CVector(-1.0f, 0.0f, 0.0f)},// D3DCOLOR_RGBA(0,0,0,255)},
  {CVector(-0.5f, 0.5f, 0.5f), CVector(-1.0f, 0.0f, 0.0f)},// D3DCOLOR_RGBA(0,255,255,255)},
  {CVector(-0.5f,-0.5f, 0.5f), CVector(-1.0f, 0.0f, 0.0f)},// D3DCOLOR_RGBA(0,0,255,255)}
};

FracVertex g_pyramidVertices[] =
{
  {CVector( 0.5f, 0.5f, 0.5f), CVector( 0.5f, 0.5f, 0.5f)}, //0xFFFF0000},
  {CVector( 0.5f,-0.5f,-0.5f), CVector( 0.5f,-0.5f,-0.5f)}, //0xFF00FF00},
  {CVector(-0.5f, 0.5f,-0.5f), CVector(-0.5f, 0.5f,-0.5f)}, //0xFF0000FF},
  {CVector(-0.5f,-0.5f, 0.5f), CVector(-0.5f,-0.5f, 0.5f)}, //0xFFFFFF00},
  {CVector( 0.5f, 0.5f, 0.5f), CVector( 0.5f, 0.5f, 0.5f)}, //0xFFFF0000},
  {CVector( 0.5f,-0.5f,-0.5f), CVector( 0.5f,-0.5f,-0.5f)}//, 0xFF00FF00}
};

Fractal::Fractal()
{
  Init(4);
}

Fractal::Fractal(int numTransforms)
{
  Init(numTransforms);
}

Fractal::~Fractal()
{
  delete mySphere;
}


void FractalTransform::Init()
{
  translation.x = translation.y = translation.z = 0.0;
  scaling.x = scaling.y = scaling.z = 1.0;
  rotation.x = rotation.y = rotation.z = 0.0;
}

void FractalData::Init()
{
  numTransforms = 1;
  for (int i = 0; i < MAX_TRANSFORMS; i++)
  {
    transforms[i].Init();
  }
  base = FRACTAL_BASE_PYRAMID;
}

//Inits the Transforms
void Fractal::Init(int numTransforms)
{
  myData.Init();

  myData.numTransforms = numTransforms;
  myCutoffDepth = 5;
  selectionOn = false;
  mySelectedTransform = 0;
  myColorLerpAmount = 0.7;
  redBlueRender = false;
  mySphere = new C_Sphere();
}

//Renders the fractal
void Fractal::Render()
{
  for(int i = 0; i < myData.numTransforms; i++)
    myData.transforms[i].color.incrementColor();

  for(int i = 0; i < myData.numTransforms; i++)
  {
    ApplyTransform(i);
    RenderChild(0, i, myData.transforms[i].color.getColor());
    InvertTransform(i);
  }
}

//The Recursively called render function
void Fractal::RenderChild(int depth, int parentTransform, const CRGBA& childColor)
{
  depth++;

  if(depth >= myCutoffDepth)
  {
    RenderBase(childColor);
    return;
  }

  for(int i = 0; i < myData.numTransforms; i++)
  {
    ApplyTransform(i);
    RenderChild(depth, parentTransform, CRGBA::Lerp(myData.transforms[i].color.getColor(), childColor, myColorLerpAmount));
    InvertTransform(i);
  }
}

//applys the transform to the matrix
void Fractal::ApplyTransform(int i)
{
  glScalef(myData.transforms[i].scaling.x, myData.transforms[i].scaling.y, myData.transforms[i].scaling.z);
  glTranslatef(myData.transforms[i].translation.x, myData.transforms[i].translation.y, myData.transforms[i].translation.z);
  glRotatef(myData.transforms[i].rotation.x, 1.0, 0.0, 0.0);
  glRotatef(myData.transforms[i].rotation.y, 0.0, 1.0, 0.0);
  glRotatef(myData.transforms[i].rotation.z, 0.0, 0.0, 1.0);
}

//Inverts the transform.. just pops the matrix off the stack.
void Fractal::InvertTransform(int iTransform)
{
  glPopMatrix();
}

//Sets whether selection is on and which transform is selected
void Fractal::SetSelection(bool drawSelection, int numSelected)
{
  selectionOn = drawSelection;
  mySelectedTransform = numSelected % myData.numTransforms;
}

FractalData *Fractal::GetDataHandle()
{
  return &myData;
}

//Increments cutoff
void Fractal::IncrementCutoff(bool up)
{
  myCutoffDepth += up ? 1 : -1;
  if (myCutoffDepth == 0)
    myCutoffDepth = 1;
}

void Fractal::SetCutoff(int cutoff)
{
  myCutoffDepth = cutoff;
  myColorLerpAmount = pow(0.4f,1.0f/((float)cutoff));
}

void Fractal::setRedBlueRender(bool red)
{
  redBlueRender = red;
}

// draws the base shape be it cube, pyramid or whatever
void Fractal::RenderBase(const CRGBA& color)
{
  const GLfloat dif[] = {color.r, color.g, color.b, color.a};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
  if(myData.base == FRACTAL_BASE_SPHERE)
  {
    mySphere->Render3D();
  }
  else if(myData.base == FRACTAL_BASE_PYRAMID)
  {
    glBegin(GL_TRIANGLE_STRIP);
    for (size_t i=0;i<6;++i)
    {
      glColor3f(g_pyramidVertices[i].color.r/255.0,
                g_pyramidVertices[i].color.g/255.0,
                g_pyramidVertices[i].color.b/255.0);
      glVertex3f(g_pyramidVertices[i].position.x,
                 g_pyramidVertices[i].position.y,
                 g_pyramidVertices[i].position.z);
    }
    glEnd();
  }
  else
  {
    glBegin(GL_TRIANGLE_STRIP);
    for (size_t i=0;i<24;++i)
    {
      glColor3f(g_cubeVertices[i].color.r/255.0,
                g_cubeVertices[i].color.g/255.0,
                g_cubeVertices[i].color.b/255.0);
      glVertex3f(g_cubeVertices[i].position.x,
                 g_cubeVertices[i].position.y,
                 g_cubeVertices[i].position.z);
    }
    glEnd();
  }
}
