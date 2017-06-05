/*
 * CrystalMorph Screensaver for XBox Media Center
 * Copyright (c) 2004 Team XBMC
 *
 * Ver 1.0 2007-02-12 by Asteron  http://asteron.projects.googlepages.com/home
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2  of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#include <kodi/addon-instance/Screensaver.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Fractal.h"
#include "fractalcontroller.h"
#include "Fractal.h"

// stuff for the background plane

struct BG_VERTEX
{
    CVector position;
    CRGBA color;
};

////////////////////////////////////////////////////////////////////////////////

class CScreensaverCrystalmorph
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceScreensaver
{
  public:
    CScreensaverCrystalmorph();

    virtual bool Start() override;
    virtual void Stop() override;
    virtual void Render() override;

  private:
    FractalSettings m_settings;

    int             m_iWidth;
    int             m_iHeight;
    float           m_fRatio;

    CVector         m_lightDir = CVector(-0.5f, -0.5f, 0.5f);
    BG_VERTEX       m_BGVertices[4];

    void            Step();
    void            CreateLight();
    void            SetupGradientBackground(const CRGBA& dwTopColor, const CRGBA& dwBottomColor);
    void            RenderGradientBackground();
    void            SetCamera();
    void            SetDefaults();
};

void CScreensaverCrystalmorph::Step()
{
  if (m_settings.frame > m_settings.nextTransition)
  {
    m_settings.nextTransition = m_settings.frame + m_settings.transitionTime;

    if (m_settings.animationCountdown == 0)
    {
      m_settings.fractalcontroller->SetAnimation(true);
      m_settings.animationCountdown = 3+rand()%5;
      m_settings.nextTransition = m_settings.frame + m_settings.animationTime;
    }
    else
    {
      m_settings.fractalcontroller->SetAnimation(false);

      if (m_settings.animationCountdown-- == 1)
      {
        m_settings.fractalcontroller->SetToRandomFractal(3+rand()%3);
      }
      else
      {
        static int preset = rand()%6;
        if (frand() < m_settings.presetChance)
          m_settings.fractalcontroller->SetDesiredToPreset((preset+rand()%5)%6);
        else
        {
          if (rand()%70 == 0) m_settings.fractalcontroller->SetToRandomFractal(8);
          else m_settings.fractalcontroller->SetToRandomFractal();
        }
      }
      m_settings.fractalcontroller->StartMorph();
    }
  }
  m_settings.fractalcontroller->UpdateFractalData();
  m_settings.frame++;
}

void CScreensaverCrystalmorph::CreateLight()
{
  const GLfloat amb[] = {0.5, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  const GLfloat dif[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
  const GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
  const GLfloat pos[] = {5.0, -5.0, 10.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  const GLfloat dir[] = {m_lightDir.x, m_lightDir.y, m_lightDir.z, 1.0};
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
  float at = 0.5;
  glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, &at);
  at = 0.02;
  glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, &at);
  at = 0.0;
  glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, &at);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

}

////////////////////////////////////////////////////////////////////////////////

// fill in background vertex array with values that will
// completely cover screen
void CScreensaverCrystalmorph::SetupGradientBackground(const CRGBA& dwTopColor, const CRGBA& dwBottomColor)
{
  float x1 = -0.5f;
  float y1 = -0.5f;
  float x2 = (float)m_iWidth - 0.5f;
  float y2 = (float)m_iHeight - 0.5f;

  m_BGVertices[0].position = CVector( x2, y1, 0.0f);
  m_BGVertices[0].color = dwTopColor;

  m_BGVertices[1].position = CVector( x2, y2, 0.0f);
  m_BGVertices[1].color = dwBottomColor;

  m_BGVertices[2].position = CVector( x1, y1, 0.0f);
  m_BGVertices[2].color = dwTopColor;

  m_BGVertices[3].position = CVector( x1, y2, 0.0f);
  m_BGVertices[3].color = dwBottomColor;
}

///////////////////////////////////////////////////////////////////////////////


void CScreensaverCrystalmorph::RenderGradientBackground()
{
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_TRIANGLE_STRIP);
  for (size_t i=0;i<4;++i)
  {
    glColor3f(m_BGVertices[i].color.r, m_BGVertices[i].color.g,
              m_BGVertices[i].color.b);
    glVertex3f(m_BGVertices[i].position.x, m_BGVertices[i].position.y,
               m_BGVertices[i].position.z);
  }
  glEnd();
}

void CScreensaverCrystalmorph::SetCamera()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, m_fRatio, 0.1, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void CScreensaverCrystalmorph::SetDefaults()
{
  m_settings.frame = 0;
  m_settings.iMaxDepth = 5;
  m_settings.iMaxObjects = 4000;
  m_settings.nextTransition = 500;
  m_settings.morphSpeed = 3.0;
  m_settings.presetChance = 0.3;
  m_settings.animationTime = 3000;
  m_settings.transitionTime = 500;

  m_settings.animationCountdown = 3+rand()%5;
}

////////////////////////////////////////////////////////////////////////////
// XBMC has loaded us into memory, we should set our core values
// here and load any settings we may have from our config file
//
CScreensaverCrystalmorph::CScreensaverCrystalmorph()
{
  m_iWidth = Width();
  m_iHeight = Height();
  m_fRatio = (float)m_iWidth/(float)m_iHeight;

  SetCamera();
  SetDefaults();
  m_settings.fractal = new Fractal();
  m_settings.fractalcontroller = new FractalController(m_settings.fractal, &m_settings);
  m_settings.fractalcontroller->SetMorphSpeed(m_settings.morphSpeed);
  CreateLight();
}

// XBMC tells us we should get ready
// to start rendering. This function
// is called once when the screensaver
// is activated by XBMC.
bool CScreensaverCrystalmorph::Start()
{
  SetupGradientBackground(CRGBA(0,0,0,255), CRGBA(0,60,60,255));
}

// XBMC tells us to render a frame of
// our screensaver. This is called on
// each frame render in XBMC, you should
// render a single frame only - the DX
// device will already have been cleared.
void CScreensaverCrystalmorph::Render()
{
  CreateLight();
  SetCamera();
  RenderGradientBackground();
  static int sx = (rand()%2)*2-1, sy = (rand()%2)*2-1;

  glRotatef(sx*m_settings.frame*0.0051,1.0, 0.0, 0.0);
  glRotatef(sy*m_settings.frame*0.0032,0.0, 1.0, 0.0);
  Step();
  m_settings.fractal->Render();
}

// XBMC tells us to stop the screensaver
// we should free any memory and release
// any resources we have created.
void CScreensaverCrystalmorph::Stop()
{
  delete m_settings.fractal;
  delete m_settings.fractalcontroller;
}

ADDONCREATOR(CScreensaverCrystalmorph);
