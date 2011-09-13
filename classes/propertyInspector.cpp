

#include "propertyInspector.h"
#include "renderer.h"
#include "input.h"
#include "assignButton.h"

PropertyInspector::PropertyInspector(){

    bOpen=false;
    initialLocation=location;
    tabWidth=350;
    listHeight=15;
    listWidth=150;
    //listOffsetY=20;
    level=0;
    textureID="icon_advanced";
    oldParent=NULL;
    parent=NULL;

    listDisplayMode=0;
    scrollSize=250.0;
    listDisplaySize=400;
}

PropertyInspector::~PropertyInspector(){}

void PropertyInspector::setup(){

    Inspector::setup();
    tabs.clear();
    tabs.push_back( new PropTabOne(this) );
    //tabs.push_back( new PropTabTwo(this) );

}

void PropertyInspector::createInspectorButtons(){

/*
    AssignButton* tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    tabButton->name="tabOne";
    tabButton->color=Vector4f(0.8,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);

    tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+60.0f,location.y, 0.0f));
    tabButton->name="tabTwo";
    tabButton->color=Vector4f(0.8,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);
*/
}

void PropertyInspector::refreshList(){

        if (sceneData->selectedActors.size()>0){
            parent=sceneData->selectedActors[0];
        }else{
            parent=NULL;
        }

        if (parent!=oldParent){

            tabs[currentTab]->assembleList();
            oldParent=parent;
        }

}

void PropertyInspector::PropTabOne::assembleList(){


//    mine->listOffsetX=0;
//    mine->listOffsetY=0;

    //cleanup
    for (int i=0;i<(int)mine->listButton.size();i++){
        mine->listButton[i]->remove();
    }
    mine->listButton.clear();

    if (mine->scrollBar){
        mine->scrollBar->remove();
        mine->scrollBar=NULL;
    }

    if (mine->sceneData->selectedActors.size()>0){

        cout << "assembling property list..." << endl;

        std::map <std::string, memberID>::iterator it;
        int i=0;

        cout << "selected actor name: " << mine->sceneData->selectedActors[0]->name << endl;

        Vector3f startListLoc=mine->listLoc;

        for ( it=mine->sceneData->selectedActors[0]->property.begin() ; it != mine->sceneData->selectedActors[0]->property.end(); it++ ){

            //find Actor* properties
            memberID * mID=&it->second;

            if (!mID->bShowProperty)
                continue;
            else
                cout << "processing property number " << i << " with name: " << it->first << endl;


            //Properties now have their own knowledge of what button type they should spawn!
            mine->sceneData->actorInfo[mID->propertyButtonType].actorReference->create();
            mine->listButton.push_back(mine->sceneData->buttonList.back());
            mine->listButton[i]->name=it->first;
            mine->listButton[i]->buttonProperty=it->first;
            mine->listButton[i]->textureID="icon_base";
            mine->listButton[i]->parent=mine->sceneData->selectedActors[0];
            mine->listButton[i]->level=mine->level+1;
            mine->listButton[i]->bDrawName=true;
            mine->listButton[i]->color=Vector4f(0.2,0.2,0.2,1.0);
            mine->listButton[i]->sceneShaderID="color";

            mine->listButton[i]->bPermanent=true;

            if (i>0 && mine->listButton[i-1]->bIndividualListSize && !mine->listButton[i-1]->bHidden){
                mine->listLoc+=mine->listButton[i-1]->scale.y-mine->listHeight;// + listButtonDistance.y;
            }
            if (!mine->listButton[i]->bIndividualListSize){
                if (mine->listWidth>0)
                    mine->listButton[i]->scale.x=mine->listWidth;
                if (mine->listHeight>0)
                    mine->listButton[i]->scale.y=mine->listHeight;
            }

            //if (!bOpen)
            //    listButton[i]->bHidden=true;

            mine->placeButton(i,i);
            mine->listButton[i]->setup();

            i++;
        }

        mine->listLoc=startListLoc;


    }

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y;
    else
        mine->listSize.y=0;

}

void PropertyInspector::PropTabTwo::assembleList(){

    //cleanup
    for (int i=0;i<(int)mine->listButton.size();i++){
        mine->listButton[i]->remove();
    }
    mine->listButton.clear();


}


void PropertyInspector::trigger(MsbObject* other){

    Inspector::trigger(other);

    if (other->name=="tabOne"){
        currentTab=0;
        tabs[currentTab]->assembleList();
    }

    if (other->name=="tabTwo"){
        currentTab=1;
        tabs[currentTab]->assembleList();
    }

}

void PropertyInspector::create(){sceneData->addButton(this);}
