

#include "layerInspector.h"
#include "renderer.h"
#include "input.h"
LayerInspector::LayerInspector(){
    level=0;
    listColumns=1;
    newLayer=NULL;
    listHeight=12;
    listWidth=200;
    registerProperties();
}

LayerInspector::~LayerInspector(){}


void LayerInspector::registerProperties(){

    Inspector::registerProperties();
    createMemberID("ACTORREFERENCES",&actorReferences, this);
}

void LayerInspector::setup(){

    Inspector::setup();

    tabs.clear();
    tabs.push_back( new ActorListTab(this) );
    tabs.push_back( new HelpersTab(this) );

    tabs[currentTab]->assembleList();

}


void LayerInspector::update(double deltaTime){

    Inspector::update(deltaTime);
    listDisplaySize= renderer->screenY-(location.y + 2.0* listHeight);
}

void LayerInspector::createInspectorButtons(){

    /*
    newLayer= new AssignButton;
    newLayer->parent=this;
    sceneData->buttonList.push_back(newLayer);
    newLayer->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    newLayer->name="new Layer";
    newLayer->color=Vector4f(1,0,0,1);
    inspectorButtons.push_back(newLayer);
    */

    AssignButton* tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+30.0f,location.y, 0.0f));
    tabButton->scale.x=64;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="color";
    tabButton->name="Actors";
    tabButton->color=selectedTabColor;
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);

    tabButton= new AssignButton;
    tabButton->parent=this;
    sceneData->buttonList.push_back(tabButton);
    tabButton->setLocation(Vector3f(location.x+100.0f,location.y, 0.0f));
    tabButton->scale.x=64;
    tabButton->scale.y=16;
    tabButton->sceneShaderID="color";
    tabButton->name="Helpers";
    tabButton->color=Vector4f(0.8,0.8,0.8,1.0);
    tabButton->bDrawName=true;
    inspectorButtons.push_back(tabButton);
    tabTriggerButtons.push_back(tabButton);

}

void LayerInspector::refreshList(){


    if (currentTab==0){

        int laySize=sceneData->layerList.size();
        int actSize=sceneData->actorList.size();
        int listSize=listButton.size();

        if ( listSize !=laySize + actSize ){
            assembleList();
            //open
            clickedLeft();
            //close
            clickedLeft();
            //or vice versa
        }

        //refresh Actor names
        for (int i=0;i<(int)actorButtons.size();i++){
            if ((int)actorReferences.size()>i && actorReferences[i])
                actorButtons[i]->name=actorReferences[i]->name;
        }
    }

    //currentTab==1 is assembled when it's clicked! We do not need to re-check every frame...
}

void LayerInspector::ActorListTab::assembleList(){


    mine->assembleList();

}

void LayerInspector::ActorListTab::trigger(MsbObject* other){


}


void LayerInspector::HelpersTab::assembleList(){


    //clean up
    LayerInspector* m=(LayerInspector*)mine;

    m->clearLists();

    //make assignbutton for brush
    //make assignbutton for grid
    //make assignbutton for controller
        BasicButton* l= new AssignButton;
        m->sceneData->buttonList.push_back(l);
        l->name="Brush";
        m->listButton.push_back(l);

        l= new AssignButton;
        m->sceneData->buttonList.push_back(l);
        l->name="Controller";
        m->listButton.push_back(l);

        l= new AssignButton;
        m->sceneData->buttonList.push_back(l);
        l->name="Grid";
        m->listButton.push_back(l);

        for (int i=0;i<(int)m->listButton.size();i++){
            l=m->listButton[i];
            l->parent=m;
            l->level=m->level+1;
            l->bDrawName=true;
            l->color=Vector4f(0.8,0.6,0.6,1.0);

            l->bPermanent=true;
            l->sceneShaderID="color";

            if (m->listWidth>0)
                l->scale.x=m->listWidth;
            if (m->listHeight>0)
                l->scale.y=m->listHeight;

            m->placeButton(i,i);
        }

    cout << "HelpersList: our Button list is: "<< mine->listButton.size() <<" elements long..." << endl;

    if (mine->listButton.size()>0)
        mine->listSize.y=mine->listButton[mine->listButton.size()-1]->location.y+mine->listButton[mine->listButton.size()-1]->scale.y - mine->location.y;
    else
        mine->listSize.y=0;

}

