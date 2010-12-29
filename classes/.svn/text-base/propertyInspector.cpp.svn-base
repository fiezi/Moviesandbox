

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

    maxListItems=(int) ((renderer->screenY* 4.0/5.0) / listHeight );
    //maxListItems=10;
}

PropertyInspector::~PropertyInspector(){}

void PropertyInspector::refreshList(){

        if (input->selectedActors.size()>0){
            parent=input->selectedActors[0];
        }else{
            parent=NULL;
        }

        if (parent!=oldParent){

            assembleList();

            clickedLeft();
            clickedLeft();

            oldParent=parent;
        }

}



void PropertyInspector::assembleList(){

    //cleanup
    for (int i=0;i<(int)listButton.size();i++){
        listButton[i]->remove();
    }
    listButton.clear();

    if (input->selectedActors.size()>0){

        cout << "assembling property list..." << endl;

        std::map <std::string, memberID>::iterator it;
        int i=0;

        cout << "selected actor name: " << input->selectedActors[0]->name << endl;

        for ( it=input->selectedActors[0]->property.begin() ; it != input->selectedActors[0]->property.end(); it++ ){

            //find Actor* properties
            memberID * mID=&it->second;

            const std::type_info* mType;
            mType=&(typeid(Actor*));

            if (mID->memberType->name()==mType->name()){
                renderer->actorInfo["15PickWorldButton"].actorReference->create();
                listButton.push_back(renderer->buttonList.back());
                }
            else{
                renderer->actorInfo["15TextInputButton"].actorReference->create();
                listButton.push_back(renderer->buttonList.back());
                }

            listButton[i]->name=it->first;
            listButton[i]->buttonProperty=it->first;
            listButton[i]->textureID="icon_base";
            listButton[i]->parent=input->selectedActors[0];
            listButton[i]->level=level+1;
            listButton[i]->bDrawName=true;
            listButton[i]->color=Vector4f(0.2,0.2,0.2,1.0);
            listButton[i]->sceneShaderID="color";

            listButton[i]->bPermanent=true;

            //if (!bOpen)
            //    listButton[i]->bHidden=true;
            placeButton(i,i);
            listButton[i]->setup();
            i++;
        }

    }

   // if (bOpen)
   //     createScrollBar();
}


void PropertyInspector::create(){renderer->addButton(this);}
