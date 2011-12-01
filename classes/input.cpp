#include "input.h"
#include "basicButton.h"
#include "listButton.h"
#include "control.h"
#include "interpolationHelper.h"
#include "drawingWidget.h"
#include "boneWidget.h"
#include "brush.h"
#include "moveButton.h"
#include "textInputButton.h"
#include "colladaLoader.h"
#include "spriteMeshLoader.h"
#include "spriteMeshLoaderXML.h"
#include "character.h"
#include "boneActor.h"
#include "udpInput.h"
#include "assignButton.h"
#include "userPopUp.h"

//define all static thingys:
Input* Input::inputInstance=NULL;

Input::Input(){

    lastKey=0;

    mouseX=0,
    mouseY=0,

    startPressLeftBtn=0,
    startPressRightBtn=0,

    pressedLeft=false,
    pressedRight=false,
    pressedMiddle=false,
    bShiftDown=false,
    bCtrlDown=false,
    bAltDown=false,
    bModifierPressed=false,
    bTextInput=false,
    bKeepSelection=false;
    bPressedMovementKeys=false;
    bConfineMouse=false;
    inputText="";
    tooltip="";
    //eventTrigger="NULL";

    keyVector=Vector3f(0,0,0),          //delta!!!!
    mouseVector=Vector3f(0,0,0),        //delta!!!!
    mouse3D=Vector3f(0,0,0);            //mouse position in 3D coordinates (uses picking)


    focusButton=NULL;     //the button that gets all our attention
    dragButton=NULL;       // the button we're currently dragging with us
    hudTarget=NULL;       // the button we're currently hovering on

    worldTarget=NULL;

    upDown=NULL;
    leftRight=NULL;

}

Input::~Input(){}

void Input::registerProperties(){

}

Input* Input::getInstance(){

    if (inputInstance)
        return inputInstance;
    else{
        inputInstance=new Input;
        return inputInstance;
        }
}

void Input::setup(){

    renderer=Renderer::getInstance();
    sceneData=SceneData::getInstance();
}

//Here we check all buttons and see if the mouse is hovering over them!
void Input::update(double deltaTime){


    BasicButton *myButton;
    hudTarget=NULL;

    if (worldTarget){
        worldTarget->mouseOver();
    }


    if (sceneData->staticButton){
        sceneData->staticButton->update(deltaTime);
        focusButton=sceneData->staticButton;
    }

    for (unsigned int i=0; i<sceneData->buttonList.size();i++){

        //exclude nodes from mouse over and stuff! they have been updated in renderer already
        myButton=sceneData->buttonList[i];
        Node* myNode=dynamic_cast<Node*>(myButton);
		NodeIO* myNodeIO=dynamic_cast<NodeIO*>(myButton);
		if (myNode || myNodeIO){
            if (!renderer->bDrawNodes){
                continue;
            }
		}

        if (myButton->bAlwaysUpdate)
            myButton->update(deltaTime);

        if (myButton->bHidden)
            continue;

        if (mouseX > myButton->location.x && mouseX < myButton->location.x + myButton->scale.x){
            if (mouseY > myButton->location.y && mouseY < myButton->location.y + myButton->scale.y){
                hudTarget=myButton;
            }
        }
    }



    //only one hudTarget should mouseOver at a time!
    if (hudTarget){
        hudTarget->mouseOver();
    }
}

void Input::resetInputVectors(){

    //reset last pressed key after controller update!
    lastKey=0;


    if (pressedLeft)
        startPressLeftBtn++;
    if (pressedRight)
        startPressRightBtn++;



    mouseVector*=0;
    if (!bPressedMovementKeys)
        keyVector*=0;
}

