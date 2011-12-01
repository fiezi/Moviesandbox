#ifndef _OF_TTF_FONT_H_
#define _OF_TTF_FONT_H_


#include "includes.h"

/*
This class has been slightly modified from OpenFrameworks 0.06 - thanks to Zach and Theo!
for more info on OpenFrameworks go to openframeworks.cc
*/

//--------------------------------------------------
typedef struct {
	int value;
	int height;
	int width;
	int setWidth;
	int topExtent;
	int leftExtent;
	float tTex;
	float vTex;		//0-1 pct of bitmap...
	float xOff;
	float yOff;
} charProps;

//--------------------------------------------------
#define NUM_CHARACTER_TO_START		33		// 0 - 32 are control characters, no graphics needed.

using namespace std;

class Renderer;

class ofTrueTypeFont{

public:


	ofTrueTypeFont();
	virtual ~ofTrueTypeFont();

	// 			-- default, non-full char set, anti aliased:
	void 		loadFont(string filename, int fontsize);
	void 		loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours = false);

	bool		bLoadedOk;
	bool 		bAntiAlised;
	bool 		bFullCharacterSet;

  	float 		getLineHeight();
  	void 		setLineHeight(float height);

	void 		drawString(string s, float x, float y);

	int 		nCharacters;

    Renderer*   renderer;

protected:

	float 			lineHeight;
	charProps 		* 	cps;			// properties for each character
	GLuint			*	texNames;		// textures for each character
	int				fontSize;
	bool			bMakeContours;

	void 			drawChar(int c, float x, float y);

	int 			ofNextPow2(int a);
	int				border, visibleBorder;

};


#endif

