/*
    Inspector Base Class
*/

#include "inspector.h"
#include "renderer.h"
#include "sceneData.h"
#include "inspectorManager.h"

Inspector::Inspector(){

    bOpen=false;
    bToggleInspector=true;
    bPermanent=true;
    initialLocation=location;
    tabWidth=350;
    tabHeight=0;
    listHeight=64;
    listWidth=48;
    listDisplayMode=4;
    listColumns=1;
    level=0;

    textureID="icon_ball";

    backgroundButton=NULL;

    listOffsetY=32;

    registerProperties();
}

Inspector::~Inspector(){}


/*
void Inspector::myTab::assembleList(){

    listOffsetX=0;
    listOffsetY=0;
       cout << "creating list..." << endl;

}
*/


void Inspector::setup(){

    TabbedListButton::setup();

    buttonColor=sceneData->inspectorColor;

    Vector4f backgroundColor=sceneData->menuBackgroundColor;

    backgroundButton=new BasicButton;
    sceneData->buttonList.push_back(backgroundButton);
    backgroundButton->sceneShaderID="buttonColor";
	backgroundButton->textureID="icon_flat";
    //do not do mouseOver highlighting for background of inspectors...
    if (tabHeight==0){
        backgroundButton->scale.x=350;
        backgroundButton->scale.y=renderer->windowY-location.y;
        backgroundButton->location.x=location.x+scale.x;
        backgroundButton->location.y=location.y;
    }else{
        backgroundButton->scale.x=renderer->windowX;
        backgroundButton->scale.y=renderer->windowY;
        backgroundButton->location.x=location.x;
        backgroundButton->location.y=location.y+scale.y;
    }

    backgroundButton->level=level;
    backgroundButton->bPermanent=true;
    backgroundButton->name="";
    backgroundButton->setLocation(backgroundButton->location);
    backgroundButton->setup();
    backgroundButton->buttonColor=backgroundColor;
    backgroundButton->mouseOverColor=backgroundButton->buttonColor;


    //setup tabs and fill buttonlist once
    tabs.clear();
    tabs.push_back( new MyTab(this) );

    tabs[0]->assembleList();

    //create inspectorButtons
    createInspectorButtons();

    myManager=sceneData->inspectorManager;

}

void Inspector::update(double deltaTime){

    BasicButton::update(deltaTime);

    refreshList();

    //check if we need scrollBar and make one if we do
	if (scrollBar){
		if (listSize.y>listDisplaySize){
			trigger(scrollBar);
			placeScrollBar();
		}else {
			scrollBar->remove();
			scrollBar=NULL;
		}
        scrollBar->bHidden=!bOpen;
	}
	else{
		if (listSize.y>listDisplaySize){
			createScrollBar();
            scrollBar->bHidden=!bOpen;
		}
	}
}

void Inspector::createInspectorButtons(){

}

void Inspector::refreshList(){

}

void Inspector::drawPlane(){

    BasicButton::drawPlane();

    //renderer->setupShading("color");
}

void Inspector::mouseOver(){

ListButton::mouseOver();
}

void Inspector::mouseDrag(){}
void Inspector::finishDrag(){}

void Inspector::clickedLeft(){

    if (!bOpen){

        bOpen=true;

        myManager->inspectorOpened(this);

        initialLocation=location;
        setLocation(location-Vector3f(tabWidth, tabHeight, 0) );
        if (tabHeight==0)
            backgroundButton->setLocation(location + Vector3f(scale.x,0,0));
        else
            backgroundButton->setLocation(location + Vector3f(0,scale.y,0));

        for (int i=0;i<(int)listButton.size();i++){
            listButton[i]->bHidden=false;
            listButton[i]->setLocation( listButton[i]->location - Vector3f( tabWidth, tabHeight, 0 ) );
            listButton[i]->initialLocation-=Vector3f( tabWidth, tabHeight, 0 );
            }
        for (int i=0;i<(int)inspectorButtons.size();i++){
            inspectorButtons[i]->bHidden=false;
            inspectorButtons[i]->setLocation( inspectorButtons[i]->location - Vector3f( tabWidth, tabHeight, 0 ) );
            inspectorButtons[i]->initialLocation-=Vector3f( tabWidth, tabHeight, 0 );
        }
    }
    else{

        for (int i=0;i<(int)listButton.size();i++){
            listButton[i]->bHidden=true;
            listButton[i]->setLocation( listButton[i]->location + Vector3f( tabWidth, tabHeight, 0 ) );
            listButton[i]->initialLocation+=Vector3f( tabWidth, tabHeight, 0 );
        }
        for (int i=0;i<(int)inspectorButtons.size();i++){
            inspectorButtons[i]->bHidden=true;
            inspectorButtons[i]->setLocation( inspectorButtons[i]->location + Vector3f( tabWidth, tabHeight, 0 ) );
            inspectorButtons[i]->initialLocation+=Vector3f( tabWidth, tabHeight, 0 );
        }
        setLocation(initialLocation);
        if (tabHeight==0)
            backgroundButton->setLocation(location + Vector3f(scale.x,0,0));
        else
            backgroundButton->setLocation(location + Vector3f(0,scale.y,0));
        bOpen=false;

        myManager->inspectorClosed(this);
    }

    cout << "clicked on Inspector!" << endl;
}

void Inspector::assembleList(){

    listOffsetX=0;
    listOffsetY=0;
        cout << "creating list..." << endl;
}

void Inspector::clickedRight(){}

void Inspector::focusClick(){

ListButton::focusClick();
}

void Inspector::placeButton(int buttonNumber, int drawPosition){

    ListButton::placeButton(buttonNumber,drawPosition);
    listButton[buttonNumber]->setLocation(listButton[buttonNumber]->location );
    if (scrollBar)
        placeScrollBar();
}

void Inspector::placeScrollBar(){

    scrollBar->setLocation(Vector3f(location.x + scale.x - scrollBar->scale.x - 2, location.y+scale.y +2, 0) );
}

void Inspector::deselect(int depth){

//Button::deselect(depth);
}

void Inspector::create(){sceneData->addButton(this);}