void LayerInspector::HelpersTab::trigger(MsbObject* other){


}

void LayerInspector::clearLists(){

    for (int i=0;i<(int)actorButtons.size();i++){
        actorButtons[i]->remove();
    }

    for (int i=0;i<(int)layerButtons.size();i++){
        layerButtons[i]->remove();
    }

    if (scrollBar){
        scrollBar->remove();
        scrollBar=NULL;
    }
    listButton.clear();
    actorButtons.clear();
    layerButtons.clear();
    actorReferences.clear();
}

void LayerInspector::assembleList(){

    //remove all buttons
    clearLists();

    for (int i=0;i<(int)sceneData->layerList.size();i++){
        AssignButton* l= new AssignButton;
        sceneData->buttonList.push_back(l);
        layerButtons.push_back(l);

        l->parent=this;
        l->name=sceneData->layerList[i]->name;
        l->level=level+1;
        l->bDrawName=true;
        l->color=Vector4f(0.8,0.6,0.6,1.0);

		//actions are just set to be visible, not actually created!
        l->bHidden=true;
        l->bDragable=true;
        l->bPermanent=true;
        l->sceneShaderID="color";

        if (listWidth>0)
            l->scale.x=listWidth;
        if (listHeight>0)
            l->scale.y=listHeight;

        //l->textureID=sceneData->layerList[i]->textureID;
        listButton.push_back(l);
        placeButton(listButton.size()-1,listButton.size()-1);

        for (int j=0;j<(int)sceneData->layerList[i]->actorList.size();j++){

            AssignButton* a= new AssignButton;
            sceneData->buttonList.push_back(a);
            actorButtons.push_back(a);
            actorReferences.push_back(sceneData->layerList[i]->actorList[j]);

            a->parent=this;

            a->name=sceneData->layerList[i]->actorList[j]->name;
            a->color=Vector4f(0.4,0.4,0.4,1.0);

            a->level=level+1;
            a->bDrawName=true;
            a->bHidden=true;
            a->bPermanent=true;

            //a->scale=Vector3f(tabWidth, 30.0f, 1.0f);


            if (listWidth>0)
                a->scale.x=listWidth;
            if (listHeight>0)
                a->scale.y=listHeight;


            a->sceneShaderID="color";
            a->color=Vector4f(0.6,0.6,0.6,1);
            listButton.push_back(a);
            placeButton(listButton.size()-1,listButton.size()-1);
        }
    }

}

void LayerInspector::trigger(MsbObject* other){

  /*
    if (other==newLayer){
        sceneData->addLayer("newLayer"+layerButtons.size());
    }
  */

    if (other->name=="Actors"){
        currentTab=0;
        tabs[currentTab]->assembleList();
    }

    if (other->name=="Helpers"){
        currentTab=1;
        tabs[currentTab]->assembleList();
    }


    if (other->name=="Brush"){
        input->deselectActors();
        //sceneData->brush->bSelected=true;
        sceneData->selectedActors.push_back(sceneData->brush);
    }
    if (other->name=="Controller"){
        input->deselectActors();
        //sceneData->controller->bSelected=true;
        sceneData->selectedActors.push_back(sceneData->controller);
    }

    if (other->name=="Grid"){
        input->deselectActors();
        sceneData->grid->bSelected=true;
        sceneData->selectedActors.push_back(sceneData->grid);
    }

    for (int i=0;i<(int)actorButtons.size();i++){
        if (other==actorButtons[i]){
            input->worldTarget=actorReferences[i];

            sceneData->controller->currentTool->selectActors(MOUSEBTNLEFT,input->worldTarget);
        }
    }

    Inspector::trigger(other);

}

void LayerInspector::create(){sceneData->addButton(this);}
