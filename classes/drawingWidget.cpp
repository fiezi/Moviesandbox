#include "drawingWidget.h"
#include "input.h"
#include "renderer.h"
#include "sliderButton.h"
#include "spriteMeshLoader.h"
#include "drawTool.h"

DrawingWidget::DrawingWidget(){

name="drawWidget";
tooltip="draw";

bWidgetOpen=false;
bKinectToolOpen=false;

color=Vector4f(0.8,0.8,0.8,1.0);
listDisplayMode=0;

}

DrawingWidget::~DrawingWidget(){

}

void DrawingWidget::setup(){

    Widget::setup();
    drawTool=(DrawTool*)input->controller->myTools[TOOL_DRAW];
}

void DrawingWidget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);
    input->controller->switchTool(TOOL_DRAW);

    if (bKinectToolOpen){
        listButton[4]->color=COLOR_YELLOW;
    }



}

void DrawingWidget::closeWidget(){

    input->deselectActors();


    if (renderer->brush->drawing){
        input->makeWarningPopUp("saving drawing...",this);
        renderer->draw();
        input->controller->myTools[TOOL_DRAW]->save();
        renderer->brush->drawing->drawType=DRAW_VBOMESH;
        renderer->brush->drawing->reset();
        input->staticButton->focusClick();
        input->bTextInput=false;
    }


    input->controller->switchTool(TOOL_SELECT);
    color=COLOR_WHITE;

}



///creates new drawing here!
void DrawingWidget::trigger(Actor * other){

    if (other->name=="New Drawing" && (input->controller->tool==TOOL_DRAW || input->controller->tool==TOOL_CALLIGRAPHY) ){
        input->makeUserPopUp("Name your new drawing:",this);
    }

    //from staticButton
    if (other->name=="Name your new drawing:"){
        renderer->brush->createNewDrawing();
        input->specialSelected=renderer->brush->drawing;
    }

    if (other->name=="Draw Particles (p)"){
        input->controller->switchTool(TOOL_DRAW);
    }

    if (other->name=="Draw Calligraphy"){
        input->controller->switchTool(TOOL_CALLIGRAPHY);
    }

    if (other->name=="Select Particles"){
        input->controller->switchTool(TOOL_PARTICLESELECT);
    }

	if (other->name=="save"){
        input->controller->currentTool->save();
	}

    if (other->name=="save As..."){
        cout << "creating VBO..." << endl;
        renderer->spriteMeshLoader->saveSpriteMesh("resources/meshes/"+input->inputText+".spriteMesh",(SkeletalActor*)(renderer->brush->drawing));
        renderer->spriteMeshLoader->loadSpriteMesh("resources/meshes/"+input->inputText+".spriteMesh",input->inputText);

		renderer->brush->drawing->name=input->inputText;
		renderer->brush->drawing->vboMeshID=input->inputText;

        TiXmlElement * newMesh= new TiXmlElement("SpriteMesh");
        newMesh->SetAttribute("meshID",input->inputText);
        newMesh->SetAttribute("meshFilename","resources/meshes/"+input->inputText+".spriteMesh");
        input->addToLibrary(newMesh);
        free(newMesh);
    }


    if (other->name=="merge Drawings"){
        drawTool->mergeDrawings();
    }
///scale paticle size

    if (other->name=="scaleZ x2"){
        drawTool->scaleZ(2.0);
    }

    if (other->name=="scaleZ x0.5"){
        drawTool->scaleZ(0.5);
    }

    if (other->name=="Import Kinect"){

        if (other->color==Vector4f(1,1,0,1)){
            //TODO: move these functions to drawTool!
            importKinect();
            other->color=COLOR_WHITE;
            closeKinectTool();
            bKinectToolOpen=false;
        }
        else{
            openKinectTool();
            other->color=COLOR_YELLOW;
            bKinectToolOpen=true;
        }
    }

}

