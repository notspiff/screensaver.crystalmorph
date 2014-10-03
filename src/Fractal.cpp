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


#include <xbmc/xbmc_scr_dll.h>
#include <GL/gl.h>
#include "Fractal.h"
#include "fractalcontroller.h"
#include "Fractal.h"

FractalSettings settings;

int	g_iWidth;
int g_iHeight;
float g_fRatio;

void Step()
{
  if (settings.frame > settings.nextTransition)
  {
    settings.nextTransition = settings.frame + settings.transitionTime;

    if (settings.animationCountdown == 0)
    {
      settings.fractalcontroller->SetAnimation(true);
      settings.animationCountdown = 3+rand()%5;
      settings.nextTransition = settings.frame + settings.animationTime;
    }
    else
    {
      settings.fractalcontroller->SetAnimation(false);

      if (settings.animationCountdown-- == 1)
      {
        settings.fractalcontroller->SetToRandomFractal(3+rand()%3);
      }
      else
      {
        static int preset = rand()%6;
        if (frand() < settings.presetChance)
          settings.fractalcontroller->SetDesiredToPreset((preset+rand()%5)%6);
        else
        {
          if (rand()%70 == 0) settings.fractalcontroller->SetToRandomFractal(8);
          else settings.fractalcontroller->SetToRandomFractal();
        }
      }
      settings.fractalcontroller->StartMorph();
    }
  }
  settings.fractalcontroller->UpdateFractalData();
  settings.frame++;
}

CVector g_lightDir = CVector(-0.5f, -0.5f, 0.5f);

