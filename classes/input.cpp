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

    screenX=0,
    screenY=0,

    windowX=0,
    windowY=0,

    invertMouse=-1,

    startPressLeftBtn=0,
    startPressRightBtn=0,

    numParticles=0;

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

    savedDrawingDirName="resources/drawings/",
    savedSceneDirName="resources/scenes/",
    savedActionsDirName="resources/actions/";

    keyVector=Vector3f(0,0,0),          //delta!!!!
    mouseVector=Vector3f(0,0,0),        //delta!!!!
    mouse3D=Vector3f(0,0,0);            //mouse position in 3D coordinates (uses picking)

    gridSize=0.00010f;                      //grid!

    controller=NULL;

    focusButton=NULL;     //the button that gets all our attention
    dragButton=NULL;       // the button we're currently dragging with us
    hudTarget=NULL;       // the button we're currently hovering on
    staticButton=NULL;    //in case we want to access Button functionality without actually displaying a Button

    worldTarget=NULL;
	specialSelected=NULL;
    debugText="Moviesandbox 2.0";

    actorMenu=NULL;
    inputInstance=NULL;



    upDown=NULL;
    leftRight=NULL;
}

Input::~Input(){}

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
    inspectorManager=new InspectorManager;

	windowX=100;
	windowY=100;

	glutIgnoreKeyRepeat(1);

	controller=new Control;
    controller->setup();
	verdana.loadFont("./resources/verdana.ttf",8);
	verdana.setLineHeight(20.0f);

	for (int i=0;i<FPSBUFFERSIZE;i++){
		fpsBuffer[i]=0.0;
	}

}

//Here we check all buttons and see if the mouse is hovering over them!
void Input::update(double deltaTime){


    BasicButton *myButton;
    hudTarget=NULL;

    if (staticButton){
        staticButton->update(deltaTime);
        focusButton=staticButton;
    }

    for (unsigned int i=0; i<renderer->buttonList.size();i++){

        //exclude nodes from mouse over and stuff! they have been updated in renderer already
        myButton=renderer->buttonList[i];
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
                hudTarget->mouseOver();
            }
        }
    }

    //for actor menu animation
    if (actorMenu)
        actorMenu->update(deltaTime);

    //update controller after assigning all hudTargets, staticButtons, etc...
    controller->update(deltaTime);

    //always enable moving around

    if (bPressedMovementKeys && controller->tool!=TOOL_NAV)
        controller->myTools[TOOL_NAV]->update(deltaTime);


    //reset last pressed key after controller update!
    lastKey=0;


    if (pressedLeft)
        startPressLeftBtn++;
    if (pressedRight)
        startPressRightBtn++;



    mouseVector*=0;
}