void DrawingWidget::closeKinectTool(){

#ifdef TARGET_WIN32
    cout << "closing Kinect stuff..." << endl;

    TerminateProcess(pi.hProcess,0);
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
#else

    //taken from: http://www.yolinux.com/TUTORIALS/ForkExecProcesses.html

    int  killReturn = killpg( pID_t, SIGKILL);  // Kill child process group

    if( killReturn == ESRCH)      // pid does not exist
    {
       cout << "Group does not exist!" << endl;
    }
    else if( killReturn == EPERM) // No permission to send signal
    {
       cout << "No permission to send signal!" << endl;
    }
    else
       cout << "Signal sent. All Ok!" << endl;

#endif

}

void DrawingWidget::openKinectTool(){

    renderer->brush->drawing->drawType=DRAW_POINTPATCH;
    renderer->brush->drawing->bTextured=true;
    renderer->brush->drawing->textureID="sharedMemory";
    renderer->brush->drawing->sceneShaderID="heightfield";
    renderer->brush->drawing->particleScale=100;

#ifdef TARGET_WIN32

    //call msbKinect
    //CreateProcess(,,,false,NORMAL_PRIORITY_CLASS,,"tools//msbKinect//");
    char* exePath="tools\\msbKinect\\msbKinect.exe";
    char* workingDir="tools\\msbKinect";


                ZeroMemory( &si, sizeof(si) );
                si.cb = sizeof(si);
                ZeroMemory( &pi, sizeof(pi) );

                // Start the child process.
                if( !CreateProcess( NULL,   // No module name (use command line)
                    exePath,        // Command line
                    NULL,           // Process handle not inheritable
                    NULL,           // Thread handle not inheritable
                    FALSE,          // Set handle inheritance to FALSE
                    0,              // No creation flags
                    NULL,           // Use parent's environment block
                    workingDir,           // Use working directory
                    &si,            // Pointer to STARTUPINFO structure
                    &pi )           // Pointer to PROCESS_INFORMATION structure
                )
                {
                    printf( "CreateProcess failed (%d).\n", GetLastError() );
                    return;
                }

                // Wait until child process exits.
                //WaitForSingleObject( pi.hProcess, INFINITE );

                // Close process and thread handles.
#else

        if ((processId = fork()) == 0) {
            char app[] = "tools/msbKinect/msbKinect.app/Contents/MacOS/msbKinect";
            char * const argv[] = { app, NULL };
            if (execve(app, argv, NULL) < 0) {
                perror("execv error:");
            }
        } else if (processId < 0) {
            perror("fork error");
        }
        return;

#endif
}

void DrawingWidget::importKinect(){

            static float kinectContent[1024*1024*4];
            glBindTexture(GL_TEXTURE_2D,renderer->textureList["sharedMemory"]->texture);
            glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,&kinectContent);
            glBindTexture(GL_TEXTURE_2D,0);
            renderer->brush->drawing->drawType=DRAW_PARTICLES;
            renderer->brush->drawing->bTextured=false;
            renderer->brush->drawing->textureID="NULL";
            renderer->brush->drawing->sceneShaderID="color";
            renderer->brush->drawing->bPickable=true;
            renderer->brush->drawing->particleScale=2;
            for (int i=0;i<1024*1024*4;i+=4){
                Vector3f myLoc;
                myLoc.z=kinectContent[i+3] * 10.0f;
                if (myLoc.z==0.0f)
                    continue;
                myLoc.x= (i/4)%1024;
                myLoc.y= (int)((i/4)/1024 );

                myLoc.x=myLoc.x/1024.0f - 0.5f;
                myLoc.y=myLoc.y/1024.0f - 0.5f;

                myLoc.x*=myLoc.z;
                myLoc.y*=myLoc.z;

                renderer->brush->setLocation(myLoc);
                renderer->brush->color=Vector4f(kinectContent[i],kinectContent[i+1],kinectContent[i+2],1.0);
                renderer->brush->normalMode=NORMAL_FRONT;
                input->mouseVector.x=1.0f;

                drawTool->paint();
            }
}

void DrawingWidget::create(){renderer->addButton(this);}

