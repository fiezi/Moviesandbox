<<<<<<< HEAD
/*
 *  x1600Renderer.cpp
 *  MSB_mac
 *
 *  Created by Friedrich Kirschner on 7/18/10.
 *  Copyright 2010 Friedrich Kirschner. All rights reserved.
 *
 */


#include "x1600Renderer.h"
#include "input.h"

X1600Renderer::X1600Renderer(){
}

X1600Renderer::~X1600Renderer(){
}


void X1600Renderer::drawSceneTexture(){


    glViewport (0, 0, screenX, screenY);

    glMatrixMode(GL_MODELVIEW);


    for (int i=0;i<(int)sceneData->layerList.size();i++){


		glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, multiSample_fb);

		glDrawBuffers(1, drawBuffers);

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );


        glClear( GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT );

		glEnable(GL_BLEND);

		glActiveTexture(GL_TEXTURE0);

        //drawbuffers are set up here!
        drawColor3D(sceneData->layerList[i]);


        //color blitting

        glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, multiSample_fb );
        glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, sceneData->layerList[i]->colorFBO );
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBlitFramebufferEXT( 0, 0, screenX, screenY, 0, 0, screenX, screenY, GL_COLOR_BUFFER_BIT, texFilter );

		//cleanup
		glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, 0 );
		glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, 0 );
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

        //disable blending for second and third buffer and re-render


		glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, multiSample_fb);

		glDrawBuffers(2, drawBuffers);

        glClearColor( -1.0f, -1.0f, -1.0f, -1.0f );


        glClear( GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT );

		//TODO: ATI BUG
		glDisable(GL_BLEND);

        drawData3D(sceneData->layerList[i]);

        //depth blitting
        glReadBuffer(GL_COLOR_ATTACHMENT1_EXT);

        glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, sceneData->layerList[i]->depthFBO );
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBlitFramebufferEXT( 0, 0, screenX, screenY, 0, 0, screenX, screenY, GL_COLOR_BUFFER_BIT, texFilter );
    }
	glEnable(GL_BLEND);

    //draw brush here?

    //cleanup
    glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, 0 );
    glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, 0 );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

	pick(input->mouseX,input->mouseY);

    //now draw the resulting image into a quad!
}



void X1600Renderer::drawColor3D(Layer* currentLayer){


    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    //draw color

    for (int i=0;i<(int)currentLayer->actorList.size(); i++){
        //update matrices when hidden
        if(currentLayer->actorList[i]->bHidden){
            glPushMatrix();
            transformActorMatrix(currentLayer->actorList[i]);
            glPopMatrix();
        }
        else {
            //draw in all buffers for pickable actors
            if (currentLayer->actorList[i]->bPickable){
                drawActor(currentLayer->actorList[i]);
            }
            //don't draw in picking buffer for non-pickable actors
        }
    }


    //draw non-pickable actors afterwards!
    //used for drawings while drawing, so they're visible
    for (int i=0;i<(int)currentLayer->actorList.size(); i++){
        if (!currentLayer->actorList[i]->bPickable){
            drawActor(currentLayer->actorList[i]);
        }
    }

	//draw helpers - brush, grid, etc... if we're not running
    if (!sceneData->controller->bRunning){

        for (int i=0;i<(int)sceneData->helperList.size();i++){
            if (!sceneData->helperList[i]->bHidden){
                    drawActor(sceneData->helperList[i]);
            }
        }

    }


    //reset texture Matrix transform
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);


    //this for xyz axis
    if (!Control::bRunning){

        //glDepthMask(GL_FALSE);
        setupShading("color");
        for (int i=0;i<(int)currentLayer->actorList.size();i++){
            drawGizmos(currentLayer->actorList[i]);
        }

        //glDepthMask(GL_TRUE);
    }


}



void X1600Renderer::drawData3D(Layer* currentLayer){

    glActiveTexture(GL_TEXTURE0);
    //draw normal, depth, etc...

    for (int i=0;i<(int)currentLayer->actorList.size(); i++){
            //draw in all buffers for pickable actors
            if (currentLayer->actorList[i]->bPickable){
                drawActor(currentLayer->actorList[i]);
            }
    }



	//draw helpers - brush, grid, etc... if we're not running
    if (!sceneData->controller->bRunning){

        for (int i=0;i<(int)sceneData->helperList.size();i++){
            if (!sceneData->helperList[i]->bHidden && sceneData->helperList[i]->bPickable){
				drawActor(sceneData->helperList[i]);
            }
        }

    }


    //reset texture Matrix transform
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);


    //this for xyz axis
    if (!Control::bRunning){

        setupShading("color");
        for (int i=0;i<(int)currentLayer->actorList.size();i++){
            drawGizmos(currentLayer->actorList[i]);
        }
    }


}

Renderer* X1600Renderer::getInstance(){

    if (Renderer::rendererInstance)
        return Renderer::rendererInstance;
    else{
        Renderer::rendererInstance=new X1600Renderer;
        return Renderer::rendererInstance;
	}
}


=======
/*
 *  x1600Renderer.cpp
 *  MSB_mac
 *
 *  Created by Friedrich Kirschner on 7/18/10.
 *  Copyright 2010 Friedrich Kirschner. All rights reserved.
 *
 */


#include "x1600Renderer.h"
#include "input.h"

X1600Renderer::X1600Renderer(){
}

X1600Renderer::~X1600Renderer(){
}