void Input::pressedMouse(int button,int state,int x, int y){

    if (bTextInput){
        cout << "In text input mode! Cannot press stuff!" << endl;
        return;
    }


    cout << "pressed button: " << button << endl;

    //don't forget the modifiers!

#ifdef TARGET_WIN32
        bShiftDown=((GetKeyState( VK_SHIFT ) & 0x80) > 0);
        bCtrlDown=((GetKeyState( VK_CONTROL ) & 0x80) > 0);
        bAltDown=((GetKeyState( VK_MENU ) & 0x80) > 0);
#endif


#ifdef TARGET_LINUX
        if (glutGetModifiers()==GLUT_ACTIVE_SHIFT)
            bShiftDown=true;
        if (glutGetModifiers()==GLUT_ACTIVE_CTRL)
            bCtrlDown=true;
#endif

	//some MOUSE INFO
    //boolean flags and
    //counting the frames we pressed the button - makes it easy to check against!
    //e.g.: if (Input::startPressRightBtn==0) -> the frame the button got pressed!

    if (button==MOUSEBTNLEFT && state==MOUSEBTNRELEASED){
            pressedLeft=false;
            startPressLeftBtn=0;
        }

    if (button==MOUSEBTNRIGHT && state==MOUSEBTNRELEASED){
            pressedRight=false;
            startPressRightBtn=0;
        }

    if (button==MOUSEBTNLEFT && state==MOUSEBTNPRESSED)   pressedLeft=true;

    if (button==MOUSEBTNRIGHT && state==MOUSEBTNPRESSED)  pressedRight=true;


    //then check if we're on a button
    if (state==MOUSEBTNRELEASED){

		//DESELECTION
		if ( !hudTarget && !focusButton && !dragButton && !sceneData->staticButton ){

			if (!worldTarget && !bKeepSelection){
				deselectActors();
			}
			deselectButtons(0);
		}

        //first Buttons
		//BUTTON - CLICKING

		//click on button - then click on world
		if (focusButton){
			focusButton->focusClick();
			return;
		}
		else if (sceneData->staticButton){
		    cout << "have staticBtn!" << endl;
            return;
		}

		//drag button and release
		else if (dragButton){
			dragButton->finishDrag();
			dragButton=NULL;
			return;
		}

		// regular click on button
		else if (hudTarget && state==MOUSEBTNRELEASED){
			if (button==MOUSEBTNLEFT && state==MOUSEBTNRELEASED)
				hudTarget->clickedLeft();
			if (button==MOUSEBTNRIGHT && state==MOUSEBTNRELEASED)
				hudTarget->clickedRight();
            return;
		}


	}//END MouseBtnReleased

		//tool states

    if (state==MOUSEBTNPRESSED)
        sceneData->controller->currentTool->mousePressed(button);


    if (state==MOUSEBTNRELEASED)
        sceneData->controller->currentTool->mouseReleased(button);



}


void Input::moveMouse(int x, int y){


	#ifdef TARGET_WIN32

        bShiftDown=((GetKeyState( VK_SHIFT ) & 0x80) > 0);
        bCtrlDown=((GetKeyState( VK_CONTROL ) & 0x80) > 0);
        bAltDown=((GetKeyState( VK_MENU ) & 0x80) > 0);
	#endif

	#ifdef TARGET_MACOSX
		bShiftDown=CGEventSourceKeyState(0,(CGKeyCode)56);
		bCtrlDown=CGEventSourceKeyState(0,(CGKeyCode)59);
	#endif

    #ifdef TARGET_LINUX
        bShiftDown=false;
        bCtrlDown=false;
        if (glutGetModifiers()==GLUT_ACTIVE_SHIFT)
            bShiftDown=true;
        if (glutGetModifiers()==GLUT_ACTIVE_CTRL)
            bCtrlDown=true;
    #endif

	mouseVector.x=x-mouseX;
	mouseVector.y=(y-mouseY)*sceneData->invertMouse;

	mouseX=x;
	mouseY=y;


	if (bConfineMouse){

       if (mouseX>renderer->screenX/2+200 || mouseX < renderer->screenX/2-200)
                confineMouse();

       if (mouseY>renderer->screenY/2+200 || mouseY < renderer->screenY/2-200)
                confineMouse();

        bConfineMouse=false;
    }

}

