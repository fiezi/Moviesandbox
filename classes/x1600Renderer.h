/*
 *  x1600Renderer.h
 *  MSB_mac
 *
 *  Created by Friedrich Kirschner on 7/18/10.
 *  Copyright 2010 Friedrich Kirschner. All rights reserved.
 *
 */

#ifndef H_X1600RENDERER
#define H_X1600RENDERER

#include "renderer.h"

class X1600Renderer: public Renderer{

public:

	X1600Renderer();
	virtual ~X1600Renderer();

	virtual void drawSceneTexture();
	virtual void drawColor3D(Layer* currentLayer);
	virtual void drawData3D(Layer* currentLayer);
	static Renderer* getInstance();



};
#endif
