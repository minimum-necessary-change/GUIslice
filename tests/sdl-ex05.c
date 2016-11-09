//
// microSDL GUI Library Examples
// - Calvin Hass
// - http://www.impulseadventure.com/elec/microsdl-sdl-gui.html
// - Example 05: Multiple page handling
//               Compound elements
//

#include "microsdl.h"
#include "microsdl_ex.h"

#include <libgen.h>       // For path parsing

// Defines for resources
#define FONT_DROID_SANS "/usr/share/fonts/truetype/droid/DroidSans.ttf"
#define IMG_BKGND       "/res/bkgnd1_320x240.bmp"

// Enumerations for pages, elements, fonts, images
enum {E_PG_MAIN,E_PG_EXTRA};
enum {E_ELEM_BTN_QUIT,E_ELEM_BTN_EXTRA,E_ELEM_BTN_BACK,
      E_ELEM_TXT_COUNT,E_ELEM_PROGRESS,
      E_ELEM_COMP1,E_ELEM_COMP2,E_ELEM_COMP3};
enum {E_FONT_BTN,E_FONT_TXT,E_FONT_TITLE};

bool     m_bQuit = false;

// Free-running counter for display
unsigned m_nCount = 0;

// Instantiate the GUI
#define MAX_ELEM    30
#define MAX_FONT    10
microSDL_tsGui      m_gui;
microSDL_tsElem     m_asElem[MAX_ELEM];
microSDL_tsFont     m_asFont[MAX_FONT];
microSDL_tsXGauge   m_sXGauge;
microSDL_tsXSelNum  m_sXSelNum[3];

// Button callbacks
// - Show example of common callback function
bool CbBtnCommon(void* pvGui,void *pvElem,microSDL_teTouch eTouch,int nX,int nY)
{
  microSDL_tsElem* pElem = (microSDL_tsElem*)(pvElem);
  int nElemId = pElem->nId;
  if (eTouch == MSDL_TOUCH_UP_IN) {
    if (nElemId == E_ELEM_BTN_QUIT) {
      m_bQuit = true;
    } else if (nElemId == E_ELEM_BTN_EXTRA) {
      microSDL_SetPageCur(&m_gui,E_PG_EXTRA);
    } else if (nElemId == E_ELEM_BTN_BACK) {
      microSDL_SetPageCur(&m_gui,E_PG_MAIN);
    }
  }
  return true;
}