void Input::dragMouse(int x, int y){

	#ifdef TARGET_WIN32

        bShiftDown=((GetKeyState( VK_SHIFT ) & 0x80) > 0);
        bCtrlDown=((GetKeyState( VK_CONTROL ) & 0x80) > 0);
        bAltDown=((GetKeyState( VK_MENU ) & 0x80) > 0);

	#endif

	#ifdef TARGET_MACOSX

		bShiftDown=CGEventSourceKeyState(0,(CGKeyCode)56);
		bCtrlDown=CGEventSourceKeyState(0,(CGKeyCode)59);

	#endif

    #ifdef TARGET_LINUX
        bShiftDown=false;
        bCtrlDown=false;
        if (glutGetModifiers()==GLUT_ACTIVE_SHIFT)
            bShiftDown=true;
        if (glutGetModifiers()==GLUT_ACTIVE_CTRL)
            bCtrlDown=true;
    #endif


	if (dragButton && dragButton->bDragable){
	  dragButton->mouseDrag();
	  }
	else if (hudTarget && hudTarget->bDragable)
	  hudTarget->mouseDrag();

	//delta
	mouseVector.x=x-mouseX;//yay!
	mouseVector.y=(y-mouseY)*sceneData->invertMouse;

	mouseX=x;
	mouseY=y;



	if (bConfineMouse){

       if (mouseX>renderer->screenX/2+200 || mouseX < renderer->screenX/2-200)
                confineMouse();

       if (mouseY>renderer->screenY/2+200 || mouseY < renderer->screenY/2-200)
                confineMouse();

        bConfineMouse=false;
    }
}


//****************************************************************************************
// Key is pressed down

void Input::normalKeyDown(unsigned char key, int x, int y){

    ///W,A,S,D for movement Q,E for up/down

    if (!bTextInput){

        if (key=='d'){
               keyVector.x=1;
               bPressedMovementKeys=true;
        }
        if (key=='a'){
               keyVector.x=-1;
               bPressedMovementKeys=true;
        }

        if (key=='q'){
               keyVector.y=1;
               bPressedMovementKeys=true;
        }
        if (key=='e'){
               keyVector.y=-1;
               bPressedMovementKeys=true;
        }


        if (key=='w'){
               keyVector.z=-1;
               bPressedMovementKeys=true;
        }
        if (key=='s'){
               keyVector.z=1;
               bPressedMovementKeys=true;
        }

	#ifdef TARGET_WIN32

        bShiftDown=((GetKeyState( VK_SHIFT ) & 0x80) > 0);
        bCtrlDown=((GetKeyState( VK_CONTROL ) & 0x80) > 0);
        bAltDown=((GetKeyState( VK_MENU ) & 0x80) > 0);
    #endif

    #ifdef TARGET_LINUX
        bShiftDown=false;
        bCtrlDown=false;
        if (glutGetModifiers()==GLUT_ACTIVE_SHIFT)
            bShiftDown=true;
        if (glutGetModifiers()==GLUT_ACTIVE_CTRL)
            bCtrlDown=true;
    #endif

        lastKey=key;

        sceneData->controller->currentTool->keyPressed(key);

    }
    else{

        if (key==127 || key==8){                               //pressed Delete!!!
            if (inputText.size()>0)
                inputText.erase(inputText.size()-1);
        }
        else if (key==13){                                     //pressed Enter!!!

            bTextInput=false;
            if (focusButton)                                    //be sure to call back to the button!
                focusButton->focusClick();
        }
        else if (key==27){                                     //pressed ESC!!!

            bTextInput=false;
            Input::deselectButtons(0);
        }
        else
          inputText+=key;
    }
}

void Input::specialKeyDown(int key, int x, int y){

//make this relative to where we look at!
    if (key==GLUT_KEY_RIGHT){
        keyVector.x=1;
        bPressedMovementKeys=true;
    }

    if (key==GLUT_KEY_LEFT){
        keyVector.x=-1;
        bPressedMovementKeys=true;
    }

    if (key==GLUT_KEY_PAGE_UP){
        keyVector.y=1;
        bPressedMovementKeys=true;
    }

    if (key==GLUT_KEY_PAGE_DOWN){
        keyVector.y=-1;
        bPressedMovementKeys=true;
    }


    if (key==GLUT_KEY_UP){
        keyVector.z=-1;
        bPressedMovementKeys=true;
    }
    if (key==GLUT_KEY_DOWN){
        keyVector.z=1;
        bPressedMovementKeys=true;
    }

    if (key==GLUT_KEY_END)
           exit(0);

	#ifdef TARGET_WIN32

        bShiftDown=((GetKeyState( VK_SHIFT ) & 0x80) > 0);
        bCtrlDown=((GetKeyState( VK_CONTROL ) & 0x80) > 0);
        bAltDown=((GetKeyState( VK_MENU ) & 0x80) > 0);
    #endif

    #ifdef TARGET_LINUX
        bShiftDown=false;
        bCtrlDown=false;
        if (glutGetModifiers()==GLUT_ACTIVE_SHIFT)
            bShiftDown=true;
        if (glutGetModifiers()==GLUT_ACTIVE_CTRL)
            bCtrlDown=true;
    #endif


}


