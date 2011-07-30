

#include "propertyInspector.h"
#include "renderer.h"
#include "input.h"

PropertyInspector::PropertyInspector(){

    bOpen=false;
    initialLocation=location;
    tabWidth=350;
    listHeight=15;
    listWidth=150;
    listOffset=0;
    level=0;
    textureID="icon_advanced";
    oldParent=NULL;
    parent=NULL;

    scrollSize=128.0;
}

PropertyInspector::~PropertyInspector(){}

void PropertyInspector::refreshList(){

        if (sceneData->selectedActors.size()>0){
            parent=sceneData->selectedActors[0];
        }else{
            parent=NULL;
        }

        if (parent!=oldParent){

            assembleList();
            oldParent=parent;
        }

}



void PropertyInspector::assembleList(){


    listOffsetX=0;
    listOffsetY=0;

    //cleanup
    for (int i=0;i<(int)listButton.size();i++){
        listButton[i]->remove();
    }
    listButton.clear();

    if (scrollBar){
        scrollBar->remove();
        scrollBar=NULL;
    }

    if (sceneData->selectedActors.size()>0){

        cout << "assembling property list..." << endl;

        std::map <std::string, memberID>::iterator it;
        int i=0;

        cout << "selected actor name: " << sceneData->selectedActors[0]->name << endl;

        for ( it=sceneData->selectedActors[0]->property.begin() ; it != sceneData->selectedActors[0]->property.end(); it++ ){

            //find Actor* properties
            memberID * mID=&it->second;

            if (!mID->bShowProperty)
                continue;

            //Properties now have their own knowledge of what button type they should spawn!
            sceneData->actorInfo[mID->propertyButtonType].actorReference->create();
            listButton.push_back(sceneData->buttonList.back());
            listButton[i]->name=it->first;
            listButton[i]->buttonProperty=it->first;
            listButton[i]->textureID="icon_base";
            listButton[i]->parent=sceneData->selectedActors[0];
            listButton[i]->level=level+1;
            listButton[i]->bDrawName=true;
            listButton[i]->color=Vector4f(0.2,0.2,0.2,1.0);
            listButton[i]->sceneShaderID="color";

            listButton[i]->bPermanent=true;

            if (i>0 && listButton[i-1]->bIndividualListSize && !listButton[i-1]->bHidden){
                listOffsetY+=listButton[i-1]->scale.y-listHeight;// + listButtonDistance.y;
            }
            if (!listButton[i]->bIndividualListSize){
                if (listWidth>0)
                    listButton[i]->scale.x=listWidth;
                if (listHeight>0)
                    listButton[i]->scale.y=listHeight;
            }

            //if (!bOpen)
            //    listButton[i]->bHidden=true;
            placeButton(i,i);
            listButton[i]->setup();
            i++;
        }

    }

    if (listButton.size()>0)
        listSize.y=listButton[listButton.size()-1]->location.y+listButton[listButton.size()-1]->scale.y;
    else
        listSize.y=0;
   // if (bOpen)
   //     createScrollBar();
}



void PropertyInspector::create(){sceneData->addButton(this);}
