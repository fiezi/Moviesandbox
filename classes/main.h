#ifndef _MAIN_H_
#define _MAIN_H_

#include "includes.h"
#include "input.h"
#include "base64Encode.h"


#include <sstream>
#include <fstream>
#include <ostream>
#include <ctime>
#include <strstream>


/*

Variables

*********************************/

const int windowX=640;
const int windowY=480;

Renderer*    renderManager;
Input*       inputManager;
SceneData*   sceneDataManager;
int          splashWindow;
/*

Functions

*********************************/

void draw(void);


#endif  //_MAIN_H_