//****************************************************************************************
// Key is released

void Input::keyUp(unsigned char key,int x,int y){

    //TODO: read these keycommands from an xml file


    if (!bTextInput){

    ///W,A,S,D movement Q,E for up/down

        if (key=='a' && keyVector.x==-1){
               keyVector.x=0;
               bPressedMovementKeys=false;
        }
        if (key=='d' && keyVector.x==1){
               keyVector.x=0;
               bPressedMovementKeys=false;
        }

        if (key=='q'){
               keyVector.y=0;
               bPressedMovementKeys=false;
        }
        if (key=='e'){
               keyVector.y=0;
               bPressedMovementKeys=false;
        }

        if (key=='w'){
               Input::keyVector.z=0;
               bPressedMovementKeys=false;
        }

        if (key=='s'){
               keyVector.z=0;
               bPressedMovementKeys=false;
        }

    ///System stuff

        //pressed TAB
        if (key==9){
            if (sceneData->controller->tool==TOOL_NAV || sceneData->controller->tool==TOOL_ORBIT){
                sceneData->controller->switchTool(sceneData->controller->oldTool);
            }else{
                sceneData->controller->myTools[TOOL_NAV]->myBtn->clickedLeft();
            }

        }

        //pressed ESC!!!
        if (key==27){
            Input::deselectButtons(0);
            bTextInput=false;
        }

        //space - run scene
        if (key==' ')
          sceneData->controller->startMovie();

        //pressed Delete!!!
        //TODO: this is also implemented in selectTool! Only need one!
        if (key==127 || key==8){
            if (hudTarget){
                Node* n=dynamic_cast<Node*>(hudTarget);
                if (n)  {
                    deselectButtons(0);
                    n->remove();
                }
            hudTarget=NULL;
            }
        }

        //enable physics simulation p
        /*
        if (key=='P')                               //pressed s
          renderer->bUpdatePhysics=!renderer->bUpdatePhysics;
        */

    //Ctrl-S saves current scene
    if (key==19)
        sceneData->saveAll(sceneData->currentScene);

    ///Hotkeys

        //trigger selected actor
        if (key=='T')
            if (sceneData->selectedActors[0])
                sceneData->selectedActors[0]->trigger(NULL);

        // reload all shaders!
        if (key=='S')
               sceneData->loadShaders("shaders/","resources/basic.library");

        // reload all basic textures!
        if (key=='X')
               sceneData->loadTextures("resources/icons/","resources/basic.library");

        if (key=='i')
            sceneData->controller->myTools[TOOL_SELECT]->myBtn->clickedLeft();

        /*
        if (key=='b'){
            renderer->bUseBlending=!renderer->bUseBlending;
            cout << "switched blending to: "<<renderer->bUseBlending << endl;
        }
        */

        if (key=='b'){
            if (((BoneWidget*)sceneData->controller->myTools[TOOL_BONE]->myBtn)->bWidgetOpen){
                sceneData->controller->switchTool(TOOL_BONE);
            }else{
                sceneData->controller->myTools[TOOL_BONE]->myBtn->clickedLeft();
            }
        }

        //switch to grid
        if (key=='g'){
            sceneData->controller->myTools[TOOL_GRID]->myBtn->clickedLeft();
        }

        //switch to drawing
        if (key=='p'){
            if (((DrawingWidget*)sceneData->controller->myTools[TOOL_DRAW]->myBtn)->bWidgetOpen){
                sceneData->controller->switchTool(TOOL_DRAW);
            }else{
                sceneData->controller->myTools[TOOL_DRAW]->myBtn->clickedLeft();
            }

        }

        //switch fullscreen
        if (key=='F'){
            if (renderer->bFullscreen){
                glutLeaveGameMode();
            }else{
                glutEnterGameMode();
            }
            renderer->bFullscreen=!renderer->bFullscreen;
        }



        //create bone
        if (key=='k'){
            BoneActor* bone = new BoneActor;
            bone->setLocation(sceneData->controller->controlledActor->location);
            bone->setRotation(sceneData->controller->controlledActor->rotation);
            sceneData->actorList.push_back(bone);
        }



    ///debug:

       cout << "pressed key: " << (int)key << endl;

    ///state key update
       sceneData->controller->currentTool->keyReleased(key);

    }//end !TextInput
}