// Create the default elements on each page
// - strPath: Path to executable passed in to locate resource files
bool InitOverlays(char *strPath)
{
  microSDL_tsElem*  pElem = NULL;

  // -----------------------------------
  // Background
  char* strImgBkgndPath = (char*)malloc(strlen(strPath)+strlen(IMG_BKGND)+1);
  strcpy(strImgBkgndPath, strPath);
  strcat(strImgBkgndPath, IMG_BKGND);
  microSDL_SetBkgndImage(&m_gui,strImgBkgndPath);
  free(strImgBkgndPath);
  
  // -----------------------------------
  // PAGE: MAIN

  // Create background box
  pElem = microSDL_ElemCreateBox(&m_gui,MSDL_ID_AUTO,E_PG_MAIN,(SDL_Rect){20,50,280,150});
  microSDL_ElemSetCol(pElem,MSDL_COL_WHITE,MSDL_COL_BLACK,MSDL_COL_BLACK);

  // Create title
  pElem = microSDL_ElemCreateTxt(&m_gui,MSDL_ID_AUTO,E_PG_MAIN,(SDL_Rect){10,10,310,40},
    "microSDL Demo",E_FONT_TITLE);
  microSDL_ElemSetTxtAlign(pElem,MSDL_ALIGN_MID_MID);
  microSDL_ElemSetFillEn(pElem,false);
  microSDL_ElemSetTxtCol(pElem,MSDL_COL_WHITE);

  // Create Quit button with text label
  pElem = microSDL_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_QUIT,E_PG_MAIN,
    (SDL_Rect){100,140,50,20},"Quit",E_FONT_BTN,&CbBtnCommon);

  // Create Extra button with text label
  pElem = microSDL_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_EXTRA,E_PG_MAIN,
    (SDL_Rect){170,140,50,20},"Extra",E_FONT_BTN,&CbBtnCommon);

  // Create counter
  pElem = microSDL_ElemCreateTxt(&m_gui,MSDL_ID_AUTO,E_PG_MAIN,(SDL_Rect){40,60,50,10},
    "Count:",E_FONT_TXT);
  pElem = microSDL_ElemCreateTxt(&m_gui,E_ELEM_TXT_COUNT,E_PG_MAIN,(SDL_Rect){100,60,50,10},
    "",E_FONT_TXT);
  microSDL_ElemSetTxtCol(pElem,MSDL_COL_YELLOW);

  // Create progress bar
  pElem = microSDL_ElemCreateTxt(&m_gui,MSDL_ID_AUTO,E_PG_MAIN,(SDL_Rect){40,80,50,10},
    "Progress:",E_FONT_TXT);
  pElem = microSDL_ElemXGaugeCreate(&m_gui,E_ELEM_PROGRESS,E_PG_MAIN,&m_sXGauge,(SDL_Rect){100,80,50,10},
    0,100,0,MSDL_COL_GREEN_DK,false);

  // Add compound element
  pElem = microSDL_ElemXSelNumCreate(&m_gui,E_ELEM_COMP1,E_PG_MAIN,&m_sXSelNum[0],
    (SDL_Rect){160,60,120,50},E_FONT_BTN);  
  
  // -----------------------------------
  // PAGE: EXTRA

  // Create background box
  pElem = microSDL_ElemCreateBox(&m_gui,MSDL_ID_AUTO,E_PG_EXTRA,(SDL_Rect){40,40,240,160});
  microSDL_ElemSetCol(pElem,MSDL_COL_WHITE,MSDL_COL_BLACK,MSDL_COL_BLACK);

  // Create Back button with text label
  pElem = microSDL_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_BACK,E_PG_EXTRA,
    (SDL_Rect){50,170,50,20},"Back",E_FONT_BTN,&CbBtnCommon);

  // Create a few labels
  Sint16    nPosY = 50;
  Sint16    nSpaceY = 20;
  pElem = microSDL_ElemCreateTxt(&m_gui,MSDL_ID_AUTO,E_PG_EXTRA,(SDL_Rect){60,nPosY,50,10},
    "Data 1",E_FONT_TXT); nPosY += nSpaceY;
  pElem = microSDL_ElemCreateTxt(&m_gui,MSDL_ID_AUTO,E_PG_EXTRA,(SDL_Rect){60,nPosY,50,10},
    "Data 2",E_FONT_TXT); nPosY += nSpaceY;
  pElem = microSDL_ElemCreateTxt(&m_gui,MSDL_ID_AUTO,E_PG_EXTRA,(SDL_Rect){60,nPosY,50,10},
    "Data 3",E_FONT_TXT); nPosY += nSpaceY;
  
  // Add compound element
  pElem = microSDL_ElemXSelNumCreate(&m_gui,E_ELEM_COMP2,E_PG_EXTRA,&m_sXSelNum[1],
    (SDL_Rect){130,60,120,50},E_FONT_BTN);

  pElem = microSDL_ElemXSelNumCreate(&m_gui,E_ELEM_COMP3,E_PG_EXTRA,&m_sXSelNum[2],
    (SDL_Rect){130,120,120,50},E_FONT_BTN);    
    
  return true;
}


int main( int argc, char* args[] )
{
  bool              bOk = true;
  char              acTxt[100];

  // -----------------------------------
  // Initialize

  microSDL_InitEnv(&m_gui);
  if (!microSDL_Init(&m_gui,m_asElem,MAX_ELEM,m_asFont,MAX_FONT,NULL,0)) { exit(1); }

  microSDL_InitTs(&m_gui,"/dev/input/touchscreen");

  // Load Fonts
  // - In this example, we are loading the same font but at
  //   different point sizes. We could also refer to other
  //   font files as well.
  bOk = microSDL_FontAdd(&m_gui,E_FONT_BTN,FONT_DROID_SANS,12);
  if (!bOk) { fprintf(stderr,"ERROR: FontAdd failed\n"); exit(1); }
  bOk = microSDL_FontAdd(&m_gui,E_FONT_TXT,FONT_DROID_SANS,10);
  if (!bOk) { fprintf(stderr,"ERROR: FontAdd failed\n"); exit(1); }
  bOk = microSDL_FontAdd(&m_gui,E_FONT_TITLE,FONT_DROID_SANS,32);
  if (!bOk) { fprintf(stderr,"ERROR: FontAdd failed\n"); exit(1); }


  // -----------------------------------
  // Create page elements
  // -----------------------------------
  InitOverlays(dirname(args[0])); // Pass executable path to find resource files


  // -----------------------------------
  // Start display

  // Start up display on main page
  microSDL_SetPageCur(&m_gui,E_PG_MAIN);
  
  // -----------------------------------
  // Main event loop

  m_bQuit = false;
  while (!m_bQuit) {

    m_nCount++;

    // -----------------------------------
    // Perform drawing updates
    // - Note: we can make the updates conditional on the active
    //   page by checking microSDL_GetPageCur() first.

    sprintf(acTxt,"%u",m_nCount);
    microSDL_ElemSetTxtStr(microSDL_ElemGet(&m_gui,E_ELEM_TXT_COUNT),acTxt);

    microSDL_ElemXGaugeUpdate(microSDL_ElemGet(&m_gui,E_ELEM_PROGRESS),((m_nCount/200)%100));

    // Periodically call microSDL update function    
    microSDL_Update(&m_gui);
    
  } // bQuit


  // -----------------------------------
  // Close down display

  microSDL_Quit(&m_gui);

  return 0;
}

