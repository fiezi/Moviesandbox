

#include "windowButton.h"
#include "renderer.h"
#include "input.h"
#include "assignButton.h"

WindowButton::WindowButton(){

//    bWidgetOpen=false;
//    bToggleWidget=true;
    bPermanentList=false;

    backgroundButton=NULL;

    bRelativeListLocation=false;
    listLoc.x=renderer->windowX/2-300;
    listLoc.y=renderer->windowY/2-200;
    listButtonSpacing.x=80.0;

    currentTab=0;

    level = 0;
    listDisplayMode=4;

    listWidth = 128;
    listHeight = 16;
    listColumns = 3;

    name="Preferences";

    registerProperties();

}

WindowButton::~WindowButton(){}


void WindowButton::setup(){
    ListButton::setup();
}


void WindowButton::assembleList(){

    if (backgroundButton!=NULL){
        backgroundButton->remove();
        backgroundButton=NULL;
    }

    if (listButton.size()>0)
        for (int i=0;i<(int)listButton.size();i++)
            listButton[i]->remove();

    listButton.clear();


    backgroundButton=new BasicButton;
    backgroundButton->scale.x=700;
    backgroundButton->scale.y=400;
    backgroundButton->location.x=listLoc.x;
    backgroundButton->location.y=listLoc.y-20;

    backgroundButton->setLocation(backgroundButton->location);
    backgroundButton->sceneShaderID="color";
    backgroundButton->bTextured=false;
    backgroundButton->color=Vector4f(0.3,0.3,0.3,0.75);

    sceneData->buttonList.push_back(backgroundButton);

    std::map <std::string, memberID>::iterator it;
    int i=0;

    for ( it=renderer->property.begin() ; it != renderer->property.end(); it++ ){

        memberID * mID=&it->second;

        if (!mID->bShowProperty)
            continue;
        //find Actor* properties
        sceneData->actorInfo[mID->propertyButtonType].actorReference->create();
        listButton.push_back(sceneData->buttonList.back());

        listButton[i]->name=it->first;
        listButton[i]->buttonProperty=it->first;
        listButton[i]->textureID="icon_base";
        listButton[i]->parent=renderer;
        listButton[i]->level=level+1;
        listButton[i]->bDrawName=true;
        listButton[i]->color=Vector4f(0.2,0.2,0.2,1.0);
        listButton[i]->sceneShaderID="color";

        listButton[i]->bPermanent=true;

        if (listWidth>0)
            listButton[i]->scale.x=listWidth;
        if (listHeight>0)
            listButton[i]->scale.y=listHeight;

        listButton[i]->setup();
        placeButton(i,i);
        i++;
    }

   AssignButton* closeBtn= new AssignButton;
   sceneData->buttonList.push_back(closeBtn);
   listButton.push_back(closeBtn);
   closeBtn->name="x";
   closeBtn->bDrawName=true;
   closeBtn->setLocation(  Vector3f(listLoc.x+backgroundButton->scale.x-closeBtn->scale.x, backgroundButton->location.y,0) );
   closeBtn->parent=this;


}


void WindowButton::trigger(MsbObject* other){

    ListButton::trigger(other);

    //close window!
    if (other->name=="x"){
        if (backgroundButton!=NULL){
            backgroundButton->remove();
            backgroundButton=NULL;
        }

        if (listButton.size()>0)
            for (int i=0;i<(int)listButton.size();i++){
                listButton[i]->bPermanent=false;
                listButton[i]->remove();
            }

        listButton.clear();
    }



}

/*
void WindowButton::closeWidget(){

    Widget::closeWidget();
    backgroundButton->remove();
    backgroundButton=NULL;
}
*/
void WindowButton::deselect(int depth){

/*
    if (!((ListButton*)parent)->bListOpen){
        //un-permanentize our buttons if our parent widget closes...
        for (int i=0;i<(int)listButton.size();i++){
            listButton[i]->bPermanent=false;
            listButton[i]->level=100;
        }
        //kill list buttons!
        if (backgroundButton)
            backgroundButton->remove();
        bPermanent=false;
        ListButton::deselect(1);
    }
*/
}



void WindowButton::create(){sceneData->addButton(this);}
