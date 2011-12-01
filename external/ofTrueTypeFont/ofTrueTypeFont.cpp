#include "ofTrueTypeFont.h"
#include "renderer.h"
//--------------------------

	#include <ft2build.h>
	#include <freetype/freetype.h>
	#include <freetype/ftglyph.h>
	#include <freetype/ftoutln.h>
	#include <freetype/fttrigon.h>


ofTrueTypeFont::ofTrueTypeFont(){
	bLoadedOk		= false;
	bMakeContours	= false;
	renderer=Renderer::getInstance();
}

//------------------------------------------------------------------
ofTrueTypeFont::~ofTrueTypeFont(){

	if (bLoadedOk){

		if (cps != NULL){
			delete[] cps;
		}

		if (texNames != NULL){
			for (int i = 0; i < nCharacters; i++){
				glDeleteTextures(1, &texNames[i]);
			}
			delete[] texNames;
		}
	}
}

//------------------------------------------------------------------
void ofTrueTypeFont::loadFont(string filename, int fontsize){
	// load anti-aliased, non-full character set:
	loadFont(filename, fontsize, true, false, false);
}

//------------------------------------------------------------------
void ofTrueTypeFont::loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours){

	bMakeContours = makeContours;

	//------------------------------------------------
	if (bLoadedOk == true){

		// we've already been loaded, try to clean up :

		if (cps != NULL){
			delete[] cps;
		}
		if (texNames != NULL){
			for (int i = 0; i < nCharacters; i++){
				glDeleteTextures(1, &texNames[i]);
			}
			delete [] texNames;
		}
		bLoadedOk = false;
	}
	//------------------------------------------------


	filename = filename;

	bLoadedOk 			= false;
	bAntiAlised 		= _bAntiAliased;
	bFullCharacterSet 	= _bFullCharacterSet;
	fontSize			= fontsize;

	//--------------- load the library and typeface
	FT_Library library;
	if (FT_Init_FreeType( &library )){
		cout << " PROBLEM WITH FT lib" << endl;
		return;
	}

	FT_Face face;
	if (FT_New_Face( library, filename.c_str(), 0, &face )) {
		return;
	}

	FT_Set_Char_Size( face, fontsize << 6, fontsize << 6, 96, 96);
	lineHeight = fontsize * 1.43f;

	//------------------------------------------------------
	//kerning would be great to support:
	//ofLog(OF_LOG_NOTICE,"FT_HAS_KERNING ? %i", FT_HAS_KERNING(face));
	//------------------------------------------------------

	nCharacters = bFullCharacterSet ? 256 : 128 - NUM_CHARACTER_TO_START;

	//--------------- initialize character info and textures
	cps       = new charProps[nCharacters];
	texNames  = new GLuint[nCharacters];
	glGenTextures(nCharacters, texNames);

	//--------------------- load each char -----------------------
	for (int i = 0 ; i < nCharacters; i++){

		//------------------------------------------ anti aliased or not:
		if(FT_Load_Glyph( face, FT_Get_Char_Index( face, (unsigned char)(i+NUM_CHARACTER_TO_START) ), FT_LOAD_DEFAULT )){
			cout << "error with FT_Load_Glyph" << endl;
		}

		if (bAntiAlised == true) FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		else FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);

		//------------------------------------------
		FT_Bitmap& bitmap= face->glyph->bitmap;

		// 3 pixel border around the glyph
		// We show 2 pixels of this, so that blending looks good.
		// 1 pixels is hidden because we don't want to see the real edge of the texture

		border			= 3;
		visibleBorder	= 2;


		// prepare the texture:
		int width  = ofNextPow2( bitmap.width + border*2 );
		int height = ofNextPow2( bitmap.rows  + border*2 );


		// ------------------------- this is fixing a bug with small type
		// ------------------------- appearantly, opengl has trouble with
		// ------------------------- width or height textures of 1, so we
		// ------------------------- we just set it to 2...
		if (width == 1) width = 2;
		if (height == 1) height = 2;

		// -------------------------
		// info about the character:
		cps[i].value 			= i;
		cps[i].height 			= face->glyph->bitmap_top;
		cps[i].width 			= face->glyph->bitmap.width;
		cps[i].setWidth 		= face->glyph->advance.x >> 6;
		cps[i].topExtent 		= face->glyph->bitmap.rows;
		cps[i].leftExtent		= face->glyph->bitmap_left;

		// texture internals
		cps[i].tTex             = (float)(bitmap.width + visibleBorder*2)  /  (float)width;
		cps[i].vTex             = (float)(bitmap.rows +  visibleBorder*2)   /  (float)height;

		cps[i].xOff             = (float)(border - visibleBorder) / (float)width;
		cps[i].yOff             = (float)(border - visibleBorder) / (float)height;


		/* sanity check:
		ofLog(OF_LOG_NOTICE,"%i %i %i %i %i %i",
		cps[i].value ,
		cps[i].height ,
		cps[i].width 	,
		cps[i].setWidth 	,
		cps[i].topExtent ,
		cps[i].leftExtent	);
		*/


		// Allocate Memory For The Texture Data.
		unsigned char* expanded_data = new unsigned char[ 2 * width * height];

		//-------------------------------- clear data:
		for(int j=0; j <height;j++) {
			for(int k=0; k < width; k++){
				expanded_data[2*(k+j*width)  ] = 255;   // every luminance pixel = 255
				expanded_data[2*(k+j*width)+1] = 0;
			}
		}


		if (bAntiAlised == true){
			//-----------------------------------
			for(int j=0; j <height; j++) {
				for(int k=0; k < width; k++){
					if ((k<bitmap.width) && (j<bitmap.rows)){
						expanded_data[2*((k+border)+(j+border)*width)+1] = bitmap.buffer[k + bitmap.width*(j)];
					}
				}
			}
			//-----------------------------------
		} else {
			//-----------------------------------
			// true type packs monochrome info in a
			// 1-bit format, hella funky
			// here we unpack it:
			unsigned char *src =  bitmap.buffer;
			for(int j=0; j <bitmap.rows;j++) {
				unsigned char b=0;
				unsigned char *bptr =  src;
				for(int k=0; k < bitmap.width ; k++){
					expanded_data[2*((k+1)+(j+1)*width)] = 255;
					if (k%8==0){ b = (*bptr++);}
					expanded_data[2*((k+1)+(j+1)*width) + 1] =
						b&0x80 ? 255 : 0;
				    b <<= 1;
				}
				src += bitmap.pitch;
			}
			//-----------------------------------
		}


		//Now we just setup some texture paramaters.
		glBindTexture( GL_TEXTURE_2D, texNames[i]);
		#ifndef TARGET_OF_IPHONE
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		#endif
   		if (bAntiAlised == true){
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		}
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//Here we actually create the texture itself, notice
		//that we are using GL_LUMINANCE_ALPHA to indicate that
		//we are using 2 channel data.

			bool b_use_mipmaps = false;  // FOR now this is fixed to false, could be an option, left in for legacy...
			if (b_use_mipmaps){
				gluBuild2DMipmaps(
					GL_TEXTURE_2D, GL_LUMINANCE_ALPHA, width, height,
					GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
			} else
		{
	    	glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height,
			   0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
		}


		//With the texture created, we don't need to expanded data anymore

    	delete [] expanded_data;

   }

	// ------------- close the library and typeface
	FT_Done_Face(face);
	FT_Done_FreeType(library);
  	bLoadedOk = true;
}