void Input::pressedMouse(int button,int state,int x, int y){


    cout << "pressed button: " << button << endl;

	// first gather information and select states
    windowX=glutGet( GLUT_WINDOW_X);
    windowY=glutGet( GLUT_WINDOW_Y);

    //don't forget the modifiers!

#ifdef TARGET_WIN32
        bShiftDown=((GetKeyState( VK_SHIFT ) & 0x80) > 0);
        bCtrlDown=((GetKeyState( VK_CONTROL ) & 0x80) > 0);
        bAltDown=((GetKeyState( VK_MENU ) & 0x80) > 0);
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
		if ( !hudTarget && !focusButton && !dragButton && !staticButton ){

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
		else if (staticButton){
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
        controller->currentTool->mousePressed(button);


    if (state==MOUSEBTNRELEASED)
        controller->currentTool->mouseReleased(button);



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

	mouseVector.x=x-mouseX;
	mouseVector.y=(y-mouseY)*invertMouse;

	mouseX=x;
	mouseY=y;


	if (bConfineMouse){

       if (mouseX>screenX/2+200 || mouseX < screenX/2-200)
                confineMouse();

       if (mouseY>screenY/2+200 || mouseY < screenY/2-200)
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

	if (dragButton && dragButton->bDragable){
	  dragButton->mouseDrag();
	  }
	else if (hudTarget && hudTarget->bDragable)
	  hudTarget->mouseDrag();

	//delta
	mouseVector.x=x-mouseX;//yay!
	mouseVector.y=(y-mouseY)*invertMouse;

	mouseX=x;
	mouseY=y;



	if (bConfineMouse){

       if (mouseX>screenX/2+200 || mouseX < screenX/2-200)
                confineMouse();

       if (mouseY>screenY/2+200 || mouseY < screenY/2-200)
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

        lastKey=key;

        controller->currentTool->keyPressed(key);

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
}


//****************************************************************************************
// Key is released

void Input::keyUp(unsigned char key,int x,int y){

    //TODO: read these keycommands from an xml file

    windowX=glutGet( GLUT_WINDOW_X);
    windowY=glutGet( GLUT_WINDOW_Y);

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
        if (key==9)
            controller->myTools[TOOL_NAV]->myBtn->clickedLeft();

        //pressed ESC!!!
        if (key==27)
          Input::deselectButtons(0);

        //space - run scene
        if (key==' ')
          controller->startMovie();

        //pressed Delete!!!
        if (key==127 || key==8){
            if (hudTarget){
                Node* n=dynamic_cast<Node*>(hudTarget);
                if (n)  {
                    deselectButtons(0);
                    n->remove();
                }
            }
        }

        //enable physics simulation p
        /*
        if (key=='P')                               //pressed s
          renderer->bUpdatePhysics=!renderer->bUpdatePhysics;
        */

    ///Hotkeys

        //trigger selected actor
        if (key=='T')
            if (selectedActors[0])
                selectedActors[0]->trigger(NULL);

        // reload all shaders!
        if (key=='S')
               loadShaders("resources/basic.library");

        if (key=='i')
            controller->myTools[TOOL_SELECT]->myBtn->clickedLeft();

        /*
        if (key=='b'){
            renderer->bUseBlending=!renderer->bUseBlending;
            cout << "switched blending to: "<<renderer->bUseBlending << endl;
        }
        */

        if (key=='b'){
            if (((BoneWidget*)controller->myTools[TOOL_BONE]->myBtn)->bWidgetOpen){
                controller->switchTool(TOOL_BONE);
            }else{
                controller->myTools[TOOL_BONE]->myBtn->clickedLeft();
            }
        }

        //switch to grid
        if (key=='g'){
            controller->myTools[TOOL_GRID]->myBtn->clickedLeft();
        }

        //switch to drawing
        if (key=='p'){
            if (((DrawingWidget*)controller->myTools[TOOL_DRAW]->myBtn)->bWidgetOpen){
                controller->switchTool(TOOL_DRAW);
            }else{
                controller->myTools[TOOL_DRAW]->myBtn->clickedLeft();
            }

        }




        //create bone
        if (key=='k'){
            BoneActor* bone = new BoneActor;
            bone->setLocation(controller->controlledActor->location);
            bone->setRotation(controller->controlledActor->rotation);
            renderer->actorList.push_back(bone);
        }

    ///debug:

       cout << "pressed key: " << (int)key << endl;

    ///state key update
       controller->currentTool->keyReleased(key);

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


}


//****************************************************************************************
// selection

void Input::selectActor(int button){


}

void Input::deselectButtons(int depth){

cout << "deselecting!" << endl;

    if (actorMenu){
      actorMenu->deselect(depth);
    }

    vector<BasicButton*>::iterator it;
    //make sure all Buttons are cleaned up!

	for (int i=renderer->buttonList.size()-1;i>=0;i--){
		renderer->buttonList[i]->deselect(depth);
	}
}

void Input::deselectActors(){

    for (int i=0;i<(int)selectedActors.size();i++){
        selectedActors[i]->bSelected=false;
    }
    selectedActors.clear();

    cout << "clearing Actor selection stack..." << endl;

}


void Input::makeUserPopUp(string text, Actor* parent){

    staticButton=(BasicButton*)renderer->actorInfo["9UserPopUp"].actorReference;
    ((UserPopUp*)staticButton)->bWaitForInput=true;
    staticButton->setLocation(Vector3f(screenX/2-200,screenY/2-50,0));
    staticButton->color=Vector4f(0.0,0.0,1.0,1.0);
    staticButton->name=text;
    staticButton->parent=parent;
    cout << "UserInput->parent is: " << parent->name << endl;
    staticButton->clickedLeft();
    renderer->buttonList.push_back(staticButton);
    lastMouse3D=mouse3D;
}

void Input::makeWarningPopUp(string message, Actor* parent){

    staticButton=(BasicButton*)renderer->actorInfo["9UserPopUp"].actorReference;
    staticButton->setLocation(Vector3f(screenX/2-200,screenY/2-50,0));
    staticButton->color=Vector4f(1.0,0.0,0.0,1.0);
    staticButton->name=message;
    staticButton->parent=parent;
    cout << "UserInput->parent is: " << parent->name << endl;
    staticButton->clickedLeft();
    renderer->buttonList.push_back(staticButton);
}


void Input::createNewSelectionMarker(Actor* selectedActor){

}

void Input::createActorMenu(){

	cout << "Creating radial menu for: " << worldTarget->name << endl;

	//make actor menu if we don't have one, else clean it up
	if (actorMenu)
		actorMenu->deselect(0);
	else
		actorMenu=new ListButton;

	actorMenu->setLocation( Vector3f( mouseX, mouseY, 0.0 ) );
	actorMenu->listDisplayMode=2;
	actorMenu->listType.assign(worldTarget->menuType.begin(),worldTarget->menuType.end());
	actorMenu->listName.assign(worldTarget->menuName.begin(),worldTarget->menuName.end());
	actorMenu->listIcon.assign(worldTarget->menuIcon.begin(),worldTarget->menuIcon.end());
	actorMenu->listProp.assign(worldTarget->menuProp.begin(),worldTarget->menuProp.end());
	actorMenu->parent=Input::worldTarget;
	actorMenu->clickedLeft();
}


void Input::confineMouse(){



    int centerX=windowX+screenX/2;
    int centerY=windowY+screenY/2;

#ifdef TARGET_WIN32
    glutWarpPointer(screenX/2,screenY/2);
#endif

#ifdef TARGET_MACOSX
		CGPoint myPoint;
		myPoint.x=centerX;
		myPoint.y=centerY;
		CGDisplayMoveCursorToPoint(	CGMainDisplayID (),myPoint);
#endif

	mouseX=screenX/2;
	mouseY=screenY/2;

}



//Display and Textdrawing
void Input::displayDebug(){

double deltaTime=0.0;

//shift buffer
for (int i=FPSBUFFERSIZE-1;i>0;i--)
  Input::fpsBuffer[i]=Input::fpsBuffer[i-1];

//update buffer
Input::fpsBuffer[0]=renderer->deltaTime;

//calculate buffer
for (int i=0;i<FPSBUFFERSIZE;i++)
  deltaTime+=Input::fpsBuffer[i];

deltaTime=deltaTime/FPSBUFFERSIZE;



	renderer->setupShading("font");

char writestring[30];

	for (int i=0;i<30;i++){
		writestring[i]=' ';
	}

sprintf(writestring,"FPS: %4.2f",1000.0/deltaTime);
drawText(writestring,screenX-screenX*0.75,20 );

	return;


	for (int i=0;i<30;i++){
		writestring[i]=' ';
	}

sprintf(writestring,"update: %4.2f",updateTime);
drawText(writestring,screenX-screenX*0.75,50 );

	for (int i=0;i<30;i++){
		writestring[i]=' ';
	}

sprintf(writestring,"Draw: %4.2f",drawTime);
drawText(writestring,screenX-screenX*0.75,80 );

	for (int i=0;i<30;i++){
		writestring[i]=' ';
	}

sprintf(writestring,"Draw3D: %4.2f",draw3DTime);
drawText(writestring,screenX-screenX*0.75,110 );

	for (int i=0;i<30;i++){
		writestring[i]=' ';
	}

sprintf(writestring,"Number of Particles: %i", numParticles);
drawText(writestring,screenX-screenX*0.75,140 );

}

void Input::drawText(string str, float x, float y){

 glColor4f(1.0,1.0,1.0,1.0);
 verdana.drawString(str, x, y);
}

void Input::drawText(string str, float x, float y, Vector4f col, float fontSize){

}


//**************************************************************************************
// loading

//TODO: load textures/meshes/stuff seperately or save it always too!
void Input::saveAll(std::string filename){

    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );
    TiXmlElement * root = new TiXmlElement( "Moviesandbox" );
    doc.LinkEndChild( root );

    //save controller location and rotation
    TiXmlElement * controlElement=new TiXmlElement("Controller");
    controlElement->SetAttribute("TransformMatrix",controller->memberToString(&controller->property["TRANSFORMMATRIX"]));
    root->LinkEndChild(controlElement);

    //first Actors
    for (unsigned int i=0;i<renderer->actorList.size();i++)
      {
      TiXmlElement * actorElement=renderer->actorList[i]->save(root);
      root->LinkEndChild(actorElement);
      }

    //then saveable Buttons
    for (unsigned int i=0;i<renderer->saveableButtonList.size();i++)
      {
      TiXmlElement * buttonElement=renderer->saveableButtonList[i]->save(root);
      root->LinkEndChild(buttonElement);
      }
    //then nodes
    for (unsigned int i=0;i<renderer->nodeList.size();i++)
      {
      TiXmlElement * nodeElement=renderer->nodeList[i]->save(root);
      root->LinkEndChild(nodeElement);
      }

    string saveString=savedSceneDirName;
    saveString.append(filename);

    cout << "saving filename: " << saveString << endl;

    doc.SaveFile( saveString );
}

void Input::loadAll(std::string fileName, bool bCleanUp){


    renderer->bUpdatePhysics=false;

    if (bCleanUp){
        //clean Up
        //let's not remove actions, okay?
        //let's not remove buttons? unless they are nodes?
        //Button first because of udpInput and threading

        //remove udpInputs
        if (renderer->buttonList.size()>0){
            for (int i=(int)renderer->buttonList.size()-1;i>=0;i--){
                //Action * act=NULL;
                //act=dynamic_cast<Action*>(renderer->buttonList[i]);
                UdpInput* udp=NULL;
                udp=dynamic_cast<UdpInput*>(renderer->buttonList[i]);
                if (renderer->buttonList[i]->level>0 && udp)
                    renderer->buttonList[i]->remove();
            }
        }

        if (renderer->actorList.size()>0){
            for (int i=(int)renderer->actorList.size()-1;i>=0;i--)
                renderer->actorList[i]->remove();
        }

        if (renderer->nodeList.size()>0){
            for (int i=(int)renderer->nodeList.size()-1;i>=0;i--)
                renderer->nodeList[i]->remove();
        }
    }


    string stringName=savedSceneDirName;
    stringName.append(fileName);

    cout << "Loading file..." << stringName <<endl;


    TiXmlDocument doc( stringName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    //***********************************************************************
    //Make sure this is a Moviesandbox file!
    //***********************************************************************

    element=hDoc.FirstChildElement().Element();
    if (!element) return;

    hRoot=TiXmlHandle(element);

    //***********************************************************************
    //Load Actor List
    //***********************************************************************

    //loading is a two step process
    int listPos=0;

    //load controller locations
    cout << "loading camera Position..." << endl;
    element=hRoot.FirstChild("Controller").Element();
    if (element){
        string cLoc=element->Attribute("TransformMatrix");
        controller->memberFromString(&controller->property["TRANSFORMMATRIX"], cLoc);
        controller->postLoad();
        //controller->processMove(0.0);
    }
    else cout << "no position info found... skipping..." << endl;

		cout << "loading actors..." << endl;

    //first create all Actors

      element=hRoot.FirstChild( "Actor" ).Element();
      string myType;
      for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
        {
        cout << "next element: " << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        Actor * A=renderer->actorInfo[myType].actorReference;
        A->create();
        }


    //then load all properties - for referencing reasons
      element=hRoot.FirstChild( "Actor" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
        {
        Actor* A=renderer->actorList[listPos];
        myType=element->GetText();
        cout << "Loading property type: " << myType << endl;
        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        A->load(element);
        listPos++;
        }

    //***********************************************************************
    //Load Node List
    //***********************************************************************
    listPos=0;

    cout << "loading nodes..." << endl;

      element=hRoot.FirstChild( "Node" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Node"))
        {
        cout << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        Node* N=(Node*)renderer->actorInfo[myType].actorReference;
        N->create();
        }

      element=hRoot.FirstChild( "Node" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Node"))
        {
        Node* N=renderer->nodeList[listPos];
        myType=element->GetText();
        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        N->load(element);
        listPos++;
        }
      for (uint i=0; i< renderer->nodeList.size();i++){
        renderer->nodeList[i]->setup();
        }


    //***********************************************************************
    //Setup Actor List
    //***********************************************************************
    for (int i=0;i<(int)renderer->actorList.size();i++){
        renderer->actorList[i]->postLoad();
    }
    //then setup all actors, so properties are already present!
    for (int i=0;i<(int)renderer->actorList.size();i++){
            renderer->actorList[i]->setup();
    }

    //***********************************************************************
    //Load Button List
    //***********************************************************************
    listPos=0;

    cout << "loading buttons..." << endl;

      element=hRoot.FirstChild( "Button" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Button"))
        {
        cout << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        BasicButton* B=(BasicButton*)renderer->actorInfo[myType].actorReference;
        B->create();
        }

      element=hRoot.FirstChild( "Button" ).Element();
      for( ; element!=NULL; element=element->NextSiblingElement("Button"))
        {
        BasicButton* B=renderer->saveableButtonList[listPos];
        myType=element->GetText();
        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        B->load(element);
        B->setup();
        listPos++;
        }

    inspectorManager->setup();
}

void Input::loadMeshes(std::string fileName){

    TiXmlDocument doc(fileName);
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);
    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;

    // save this for later
    hRoot=TiXmlHandle(element);
    //***********************************************************************
    //Load OBJs
    //***********************************************************************


	  element=hRoot.FirstChild( "Mesh" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("Mesh")){
    /*
		  string meshID=element->Attribute("meshID");
        string meshFileName=element->Attribute("meshFilename");
        renderer->meshList[meshID]=LoadOBJ(meshFileName.c_str());
        cout << "loading mesh " << meshID << endl;
     */

	 }

    //***********************************************************************
    //Load complex Meshes
    //***********************************************************************
      element=hRoot.FirstChild( "ColladaMesh" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("ColladaMesh")){
        string meshID=element->Attribute("meshID");
        string meshFileName=element->Attribute("meshFilename");
        renderer->colladaLoader->loadColladaMesh(meshFileName, meshID);
        cout << "loading mesh " << meshID << endl;
      }

    //***********************************************************************
    //Load sprite Meshes
    //***********************************************************************
      element=hRoot.FirstChild( "SpriteMesh" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("SpriteMesh")){
        string meshID=element->Attribute("meshID");
        string meshFileName=element->Attribute("meshFilename");
        renderer->spriteMeshLoader->loadSpriteMesh(meshFileName, meshID);
        cout << "loading sprite mesh " << meshID << endl;
      }

      element=hRoot.FirstChild( "SpriteMeshXML" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("SpriteMeshXML")){
        string meshID=element->Attribute("meshID");
        string meshFileName=element->Attribute("meshFilename");
        renderer->spriteMeshLoaderXML->loadSpriteMesh(meshFileName, meshID);
        cout << "loading legacy XML sprite mesh " << meshID << endl;
      }

}

void Input::loadPrefab(std::string fileName){

    //loading is a two step process
    int listPos=renderer->actorList.size();


    cout << "loading Prefab from... "<< fileName << endl;
    //first create all Actors
    TiXmlDocument doc(fileName);
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    element=hDoc.FirstChildElement().Element();
    element= element->FirstChildElement("Actor");

      string myType;
      for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
        {
        cout << "next element: " << element->Value() << " " << element->GetText() <<endl;
        myType=element->GetText();
        Actor * A=renderer->actorInfo[myType].actorReference;
        A->create();
        }


    //then load all properties - for referencing reasons
    element=hDoc.FirstChildElement().Element();
    element= element->FirstChildElement("Actor");

    //for relative loading
    int loadPos=listPos;

      for( ; element!=NULL; element=element->NextSiblingElement("Actor"))
        {
        Actor* A=renderer->actorList[listPos];

        //***********************************************************************
        //Fill up Properties
        //***********************************************************************
        A->actorOffset=loadPos;         //for relative actor references

        A->load(element);

//        if (A->base)
//            A->baseMatrix=A->baseMatrix;
            //cout << A->base->name << endl;


        A->actorOffset=0;               //set back to zero
        A->setup();
        selectedActors.push_back(A);                                         //push newly created buttons in selection stack
        createNewSelectionMarker(A);
        listPos++;
        }

 //   makeGroup();
}

void Input::loadAction(std::string fileName){


    TiXmlDocument doc(fileName);
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    //***********************************************************************
    //Load Action
    //***********************************************************************
    cout << "loading actions..." << endl;

    element=hDoc.FirstChildElement().Element();
    element= element->FirstChildElement("Action");
    std::string myType=element->GetText();
    Action* AC=(Action*)renderer->actorInfo[myType].actorReference;
    AC->create();

    AC=(Action*)renderer->buttonList.back();
    AC->load(element);
    renderer->actionList[AC->name]=AC;
    AC->setup();
}

void Input::loadTextures(string fileName){

    TiXmlDocument doc( fileName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);


    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;

    // save this for later
    hRoot=TiXmlHandle(element);

    //***********************************************************************
    //Load textures
    //***********************************************************************
      element=hRoot.FirstChild( "Texture" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("Texture")){
        int val=0;

        string texID=element->Attribute("textureID");
        string texFileName=element->Attribute("filename");
        string nextFrame="NULL";
        if (element->Attribute("nextFrame"))
            nextFrame=element->Attribute("nextFrame");

        float frameRate = 0.0f;
        if (element->Attribute("frameRate"))
        {
            element->Attribute("frameRate",&val);
            frameRate=float(val);
        }

        element->Attribute("bAlpha", &val);
        bool bAlpha=bool(val);

        element->Attribute("bWrap", &val);
        bool bWrap=bool(val);

        renderer->LoadTextureTGA(texFileName,bWrap,bAlpha, texID);
        renderer->textureList[texID]->nextTexture=nextFrame;
        renderer->textureList[texID]->frameRate=frameRate;
        cout << "loading texture " << texID << endl;
      }
}

void Input::loadShaders(string fileName){

    TiXmlDocument doc( fileName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);


    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;

    // save this for later
    hRoot=TiXmlHandle(element);

    //***********************************************************************
    //Load shaders
    //***********************************************************************
      element=hRoot.FirstChild( "Shader" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("Shader")){
        string shaderID=element->Attribute("shaderID");
        string vertexFileName=element->Attribute("vertexShaderFilename");
        string fragmentFileName=element->Attribute("fragmentShaderFilename");
        cout << "loading shader " << shaderID << endl;
        renderer->loadShader(vertexFileName,fragmentFileName, shaderID);
        cout << "finished loading shader " << shaderID << "-------------------------------------------------- " << endl;
      }
}

void Input::loadActionList(string fileName){

    TiXmlDocument doc( fileName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);


    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;

    // save this for later
    hRoot=TiXmlHandle(element);

    //***********************************************************************
    //Load Actions
    //***********************************************************************
      element=hRoot.FirstChild( "Action" ).Element();
      for ( ; element!=NULL ;element=element->NextSiblingElement("Action")){
        string actionFileName=element->Attribute("actionFilename");
        loadAction("resources/actions/"+actionFileName);
        cout << "loading action " << actionFileName << endl;
      }

}


void Input::addToLibrary(TiXmlElement* myElement){

    TiXmlDocument doc( "resources/my.library" );
    if (!doc.LoadFile()){
        cout << "could not find my.library!" << endl;
        return;
    }

    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    //***********************************************************************
    //Make sure this is a Moviesandbox file!
    //***********************************************************************

    element=hDoc.FirstChildElement().Element();
    if (!element) return;

    hRoot=TiXmlHandle(element);

    //check for duplicates!!
    element=element->FirstChildElement(myElement->Value());
    string myName=myElement->FirstAttribute()->Name();

    while (element && element->Attribute(myName)){
        string myAttribContent;
        string existingAttribContent;
        myAttribContent=*myElement->Attribute(myName);
        existingAttribContent=*(element->Attribute(myName));
        if (myAttribContent==existingAttribContent){
            TiXmlElement* yesNo = (TiXmlElement*)hRoot.Element()->ReplaceChild(element,*myElement);
            if (yesNo){
                doc.SaveFile("resources/my.library");
                cout << "replaced in my.library!!" << endl;
            }
            else
                exit(0);
            return;
        }
        element=element->NextSiblingElement();
    }
    //append our element to that - or just dump it in here.
    element=hDoc.FirstChildElement().Element();
    TiXmlElement* lastElementOfSameType=element->FirstChildElement(myElement->Value());
    if (lastElementOfSameType)
        element->InsertBeforeChild(lastElementOfSameType,*myElement);
    else
        element->LinkEndChild(myElement);

    doc.SaveFile("resources/my.library");
}

void Input::getAllDrawings(){

    cout << "getting drawings..." << endl;

    savedDrawings.clear();

#ifdef TARGET_WIN32

    //not used?
    //char path[MAX_PATH];
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( "resources\\drawings\\*", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
          {
		  cout << fd.cFileName << endl;
		  savedDrawings.push_back(fd.cFileName);
          }
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}
#endif

#ifdef TARGET_MACOSX
	string dir="resources/drawings";
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

    while ((dirp = readdir(dp)) != NULL) {
        savedDrawings.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif

}

void Input::getAllScenes(){

    cout << "getting scenes..." << endl;

    savedScenes.clear();

#ifdef TARGET_WIN32

    //never used?
    //char path[MAX_PATH];
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( "resources\\scenes\\*", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
		  savedScenes.push_back(fd.cFileName);
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}

#endif

#ifdef TARGET_MACOSX
	string dir="resources/scenes";
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

	while ((dirp = readdir(dp)) != NULL) {
        savedScenes.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif
}

void Input::getAllImages(){

    cout << "getting Images..." << endl;

    userImages.clear();

#ifdef TARGET_WIN32

    //char path[MAX_PATH];
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( "resources\\images\\*.tga", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
		  userImages.push_back(fd.cFileName);
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}
#endif

#ifdef TARGET_MACOSX
	string dir="resources/images";
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

	while ((dirp = readdir(dp)) != NULL) {
        userImages.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif

}

void Input::getAllBrushes(){


    cout << "getting Brushes..." << endl;

    userBrushes.clear();

#ifdef TARGET_WIN32

    //char path[MAX_PATH];
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( "resources\\brushes\\*", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
		  userBrushes.push_back(fd.cFileName);
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}
#endif
#ifdef TARGET_MACOSX
	string dir="resources/brushes";
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

	while ((dirp = readdir(dp)) != NULL) {
        userBrushes.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif
}

void Input::getAllPrefabs(){

#ifdef TARGET_WIN32

    //char path[MAX_PATH];
	WIN32_FIND_DATA fd;
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	HANDLE hFind = FindFirstFile( "resources\\prefabs\\*", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
	    do{
		if( !(fd.dwFileAttributes & dwAttr))
		  prefabs.push_back(fd.cFileName);
        }while (FindNextFile( hFind, &fd));

		FindClose( hFind);
	}
#endif
#ifdef TARGET_MACOSX
	string dir="resources/prefabs";
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return;
    }

	while ((dirp = readdir(dp)) != NULL) {
        prefabs.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return;

#endif
}


//************************************************************
//
//       3D and general Helper functions
//
//
//************************************************************

float Input::convertToGrid(float nonGrid){

return (gridSize*(int)(nonGrid/gridSize));// - nonGridRounded%gridSizeRounded)*0.001);
//return 0.0f;
}

float Input::setToRange(float min, float max, float value){
    //multiply by difference of max and min     //and add minimum
    return( value   *   (max-min)     +   min);
}


#ifdef TARGET_WIN32
string Input::openFileDialog(){


    string myFileName;

    typedef BOOL ( WINAPI *GetFileNameFromBrowse )( HWND hwnd,
                                                    LPWSTR pszFilePath,
                                                    UINT cchFilePath,
                                                    LPCWSTR pszWorkingDir,
                                                    LPCWSTR pszDefExt,
                                                    LPCWSTR pszFilters,
                                                    LPCWSTR szTitle );

    // Path buffer, specifies starting directory and on successful return
    // from open dialog also holds the selected file name
    wchar_t wszPath[MAX_PATH] = L".";

    // Load shell32 dll
    HMODULE hModule = LoadLibrary( "Shell32.dll" );
    if( !hModule ){
       return "NULL";
    }

    // Get procedure address
    GetFileNameFromBrowse GetFileNameFromBrowsePtr = ( GetFileNameFromBrowse )GetProcAddress( hModule, "GetFileNameFromBrowse" );

    // Show browse dialog
    if( GetFileNameFromBrowsePtr && GetFileNameFromBrowsePtr( 0, wszPath, MAX_PATH, 0, 0, L"", L"Nibu Open" )){

        int i = 0;
        while (wszPath[i] != 0)
        {
            myFileName += wszPath[i];
            i++;
        }

    }else
		myFileName="NULL";

    // Free loaded library
    FreeLibrary( hModule );

    return myFileName;
}
#endif

#ifdef TARGET_MACOSX
string Input::openFileDialog(){

	NavDialogCreationOptions dialogOptions;
	NavDialogRef dialog;
	NavReplyRecord replyRecord;
	CFURLRef cfURL = NULL;
	FSRef fileAsFSRef;
	OSStatus status;

	// Get the standard set of defaults
	status = NavGetDefaultDialogCreationOptions(&dialogOptions);
	require_noerr( status, CantGetNavOptions );

	// Make the window app-wide modal
	dialogOptions.modality = kWindowModalityAppModal;

	// Create the dialog
	status = NavCreateGetFileDialog(&dialogOptions, NULL, NULL, NULL, NULL, NULL, &dialog);
	require_noerr( status, CantCreateDialog );

	// Show it
	status = NavDialogRun(dialog);
	require_noerr( status, CantRunDialog );

	// Get the reply
	status = NavDialogGetReply(dialog, &replyRecord);
	require( ((status == noErr) || (status == userCanceledErr)), CantGetReply );

	// If the user clicked "Cancel", just bail
	if ( status == userCanceledErr ) goto UserCanceled;

	// Get the file
	status = AEGetNthPtr(&(replyRecord.selection), 1, typeFSRef, NULL, NULL, &fileAsFSRef, sizeof(FSRef), NULL);
	require_noerr( status, CantExtractFSRef );

	// Convert it to a CFURL
	cfURL = CFURLCreateFromFSRef(NULL, &fileAsFSRef);


   // cleanup dialog


	// Cleanup
	CantExtractFSRef:
	UserCanceled:
		verify_noerr( NavDisposeReply(&replyRecord) );
	CantGetReply:
	CantRunDialog:
		NavDialogDispose(dialog);
	CantCreateDialog:
	CantGetNavOptions:

	CFStringRef cfString = NULL;
	if ( cfURL != NULL )
	{
		cfString = CFURLCopyFileSystemPath( cfURL, kCFURLPOSIXPathStyle );
		CFRelease( cfURL );
	}

    // copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
	const int kBufferSize = 255;

	char folderURL[kBufferSize];
	Boolean bool1 = CFStringGetCString(cfString,folderURL,kBufferSize,kCFStringEncodingMacRoman);

	// append strings together

	string url1 = folderURL;
	string finalURL = url1;

	//printf("url %s\n", finalURL.c_str());

	return finalURL;
}
#endif
