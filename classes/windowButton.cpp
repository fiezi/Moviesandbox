

#include "windowButton.h"
#include "renderer.h"
#include "input.h"
WindowButton::WindowButton(){

    bWidgetOpen=false;
    bToggleWidget=true;
    bPermanentList=true;

    bRelativeListLocation=false;
    listLoc.x=renderer->screenX/2-300;
    listLoc.y=renderer->screenY/2-200;
    listButtonDistance.x=80.0;

    currentTab=0;

    level = 1;
    listDisplayMode=4;

    listWidth = 128;
    listHeight = 16;
    listColumns = 3;
    maxListItems = 20;

    name="Settings";

    registerProperties();

}

WindowButton::~WindowButton(){}


void WindowButton::setup(){
    ListButton::setup();

}


void WindowButton::assembleList(){

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



}
//ROCCHINASCHOENEGRUESE

void WindowButton::deselect(int depth){

    if (!((Widget*)parent)->bWidgetOpen){
        //un-permanentize our buttons if our parent widget closes...
        for (int i=0;i<listButton.size();i++){
            listButton[i]->bPermanent=false;
            listButton[i]->level=100;
        }
        //kill list buttons!
        bPermanent=false;
        ListButton::deselect(1);
        backgroundButton->remove();
    }

}



void WindowButton::create(){sceneData->addButton(this);}