//-----------------------------------------------------------
int ofTrueTypeFont::ofNextPow2 ( int a ){
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}


//-----------------------------------------------------------
void ofTrueTypeFont::setLineHeight(float _newLineHeight) {
	lineHeight = _newLineHeight;
}

//-----------------------------------------------------------
float ofTrueTypeFont::getLineHeight(){
	return lineHeight;
}


//-----------------------------------------------------------
void ofTrueTypeFont::drawChar(int c, float x, float y) {


	//----------------------- error checking
	if (!bLoadedOk){
		cout << "Error : font not allocated -- line "<< endl;
		return;
	}

	if (c >= nCharacters){
		//ofLog(OF_LOG_ERROR,"Error : char (%i) not allocated -- line %d in %s", (c + NUM_CHARACTER_TO_START), __LINE__,__FILE__);
		return;
	}
	//-----------------------

	int cu = c;

 	GLint height	= cps[cu].height;
	GLint bwidth	= cps[cu].width;
	GLint top		= cps[cu].topExtent - cps[cu].height;
	GLint lextent	= cps[cu].leftExtent;

	GLfloat	x1, y1, x2, y2, corr, stretch;
	GLfloat t1, v1, t2, v2;

	//this accounts for the fact that we are showing 2*visibleBorder extra pixels
	//so we make the size of each char that many pixels bigger
	stretch = (float)(visibleBorder * 2);


	t2		= cps[cu].xOff;
	v2		= cps[cu].yOff;
	t1		= cps[cu].tTex + t2;
	v1		= cps[cu].vTex + v2;

	corr	= (float)(( (fontSize - height) + top) - fontSize);

	x1		= lextent + bwidth + stretch;
	y1		= height + corr + stretch;
	x2		= (float) lextent;
	y2		= -top + corr;



	if (glIsTexture(texNames[cu])) {
	    glBindTexture(GL_TEXTURE_2D, texNames[cu]);
		glNormal3f(0, 0, 1);

		GLfloat verts[] = { x2,y2,
			x2, y1,
			x1, y1,
		x1, y2 };
		GLfloat tex_coords[] = { t2, v2,
			t2, v1,
			t1, v1,
		t1, v2 };



        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
		glVertexPointer(2, GL_FLOAT, 0, verts );

		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );

        glBindTexture(GL_TEXTURE_2D,0);

        glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
}


//=====================================================================
void ofTrueTypeFont::drawString(string c, float x, float y) {

    if (!bLoadedOk){
    	cout << "Error : font not allocated -- " << endl;
    	return;
    };

	GLint		index	= 0;
	GLfloat		X		= 0;
	GLfloat		Y		= 0;


	// (c) enable texture once before we start drawing each char (no point turning it on and off constantly)
	// (d) store the current matrix position and do a translation to the drawing position
	glPushMatrix();
	glTranslatef(x, y, 0);



	int len = (int)c.length();

	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		//TODO: fix this! NULL characters within tokenized strings to draw...
		if (cy<-32){
            cout << "found NULL character" << cy <<" "<< len << " " << index << " " << c << endl;
            return;
		}

		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {

				Y = (float) lineHeight;
				glTranslatef(-X, Y, 0);
				X = 0 ; //reset X Pos back to zero

		  }else if (c[index] == ' ') {
				 int cy = (int)'p' - NUM_CHARACTER_TO_START;
				 X += cps[cy].width;
				 glTranslatef((float)cps[cy].width, 0, 0);
		  } else {
				drawChar(cy, 0, 0);
				X += cps[cy].setWidth;
				glTranslatef((float)cps[cy].setWidth, 0, 0);
		  }
		}
		index++;
	}
	glPopMatrix();

}