void Input::specialKeyUp (int key,int x, int y){

    if (key==GLUT_KEY_LEFT && keyVector.x==-1){
        keyVector.x=0;
        bPressedMovementKeys=false;
    }
    if (key==GLUT_KEY_RIGHT && keyVector.x==1){
        keyVector.x=0;
        bPressedMovementKeys=false;
    }

    if (key==GLUT_KEY_PAGE_UP){
        keyVector.y=0;
        bPressedMovementKeys=false;
    }
    if (key==GLUT_KEY_PAGE_DOWN){
        keyVector.y=0;
        bPressedMovementKeys=false;
    }


    if (key==GLUT_KEY_UP){
        Input::keyVector.z=0;
        bPressedMovementKeys=false;
    }
    if (key==GLUT_KEY_DOWN){
        keyVector.z=0;
        bPressedMovementKeys=false;
    }

    if (key==GLUT_KEY_F1){
        renderer->bDrawMenu=!renderer->bDrawMenu;
        return;
    }

    if (key==GLUT_KEY_F2){
        renderer->bDrawNodes=!renderer->bDrawNodes;
        return;
    }

    if (key==GLUT_KEY_F3){
        renderer->bDrawLighting=!renderer->bDrawLighting;
        return;
    }

    if (key==GLUT_KEY_F4){
        renderer->bSSAO=!renderer->bSSAO;
        return;
    }

    if (key==GLUT_KEY_F5){
        renderer->bDrawColor=!renderer->bDrawColor;
        return;
    }

    if (key==GLUT_KEY_F6){
        renderer->bDrawNormals=!renderer->bDrawNormals;
        return;
    }

    if (key==GLUT_KEY_F7){
        renderer->bDOF=!renderer->bDOF;
        return;
    }


}


//****************************************************************************************
// selection

void Input::selectActor(int button){


}

void Input::deselectButtons(int depth){

cout << "deselecting!" << endl;

    if (sceneData->actorMenu){
      sceneData->actorMenu->deselect(0);
      sceneData->actorMenu=NULL;
    }

    vector<BasicButton*>::iterator it;
    //make sure all Buttons are cleaned up!

	for (int i=sceneData->buttonList.size()-1;i>=0;i--){
		sceneData->buttonList[i]->deselect(depth);
	}
}

void Input::deselectActors(){

    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
        sceneData->selectedActors[i]->bSelected=false;
    }
    sceneData->selectedActors.clear();

    cout << "clearing Actor selection stack..." << endl;

}


void Input::confineMouse(){



#ifdef TARGET_WIN32
    glutWarpPointer(renderer->screenX/2,renderer->screenY/2);
#endif

#ifdef TARGET_LINUX
    glutWarpPointer(renderer->screenX/2,renderer->screenY/2);
#endif

#ifdef TARGET_MACOSX
	CGPoint myPoint;
	    int centerX=renderer->screenX/2;
	    int centerY=renderer->screenY/2;
	myPoint.x=centerX+renderer->windowPosX;
	myPoint.y=centerY+renderer->windowPosY;
	CGDisplayMoveCursorToPoint( CGMainDisplayID (),myPoint);
#endif

	mouseX=renderer->screenX/2;
	mouseY=renderer->screenY/2;

}