void CreateLight()
{
  const GLfloat amb[] = {0.5, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  const GLfloat dif[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
  const GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
  const GLfloat pos[] = {5.0, -5.0, 10.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  const GLfloat dir[] = {g_lightDir.x, g_lightDir.y, g_lightDir.z, g_lightDir.w};
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

// stuff for the background plane

struct BG_VERTEX 
{
    CVector position;
    CRGBA color;
};

BG_VERTEX g_BGVertices[4];

////////////////////////////////////////////////////////////////////////////////

// fill in background vertex array with values that will
// completely cover screen
void SetupGradientBackground(const CRGBA& dwTopColor, const CRGBA& dwBottomColor)
{
  float x1 = -0.5f;
  float y1 = -0.5f;
  float x2 = (float)g_iWidth - 0.5f;
  float y2 = (float)g_iHeight - 0.5f;

  g_BGVertices[0].position = CVector( x2, y1, 0.0f, 1.0f );
  g_BGVertices[0].color = dwTopColor;

  g_BGVertices[1].position = CVector( x2, y2, 0.0f, 1.0f );
  g_BGVertices[1].color = dwBottomColor;

  g_BGVertices[2].position = CVector( x1, y1, 0.0f, 1.0f );
  g_BGVertices[2].color = dwTopColor;

  g_BGVertices[3].position = CVector( x1, y2, 0.0f, 1.0f );
  g_BGVertices[3].color = dwBottomColor;
}

///////////////////////////////////////////////////////////////////////////////


void RenderGradientBackground()
{
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_TRIANGLE_STRIP);
  for (size_t i=0;i<4;++i)
  {
    glColor3f(g_BGVertices[i].color.r, g_BGVertices[i].color.g,
              g_BGVertices[i].color.b);
    glVertex3f(g_BGVertices[i].position.x, g_BGVertices[i].position.y,
               g_BGVertices[i].position.z);
  }
  glEnd();
}

void SetCamera()
{
  D3DXMATRIX m_World;
  D3DXMatrixIdentity( &m_World );
	d3dSetTransform(D3DTS_WORLD, &m_World);

	D3DXMATRIX m_View;// = ViewMatrix(D3DXVECTOR(0,-20,20), D3DXVECTOR(0,0,0), D3DXVECTOR(0,-1,0), 0);
	D3DXVECTOR3 from = D3DXVECTOR3(0,0,-2);
	D3DXVECTOR3 to   = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 up   = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtLH(&m_View,&from,&to,&up);
	d3dSetTransform(D3DTS_VIEW, &m_View);

	D3DXMATRIX m_Projection; 
  D3DXMatrixPerspectiveFovLH(&m_Projection, D3DX_PI/4, (float)(g_iWidth)/(float)(g_iHeight)*g_fRatio, 0.1, 1000.0);
	d3dSetTransform(D3DTS_PROJECTION, &m_Projection);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspectRatio = (float)m_iWidth/(float)m_iHeight;
  gluPerspective(45, aspectRatio, 0.1, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void SetDefaults()
{
  settings.frame = 0;
  settings.iMaxDepth = 5;
  settings.iMaxObjects = 4000;
  settings.nextTransition = 500;
  settings.morphSpeed = 3.0;
  settings.presetChance = 0.3;
  settings.animationTime = 3000;
  settings.transitionTime = 500;

  settings.animationCountdown = 3+rand()%5;
}

////////////////////////////////////////////////////////////////////////////
// XBMC has loaded us into memory, we should set our core values
// here and load any settings we may have from our config file
//
ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!props)
    return ADDON_STATUS_UNKNOWN;

  SCR_PROPS* scrprops = (SCR_PROPS*)props;

  g_iWidth = scrprops->width;
  g_iHeight  = scrprops->height;
  g_fRatio = (float)g_iWidth/(float)g_iHeight;

  SetCamera();
  SetDefaults();
  settings.fractal = new Fractal();
  settings.fractalcontroller = new FractalController(settings.fractal, &settings);
  settings.fractalcontroller->SetMorphSpeed(settings.morphSpeed);
  CreateLight();

  return ADDON_STATUS_OK;
}

// XBMC tells us we should get ready
// to start rendering. This function
// is called once when the screensaver
// is activated by XBMC.
extern "C" void Start()
{
  SetupGradientBackground(CRGBA(0,0,0,255), CRGBA(0,60,60,255));
}

// XBMC tells us to render a frame of
// our screensaver. This is called on
// each frame render in XBMC, you should
// render a single frame only - the DX
// device will already have been cleared.
extern "C" void Render()
{
  CreateLight();
  SetCamera();
  RenderSetup();
  RenderGradientBackground();
  static int sx = (rand()%2)*2-1, sy = (rand()%2)*2-1;

  glRotatef(sx*settings.frame*0.0051,sy*settings.frame*0.0032,0);
  Step();
  settings.fractal->Render();
}

// XBMC tells us to stop the screensaver
// we should free any memory and release
// any resources we have created.
extern "C" void ADDON_Stop()
{
  delete settings.fractal;
  delete settings.fractalcontroller;
}

extern "C" void ADDON_Destroy()
{
}

extern "C" ADDON_STATUS ADDON_GetStatus()
{
  return ADDON_STATUS_OK;
}

extern "C" bool ADDON_HasSettings()
{
  return false;
}

extern "C" unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
  return 0;
}

extern "C" ADDON_STATUS ADDON_SetSetting(const char *strSetting, const void *value)
{
  return ADDON_STATUS_OK;
}

extern "C" void ADDON_FreeSettings()
{
}

extern "C" void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data)
{
}

extern "C" void GetInfo(SCR_INFO *info)
{
}

// Load settings from the [screensavername].xml configuration file
// the name of the screensaver (filename) is used as the name of
// the xml file - this is sent to us by XBMC when the Init func
// is called.
//void LoadSettings()
//{
//        XmlNode node, childNode, grandChild;
//        CXmlDocument doc;
//        
         Set up the defaults
//        SetDefaults();

//        char szXMLFile[1024];
//        strcpy(szXMLFile, "Q:\\screensavers\\");
//        strcat(szXMLFile, g_szScrName);
//        strcat(szXMLFile, ".xml");

//        OutputDebugString("Loading XML: ");
//        OutputDebugString(szXMLFile);

         Load the config file
//        if (doc.Load(szXMLFile) >= 0)
//        {
//                node = doc.GetNextNode(XML_ROOT_NODE);
//                while(node > 0)
//                {
//                        if (strcmpi(doc.GetNodeTag(node),"screensaver"))
//                        {
//                                node = doc.GetNextNode(node);
//                                continue;
//                        }
//                        if (childNode = doc.GetChildNode(node,"maxobjects")){
//        settings.iMaxObjects = atoi(doc.GetNodeText(childNode));
//                        }
//      if (childNode = doc.GetChildNode(node,"maxcutoffdepth")){
//        settings.iMaxDepth = atoi(doc.GetNodeText(childNode));
//                        }
//      if (childNode = doc.GetChildNode(node,"morphspeed")){
//        settings.morphSpeed = atof(doc.GetNodeText(childNode));
//                        }
//      if (childNode = doc.GetChildNode(node,"animationtime")){
//        settings.animationTime = 60*atof(doc.GetNodeText(childNode));
//                        }
//      if (childNode = doc.GetChildNode(node,"transitiontime")){
//        settings.nextTransition = settings.transitionTime = 60*atof(doc.GetNodeText(childNode));
//                        }
//      if (childNode = doc.GetChildNode(node,"presetchance")){
//        settings.presetChance = atof(doc.GetNodeText(childNode));
//                        }

//                        node = doc.GetNextNode(node);
//                }
//                doc.Close();
//        }
//}