void X1600Renderer::drawSceneTexture(){

    glPushAttrib(GL_VIEWPORT_BIT);

    glViewport (0, 0, scene_size, scene_size);

    glMatrixMode(GL_MODELVIEW);


    for (int i=0;i<(int)sceneData->layerList.size();i++){


		glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, multiSample_fb);

		glDrawBuffers(1, drawBuffers);

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );


        glClear( GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT );

		glEnable(GL_BLEND);

		glActiveTexture(GL_TEXTURE0);

        //drawbuffers are set up here!
        drawColor3D(sceneData->layerList[i]);


        //color blitting

        glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, multiSample_fb );
        glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, sceneData->layerList[i]->colorFBO );
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBlitFramebufferEXT( 0, 0, scene_size, scene_size, 0, 0, scene_size, scene_size, GL_COLOR_BUFFER_BIT, texFilter );

		//cleanup
		glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, 0 );
		glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, 0 );
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

        //disable blending for second and third buffer and re-render


		glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, multiSample_fb);

		glDrawBuffers(2, drawBuffers);

        glClearColor( -1.0f, -1.0f, -1.0f, -1.0f );


        glClear( GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT );

		//TODO: ATI BUG
		glDisable(GL_BLEND);

        drawData3D(sceneData->layerList[i]);

        //depth blitting
        glReadBuffer(GL_COLOR_ATTACHMENT1_EXT);

        glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, sceneData->layerList[i]->depthFBO );
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBlitFramebufferEXT( 0, 0, scene_size, scene_size, 0, 0, scene_size, scene_size, GL_COLOR_BUFFER_BIT, GL_NEAREST );
/*
        //picking blitting
        glReadBuffer(GL_COLOR_ATTACHMENT2_EXT);

        glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, sceneData->layerList[i]->pickFBO );
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBlitFramebufferEXT( 0, 0, scene_size, scene_size, 0, 0, scene_size, scene_size, GL_COLOR_BUFFER_BIT, GL_NEAREST );

        //lightInfo blitting

        glReadBuffer(GL_COLOR_ATTACHMENT3_EXT);

        glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, sceneData->layerList[i]->lightDataFBO );
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

        glBlitFramebufferEXT( 0, 0, scene_size, scene_size, 0, 0, scene_size, scene_size, GL_COLOR_BUFFER_BIT, GL_NEAREST );
*/
    }
	glEnable(GL_BLEND);

    //draw brush here?

    //cleanup
    glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, 0 );
    glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, 0 );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

	pick(input->mouseX,input->mouseY);

    glPopAttrib();
    //now draw the resulting image into a quad!
}



void X1600Renderer::drawColor3D(Layer* currentLayer){


    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    //draw color

    for (int i=0;i<(int)currentLayer->actorList.size(); i++){
        //update matrices when hidden
        if(currentLayer->actorList[i]->bHidden){
            glPushMatrix();
            transformActorMatrix(currentLayer->actorList[i]);
            glPopMatrix();
        }
        else {
            //draw in all buffers for pickable actors
            if (currentLayer->actorList[i]->bPickable){
                drawActor(currentLayer->actorList[i]);
            }
            //don't draw in picking buffer for non-pickable actors
        }
    }


    //draw non-pickable actors afterwards!
    //used for drawings while drawing, so they're visible
    for (int i=0;i<(int)currentLayer->actorList.size(); i++){
        if (!currentLayer->actorList[i]->bPickable){
            drawActor(currentLayer->actorList[i]);
        }
    }

	//draw helpers - brush, grid, etc... if we're not running
    if (!sceneData->controller->bRunning){

        for (int i=0;i<(int)sceneData->helperList.size();i++){
            if (!sceneData->helperList[i]->bHidden){
                    drawActor(sceneData->helperList[i]);
            }
        }

    }


    //reset texture Matrix transform
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);


    //this for xyz axis
    if (!Control::bRunning){

        //glDepthMask(GL_FALSE);
        setupShading("color");
        for (int i=0;i<(int)currentLayer->actorList.size();i++){
            drawGizmos(currentLayer->actorList[i]);
        }

        //glDepthMask(GL_TRUE);
    }


}



void X1600Renderer::drawData3D(Layer* currentLayer){

    glActiveTexture(GL_TEXTURE0);
    //draw normal, depth, etc...

    for (int i=0;i<(int)currentLayer->actorList.size(); i++){
            //draw in all buffers for pickable actors
            if (currentLayer->actorList[i]->bPickable){
                drawActor(currentLayer->actorList[i]);
            }
    }



	//draw helpers - brush, grid, etc... if we're not running
    if (!sceneData->controller->bRunning){

        for (int i=0;i<(int)sceneData->helperList.size();i++){
            if (!sceneData->helperList[i]->bHidden && sceneData->helperList[i]->bPickable){
				drawActor(sceneData->helperList[i]);
            }
        }

    }


    //reset texture Matrix transform
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);


    //this for xyz axis
    if (!Control::bRunning){

        setupShading("color");
        for (int i=0;i<(int)currentLayer->actorList.size();i++){
            drawGizmos(currentLayer->actorList[i]);
        }
    }


}

Renderer* X1600Renderer::getInstance(){

    if (Renderer::rendererInstance)
        return Renderer::rendererInstance;
    else{
        Renderer::rendererInstance=new X1600Renderer;
        return Renderer::rendererInstance;
	}
}


>>>>>>> 96f565aa9f13cc6b10d6fcb5d95920e9ca9b80d8
